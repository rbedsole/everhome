---
layout: default
title: Technical
permalink: /technical/
---

# Technical

Everhome's technical reference.

This section records the configuration decisions, compatibility discoveries, custom fixes, server details, and troubleshooting knowledge needed to keep a long-lived modded world healthy. It documents **known-good state**, not every setting that has ever been touched.

## Current Platform

- **Minecraft:** 26.2
- **Server:** Fabric
- **Server manager:** MC Server Manager
- **Java:** 25.0.1 LTS
- **Server memory:** 8192 MB

---

## Known-Good Rendering Stack

Everhome's current distant-terrain setup is intentionally pinned to:

- **Voxy:** 0.2.19-beta
- **Iris:** 1.11.2
- **Sodium:** 0.9.1
- **Voxy render distance:** 512
- **Voxy Server Side LOD Distance:** 512
- **Receive Server LODs:** On

A newer Iris 1.11.4 + Sodium 0.9.2 combination prevented Voxy LODs from appearing beyond normal terrain. Returning to Iris 1.11.2 + Sodium 0.9.1 restored the expected behavior.

These versions should not be casually updated without a reason and a way to verify that distant terrain still works.

---

## Lighting

### Beltborne Lantern + LambDynamicLights

Beltborne Lantern dynamic lighting depends on **First Person Lighting** being enabled in LambDynamicLights.

When first-person lighting was disabled, the lantern appeared to stop working. Re-enabling it restored the expected dynamic light.

---

## Server-Side World Detail

Voxy Server Side is installed and has successfully completed its startup/backfill process and client handshake.

Its current configuration is part of the known-good Voxy setup and should be preserved while the client rendering stack remains stable.

---

## Custom Everhome Fixes

Everhome uses custom or world-specific fixes where necessary rather than modifying unrelated mods blindly.

Current examples include:

- **Zombie Aggro Tweaks**, a custom server-side behavior mod.
- **Everhome Health Indicator+ Fix**, maintained alongside Health Indicator+.

Technical notes for these can be expanded into dedicated pages when their implementation history becomes useful to preserve.

---

## Stability Philosophy

Everhome is intended to survive for a long time. A newer version is not automatically a better version for this world.

Known-good configurations take priority over unnecessary updates. Changes should solve a problem, add something worthwhile, or support a deliberate Minecraft upgrade. When a compatibility issue is discovered and resolved, the working combination should be recorded here so the same dragon does not need to be slain twice.

---

## Future Technical Library

As this section grows, it can split into dedicated references for:

- Mods and client/server placement
- Server configuration
- Rendering and shaders
- Controller configuration
- Custom Everhome mods and patches
- Troubleshooting history
- Minecraft version upgrades
- World backup and recovery procedures

The Technical section is deliberately separate from the World Journal. A technical incident can be part of the world's history when it matters, but troubleshooting documentation should remain easy to find without digging through adventure entries.
