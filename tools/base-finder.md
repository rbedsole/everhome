---
layout: default
title: Base Finder
permalink: /tools/base-finder/
---

<div class="section-hero">
  <div class="ever-kicker">Everhome Tools</div>
  <h1>Base Finder</h1>
  <p>Build a search definition for Everhome's permanent home, run lightweight work locally, or hand the same job to GitHub Actions so the phone can go to sleep while the remote runner keeps working.</p>
</div>

<div class="finder-status">
  <span class="status-chip">Phase 1 · Control panel</span>
  <strong>Search engine integration is not connected yet.</strong>
  <p>This version safely creates, saves, exports and submits search definitions. It does not fabricate candidate locations while the Minecraft 26.2 world-generation engine is still being wired in.</p>
</div>

<div class="finder-grid">
  <section class="finder-panel">
    <span class="ever-label">World</span>
    <h2>Search source</h2>

    <label class="finder-field">
      <span>Minecraft version</span>
      <select id="bf-version">
        <option value="26.2" selected>Java 26.2</option>
      </select>
    </label>

    <label class="finder-field">
      <span>World seed</span>
      <input id="bf-seed" type="password" inputmode="numeric" autocomplete="off" placeholder="Stored only on this device">
      <small>The local page can remember the seed in this browser. Remote jobs should use the repository secret <code>EVERHOME_SEED</code> instead of putting the seed in a workflow input.</small>
    </label>

    <label class="finder-check">
      <input id="bf-save-seed" type="checkbox">
      <span>Remember seed on this device</span>
    </label>
  </section>

  <section class="finder-panel">
    <span class="ever-label">Search area</span>
    <h2>Where to look</h2>

    <div class="finder-two">
      <label class="finder-field"><span>Center X</span><input id="bf-center-x" type="number" value="0"></label>
      <label class="finder-field"><span>Center Z</span><input id="bf-center-z" type="number" value="0"></label>
    </div>

    <label class="finder-field"><span>Radius (blocks)</span><input id="bf-radius" type="number" min="1000" step="1000" value="50000"></label>
    <label class="finder-field"><span>Coarse sample spacing (blocks)</span><input id="bf-step" type="number" min="32" step="32" value="1024"></label>
    <small class="finder-help">The runner refines promising areas at 32-block resolution after this pass. 1024 blocks is the recommended starting spacing for a large remote search.</small>
  </section>
</div>

<section class="finder-panel finder-wide">
  <div class="finder-heading">
    <div>
      <span class="ever-label">Criteria</span>
      <h2>What makes a place worth visiting?</h2>
    </div>
    <button class="finder-button secondary" id="bf-add-criterion" type="button">Add criterion</button>
  </div>

  <p class="finder-help">Nothing is silently demoted. You decide whether each criterion is required, preferred, or a bonus. The engine will eventually report which requirements eliminate candidates rather than quietly changing your priorities.</p>

  <div id="bf-criteria" class="finder-criteria"></div>
</section>

<section class="finder-panel finder-wide">
  <span class="ever-label">Execution</span>
  <h2>Run it here or hand it off</h2>

  <div class="finder-mode-grid">
    <article class="finder-mode-card">
      <span class="dimension-chip">Desktop</span>
      <h3>Local search</h3>
      <p>Runs in this browser using the future Cubiomes/WebAssembly engine. Best for interactive tuning while the page stays open.</p>
      <button class="finder-button" id="bf-local-run" type="button" disabled>Worldgen engine not connected</button>
    </article>

    <article class="finder-mode-card">
      <span class="dimension-chip">Phone friendly</span>
      <h3>Remote search</h3>
      <p>Creates the same search payload for GitHub Actions. Once the workflow starts, the phone can lock or leave the page without stopping the runner.</p>
      <button class="finder-button" id="bf-copy-remote" type="button">Copy remote payload</button>
      <a class="finder-button secondary finder-link-button" id="bf-open-actions" href="https://github.com/rbedsole/everhome/actions/workflows/base-finder.yml" target="_blank" rel="noopener">Open remote runner</a>
    </article>
  </div>

  <div id="bf-message" class="finder-message" aria-live="polite"></div>
</section>

<section class="finder-panel finder-wide">
  <div class="finder-heading">
    <div>
      <span class="ever-label">Portable definition</span>
      <h2>Search JSON</h2>
    </div>
    <button class="finder-button secondary" id="bf-copy-json" type="button">Copy JSON</button>
  </div>
  <pre id="bf-json" class="finder-json"></pre>
</section>

<div class="ever-note"><strong>Remote privacy:</strong> the generated remote payload deliberately excludes the world seed. The workflow is designed to read the seed from a GitHub Actions repository secret named <code>EVERHOME_SEED</code>. Because the Everhome repository is public, do not paste the seed into workflow inputs if you want it kept private.</div>

<script src="{{ '/assets/base-finder.js' | relative_url }}" defer></script>
