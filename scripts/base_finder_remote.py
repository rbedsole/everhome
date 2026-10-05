#!/usr/bin/env python3
from __future__ import annotations
import csv, json, os, subprocess, sys
from datetime import datetime, timezone
from pathlib import Path

SCHEMA="everhome.base-finder.search.v1"
def fail(msg): print(f"::error::{msg}"); raise SystemExit(2)

def main():
    if len(sys.argv)!=4: fail("usage: base_finder_remote.py SEARCH_CONFIG RESULTS SEARCH_ENGINE")
    config_path,result_path,engine_path=map(Path,sys.argv[1:])
    try: config=json.loads(config_path.read_text())
    except Exception as exc: fail(f"invalid search configuration: {exc}")
    if config.get("schema")!=SCHEMA: fail("unsupported search schema")
    area=config.get("search_area") or {}
    for k in ("center_x","center_z","radius_blocks","coarse_step_blocks"):
        if k not in area: fail(f"search_area.{k} is required")
    radius=int(area["radius_blocks"]); step=int(area["coarse_step_blocks"])
    if radius < 1000: fail("radius_blocks must be at least 1000")
    if step < 128: fail("coarse_step_blocks must be at least 128 for remote searches")
    axis_samples=(2*radius)//step+1
    coarse_samples=axis_samples*axis_samples
    if coarse_samples > 50000:
        fail(f"remote search would test about {coarse_samples:,} coarse points; increase coarse_step_blocks or reduce radius (limit 50,000)")
    seed=os.environ.get("EVERHOME_SEED","").strip()
    if not seed: fail("EVERHOME_SEED repository secret is not set")

    csv_path=Path("refined-candidates.csv")
    cmd=[str(engine_path.resolve()),seed,str(int(area["center_x"])),str(int(area["center_z"])),
         str(int(area["radius_blocks"])),str(int(area["coarse_step_blocks"])),str(csv_path)]
    run=subprocess.run(cmd,text=True,capture_output=True)
    if run.returncode: fail("search engine failed: "+run.stderr[-3000:])

    candidates=[]
    with csv_path.open(newline="",encoding="utf-8") as f:
        for row in csv.DictReader(f):
            candidates.append({
                "rank":int(row["rank"]),"x":int(row["x"]),"z":int(row["z"]),"y":float(row["y"]),
                "plains_pct":float(row["plains_pct"]),"relief":float(row["relief"]),
                "avg_deviation":float(row["avg_deviation"]),
                "mountain":{"x":int(row["mountain_x"]),"z":int(row["mountain_z"]),"y":float(row["mountain_y"]),
                            "rise":float(row["rise"]),"distance":float(row["mountain_distance"]),"mass":float(row["mountain_mass"])},
                "score":float(row["score"])
            })

    result={
      "schema":"everhome.base-finder.result.v1","created_at":datetime.now(timezone.utc).isoformat(),
      "minecraft_version":config.get("minecraft_version"),"status":"complete",
      "candidate_count":len(candidates),"candidates":candidates,
      "algorithm":{
        "name":"Everhome recovered plains+mountain refinement",
        "settlement_core_blocks":224,"minimum_plains_percent":50,
        "mountain_distance_blocks":[160,800],"minimum_mountain_rise":45,
        "mountain_mass_threshold_above_settlement":40,"refinement_radius_blocks":512,
        "refinement_step_blocks":32,"final_deduplication_blocks":300,"maximum_results":150,
        "philosophy":"Permissive candidate finder; final terrain/build suitability remains human-reviewed."
      },
      "runner":{"mode":"github-actions","seed_source":"EVERHOME_SEED","worldgen_engine":"cubiomes-26.2-s8","coarse_samples":coarse_samples},
      "search_definition":config
    }
    result_path.write_text(json.dumps(result,indent=2)+"\n")
    summary=os.environ.get("GITHUB_STEP_SUMMARY")
    if summary:
        top=candidates[:10]
        lines=["# Everhome Base Finder","","**Status:** complete",f"**Candidates:** {len(candidates)}","",
               "| Rank | Settlement | Plains | Mountain rise | Distance | Score |","|---:|---|---:|---:|---:|---:|"]
        for c in top: lines.append(f"| {c['rank']} | {c['x']}, {c['z']} | {c['plains_pct']:.1f}% | {c['mountain']['rise']:.1f} | {c['mountain']['distance']:.1f} | {c['score']:.2f} |")
        Path(summary).write_text("\n".join(lines)+"\n")
    print(f"Everhome search complete: {len(candidates)} candidates")
    if run.stderr: print(run.stderr[-3000:])
    return 0
if __name__=="__main__": raise SystemExit(main())
