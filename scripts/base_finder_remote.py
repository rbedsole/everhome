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
from datetime import datetime, timezone
from pathlib import Path

SCHEMA = "everhome.base-finder.search.v1"


def fail(message: str) -> None:
    print(f"::error::{message}")
    raise SystemExit(2)


def main() -> int:
    if len(sys.argv) != 3:
        fail("usage: base_finder_remote.py SEARCH_CONFIG RESULTS")

    config_path = Path(sys.argv[1])
    result_path = Path(sys.argv[2])

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

    seed_present = bool(os.environ.get("EVERHOME_SEED", "").strip())
    status = "engine_pending" if seed_present else "blocked_missing_seed_secret"

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
            "worldgen_engine": "pending-cubiomes-26.2",
        },
        "message": (
            "Remote handoff is working. Cubiomes 26.2 candidate generation is the next integration step."
            if seed_present
            else "Set the repository Actions secret EVERHOME_SEED before the remote engine is connected."
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
