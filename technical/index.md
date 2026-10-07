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
  <div><small>Minecraft</small><strong>Java 26.2</strong></div>
  <div><small>Server</small><strong>Fabric</strong></div>
  <div><small>Java</small><strong>25.0.1</strong></div>
  <div><small>Client Memory</small><strong>2048–8192 MB</strong></div>
</div>

## World Identity

<div class="known-good">
  <div class="known-good-head"><span class="status-dot"></span><div><span class="ever-label">Everhome World</span><strong>Seed 888882571486312935</strong></div></div>
  <div class="config-grid">
    <div><small>Edition</small><strong>Java</strong></div><div><small>Production Version</small><strong>26.2</strong></div><div><small>Loader</small><strong>Fabric</strong></div><div><small>Launcher</small><strong>Prism Launcher 11.1.1</strong></div><div><small>Server Memory</small><strong>6 GB</strong></div><div><small>Java Runtime</small><strong>25.0.1 · 64-bit</strong></div>
  </div>
  <p>The seed is part of Everhome's permanent technical identity and should be kept with the repository rather than reconstructed from old records.</p>
</div>

## Known-Good Rendering Stack

<div class="known-good">
  <div class="known-good-head"><span class="status-dot"></span><div><span class="ever-label">Pinned & Working</span><strong>Voxy distant-terrain stack</strong></div></div>
  <div class="config-grid">
    <div><small>Voxy</small><strong>0.2.19-beta</strong></div><div><small>Iris</small><strong>1.11.2</strong></div><div><small>Sodium</small><strong>0.9.1</strong></div><div><small>Voxy Extra</small><strong>0.2.6</strong></div><div><small>Voxy World Gen V2</small><strong>2.4.3</strong></div><div><small>VoxyServer</small><strong>1.2.4</strong></div>
  </div>
  <p>Preserve known-good combinations unless an update solves a real problem. The Voxy family is also a first suspect if high-memory distant-terrain workloads cause another client heap failure.</p>
</div>

<div class="tech-grid">
  <article class="tech-card"><span class="ever-label">Memory</span><h3>Client Heap</h3><p>An October 2026 crash was a Java heap exhaustion at a 4096 MB maximum. The Everhome Prism profile should use 2048 MB minimum and 8192 MB maximum; if an 8 GB heap also fails, investigate the workload or a leak rather than blindly allocating huge amounts of RAM.</p></article>
  <article class="tech-card"><span class="ever-label">Lighting</span><h3>Dynamic Lighting</h3><p>The earlier belt lantern failure was caused by first-person lighting being disabled. Re-enabling first-person lighting restored portable light.</p></article>
  <article class="tech-card"><span class="ever-label">Custom Fix</span><h3>Zombie Aggro Tweaks</h3><p>Everhome uses this specifically to disable the zombie revenge mechanic. It should not be treated as a mod that increases zombie danger around villagers.</p></article>
  <article class="tech-card"><span class="ever-label">Construction</span><h3>WorldEdit & Litematica</h3><p>WorldEdit handles large-scale terrain work, excavation, corrections, and deliberate schematic placement. Litematica is available when a build should be constructed from a blueprint rather than pasted complete.</p></article>
  <article class="tech-card"><span class="ever-label">Interface</span><h3>REI</h3><p>REI is now installed client-side as well as being present in the server setup, resolving the earlier absence of the recipe/item interface on the client.</p></article>
</div>

## Settlement Direction

<div class="ever-callout"><strong>MineColonies is the planned settlement framework.</strong><p>The current intention is to use the unofficial Fabric MineColonies port for Everhome's inhabited settlement, unless a demonstrably better compatible alternative is found before installation. MineColonies would provide citizens, functional buildings, production chains, and Warehouse/Courier logistics. The player's residence and mountain master-storage system remain separate from the colony economy.</p></div>

## Infrastructure Mods to Remember

<div class="tag-cloud"><span>MineColonies · Planned</span><span>Bag Of Holding</span><span>Linked Chests</span><span>Nether Chested</span><span>Echo Chest</span><span>Hopper Gadgetry</span><span>FarmTweaks</span><span>Toss To Feed</span><span>Simple Quarries</span><span>Trading Post</span><span>Trade Cycling</span><span>Enchanting Infuser</span><span>Easy Magic</span><span>Easy Anvils</span><span>Universal Enchants</span><span>Magnum Torch</span></div>

<div class="ever-callout"><strong>Known-good beats merely newer.</strong><p>Changes should solve a problem, add something worthwhile, or support a deliberate Minecraft upgrade. When a compatibility problem is solved, record the working combination so the same dragon does not need to be slain twice.</p></div>
