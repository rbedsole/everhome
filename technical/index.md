---
layout: default
title: Technical
permalink: /technical/
---
<div class="section-hero">
  <div class="ever-kicker">Known-Good Reference</div>
  <h1>Technical</h1>
  <p>Configuration decisions, compatibility discoveries, custom fixes and troubleshooting knowledge that keep a long-lived modded world healthy.</p>
</div>

<div class="tech-platform">
  <div><small>Minecraft</small><strong>26.2</strong></div>
  <div><small>Server</small><strong>Fabric</strong></div>
  <div><small>Java</small><strong>25.0.1 LTS</strong></div>
  <div><small>Memory</small><strong>8192 MB</strong></div>
</div>

## Known-Good Rendering Stack

<div class="known-good">
  <div class="known-good-head"><span class="status-dot"></span><div><span class="ever-label">Pinned & Working</span><strong>Voxy distant-terrain stack</strong></div></div>
  <div class="config-grid">
    <div><small>Voxy</small><strong>0.2.19-beta</strong></div><div><small>Iris</small><strong>1.11.2</strong></div><div><small>Sodium</small><strong>0.9.1</strong></div><div><small>Render Distance</small><strong>512</strong></div><div><small>Server LOD</small><strong>512</strong></div><div><small>Receive Server LODs</small><strong>On</strong></div>
  </div>
  <p>Iris 1.11.4 + Sodium 0.9.2 prevented Voxy LODs beyond normal terrain. Returning to this combination restored expected behavior. Do not casually update it without a reason and a way to verify distant terrain.</p>
</div>

<div class="tech-grid">
  <article class="tech-card"><span class="ever-label">Lighting</span><h3>Beltborne Lantern</h3><p>LambDynamicLights <strong>First Person Lighting</strong> must remain enabled for the lantern's dynamic light to work.</p></article>
  <article class="tech-card"><span class="ever-label">Server Detail</span><h3>Voxy Server Side</h3><p>Installed with successful startup/backfill and client handshake. Preserve the working configuration while the rendering stack remains stable.</p></article>
  <article class="tech-card"><span class="ever-label">Custom Fix</span><h3>Zombie Aggro Tweaks</h3><p>Custom server-side behavior mod maintained specifically for Everhome.</p></article>
  <article class="tech-card"><span class="ever-label">Custom Fix</span><h3>Health Indicator+ Fix</h3><p>Everhome-specific compatibility fix maintained alongside Health Indicator+.</p></article>
</div>

## Stability Philosophy

<div class="ever-callout"><strong>Known-good beats merely newer.</strong><p>Changes should solve a problem, add something worthwhile, or support a deliberate Minecraft upgrade. When a compatibility problem is solved, the working combination gets recorded so the same dragon does not need to be slain twice.</p></div>

## Future Technical Library

<div class="tag-cloud"><span>Mods & placement</span><span>Server configuration</span><span>Rendering & shaders</span><span>Controller setup</span><span>Custom patches</span><span>Troubleshooting</span><span>Version upgrades</span><span>Backups & recovery</span></div>
