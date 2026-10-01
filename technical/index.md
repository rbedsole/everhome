---
layout: default
title: Technical Notes
---

# Technical Notes

Everhome's technical reference for mods, configuration decisions, compatibility fixes, and troubleshooting.

## Known-Good Rendering Stack

- Minecraft 26.2
- Voxy 0.2.19-beta
- Iris 1.11.2
- Sodium 0.9.1
- Voxy render distance: 512
- Voxy Server Side LOD Distance: 512
- Receive Server LODs: On

The current Iris/Sodium versions are intentionally preserved because this combination restored Voxy distant terrain beyond vanilla render distance.

## Lighting

LambDynamicLights first-person lighting must remain enabled for the Beltborne Lantern dynamic light to work correctly.

## Philosophy

Known-good configurations take priority over unnecessary updates. Everhome is a forever world, so stability wins when the shiny new button offers no meaningful benefit.
