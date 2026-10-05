#!/usr/bin/env python3
"""Everhome Base Finder remote-runner entry point.

Phase 1 validates the shared search contract and GitHub Actions handoff without
pretending to produce Minecraft candidates before the 26.2 world-generation
engine is connected.
"""
from __future__ import annotations

import json
import os
import sys
import subprocess
from datetime import datetime, timezone
from pathlib import Path

SCHEMA = "everhome.base-finder.search.v1"


def fail(message: str) -> None:
    print(f"::error::{message}")
    raise SystemExit(2)


def main() -> int:
    if len(sys.argv) != 4:
        fail("usage: base_finder_remote.py SEARCH_CONFIG RESULTS CUBIOMES_PROBE")

    config_path = Path(sys.argv[1])
    result_path = Path(sys.argv[2])
    probe_path = Path(sys.argv[3])

    try:
        config = json.loads(config_path.read_text(encoding="utf-8"))
    except Exception as exc:
        fail(f"invalid search configuration: {exc}")

    if config.get("schema") != SCHEMA:
        fail(f"unsupported schema: {config.get('schema')!r}")

    area = config.get("search_area") or {}
    for key in ("center_x", "center_z", "radius_blocks", "coarse_step_blocks"):
        if key not in area:
            fail(f"search_area.{key} is required")

    criteria = config.get("criteria")
    if not isinstance(criteria, list):
        fail("criteria must be a list")

    seed_text = os.environ.get("EVERHOME_SEED", "").strip()
    seed_present = bool(seed_text)
    engine_probe = None
    status = "blocked_missing_seed_secret"

    if seed_present:
        try:
            probe = subprocess.run(
                [str(probe_path.resolve()), seed_text, str(area["center_x"]), "64", str(area["center_z"])],
                check=True, capture_output=True, text=True,
            )
            engine_probe = json.loads(probe.stdout)
            status = "engine_ready_search_logic_pending"
        except Exception as exc:
            fail(f"Cubiomes 26.2 engine probe failed: {exc}")

    result = {
        "schema": "everhome.base-finder.result.v1",
        "created_at": datetime.now(timezone.utc).isoformat(),
        "minecraft_version": config.get("minecraft_version"),
        "status": status,
        "candidate_count": 0,
        "candidates": [],
        "runner": {
            "mode": "github-actions",
            "seed_source": "EVERHOME_SEED",
            "seed_available": seed_present,
            "worldgen_engine": "cubiomes-26.2",
            "engine_probe": engine_probe,
        },
        "message": (
            "Cubiomes 26.2 is running against Everhome's seed. Candidate scoring remains disabled until the established Everhome criteria are mapped to objective tests."
            if seed_present
            else "Set the repository Actions secret EVERHOME_SEED to enable the remote engine."
        ),
        "search_definition": config,
    }

    result_path.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")

    summary = os.environ.get("GITHUB_STEP_SUMMARY")
    if summary:
        Path(summary).write_text(
            "# Everhome Base Finder\n\n"
            f"**Status:** `{status}`\n\n"
            f"**Minecraft:** {config.get('minecraft_version', 'unknown')}\n\n"
            f"**Criteria:** {len(criteria)}\n\n"
            f"{result['message']}\n",
            encoding="utf-8",
        )

    print(result["message"])
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
