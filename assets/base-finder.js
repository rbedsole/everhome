(() => {
  const $ = (id) => document.getElementById(id);
  const criteriaEl = $("bf-criteria");
  const messageEl = $("bf-message");
  const jsonEl = $("bf-json");

  const defaultCriteria = [
    { name: "Terrain / landscape quality", importance: "preferred", weight: 5, notes: "" },
    { name: "Mountain suitable for major builds", importance: "preferred", weight: 5, notes: "" },
    { name: "Access to useful nearby biomes", importance: "preferred", weight: 4, notes: "" }
  ];

  function uid() {
    return Math.random().toString(36).slice(2, 9);
  }

  function getCriteria() {
    return [...criteriaEl.querySelectorAll(".finder-criterion")].map((row) => ({
      name: row.querySelector('[data-field="name"]').value.trim(),
      importance: row.querySelector('[data-field="importance"]').value,
      weight: Number(row.querySelector('[data-field="weight"]').value || 1),
      notes: row.querySelector('[data-field="notes"]').value.trim()
    })).filter((c) => c.name);
  }

  function addCriterion(c = { name: "", importance: "preferred", weight: 3, notes: "" }) {
    const row = document.createElement("div");
    row.className = "finder-criterion";
    row.dataset.id = uid();
    row.innerHTML = `
      <label class="finder-field criterion-name">
        <span>Criterion</span>
        <input data-field="name" type="text" value="${escapeHtml(c.name)}" placeholder="Describe what matters">
      </label>
      <label class="finder-field">
        <span>Importance</span>
        <select data-field="importance">
          <option value="required" ${c.importance === "required" ? "selected" : ""}>Required</option>
          <option value="preferred" ${c.importance === "preferred" ? "selected" : ""}>Preferred</option>
          <option value="bonus" ${c.importance === "bonus" ? "selected" : ""}>Bonus</option>
        </select>
      </label>
      <label class="finder-field">
        <span>Weight</span>
        <input data-field="weight" type="number" min="1" max="10" value="${Number(c.weight || 3)}">
      </label>
      <label class="finder-field criterion-notes">
        <span>Notes / target</span>
        <input data-field="notes" type="text" value="${escapeHtml(c.notes || "")}" placeholder="Distance, shape, biome list, etc.">
      </label>
      <button type="button" class="finder-remove" aria-label="Remove criterion">×</button>
    `;
    row.querySelector(".finder-remove").addEventListener("click", () => {
      row.remove();
      refresh();
    });
    row.querySelectorAll("input,select").forEach((el) => el.addEventListener("input", refresh));
    criteriaEl.appendChild(row);
  }

  function escapeHtml(value) {
    return String(value)
      .replaceAll("&", "&amp;")
      .replaceAll("<", "&lt;")
      .replaceAll(">", "&gt;")
      .replaceAll('"', "&quot;");
  }

  function searchDefinition({ includeSeed = false } = {}) {
    const seed = $("bf-seed").value.trim();
    const data = {
      schema: "everhome.base-finder.search.v1",
      minecraft_version: $("bf-version").value,
      search_area: {
        center_x: Number($("bf-center-x").value || 0),
        center_z: Number($("bf-center-z").value || 0),
        radius_blocks: Number($("bf-radius").value || 50000),
        coarse_step_blocks: Number($("bf-step").value || 1024)
      },
      criteria: getCriteria(),
      execution: {
        engine: "pending-cubiomes-26.2",
        requested_mode: "hybrid"
      }
    };
    if (includeSeed && seed) data.seed = seed;
    return data;
  }

  function remoteDefinition() {
    const data = searchDefinition({ includeSeed: false });
    data.execution.requested_mode = "remote";
    data.execution.seed_source = "github_actions_secret:EVERHOME_SEED";
    return data;
  }

  function toBase64Unicode(obj) {
    const bytes = new TextEncoder().encode(JSON.stringify(obj));
    let binary = "";
    bytes.forEach((b) => binary += String.fromCharCode(b));
    return btoa(binary);
  }

  async function copyText(text, success) {
    try {
      await navigator.clipboard.writeText(text);
      setMessage(success, "ok");
    } catch {
      setMessage("Clipboard access was blocked. Select the JSON below and copy it manually.", "warn");
    }
  }

  function setMessage(text, type = "") {
    messageEl.textContent = text;
    messageEl.className = "finder-message" + (type ? " " + type : "");
  }

  function refresh() {
    const def = searchDefinition({ includeSeed: false });
    jsonEl.textContent = JSON.stringify(def, null, 2);
    localStorage.setItem("everhome-base-finder-config", JSON.stringify(def));

    if ($("bf-save-seed").checked && $("bf-seed").value.trim()) {
      localStorage.setItem("everhome-base-finder-seed", $("bf-seed").value.trim());
    } else {
      localStorage.removeItem("everhome-base-finder-seed");
    }
  }

  function loadSaved() {
    try {
      const saved = JSON.parse(localStorage.getItem("everhome-base-finder-config") || "null");
      if (saved?.search_area) {
        $("bf-center-x").value = saved.search_area.center_x ?? 0;
        $("bf-center-z").value = saved.search_area.center_z ?? 0;
        $("bf-radius").value = saved.search_area.radius_blocks ?? 50000;
        $("bf-step").value = saved.search_area.coarse_step_blocks ?? 1024;
      }
      const rows = Array.isArray(saved?.criteria) && saved.criteria.length ? saved.criteria : defaultCriteria;
      rows.forEach(addCriterion);
    } catch {
      defaultCriteria.forEach(addCriterion);
    }

    const seed = localStorage.getItem("everhome-base-finder-seed");
    if (seed) {
      $("bf-seed").value = seed;
      $("bf-save-seed").checked = true;
    }
  }

  $("bf-add-criterion").addEventListener("click", () => {
    addCriterion();
    refresh();
  });

  $("bf-copy-json").addEventListener("click", () =>
    copyText(JSON.stringify(searchDefinition({ includeSeed: false }), null, 2), "Search JSON copied.")
  );

  $("bf-copy-remote").addEventListener("click", () => {
    const payload = toBase64Unicode(remoteDefinition());
    copyText(payload, "Remote payload copied. Open the remote runner, choose Run workflow, and paste it into search_config_b64.");
  });

  $("bf-seed").addEventListener("input", refresh);
  $("bf-save-seed").addEventListener("change", refresh);
  ["bf-version","bf-center-x","bf-center-z","bf-radius","bf-step"].forEach((id) => {
    $(id).addEventListener("input", refresh);
    $(id).addEventListener("change", refresh);
  });

  loadSaved();
  refresh();
})();
