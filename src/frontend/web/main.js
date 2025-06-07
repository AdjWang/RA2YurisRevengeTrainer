var client = undefined;
var selectingHouseMap = new Map();
var protectedHouseMap = new Map();

function initTab() {
  document.querySelectorAll('.tab').forEach(tab => {
    tab.addEventListener('click', () => {
      // Remove active class from all tabs and content
      document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
      document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));
      // Add active class to clicked tab and corresponding content
      tab.classList.add('active');
      const tabId = tab.getAttribute('data-tab');
      document.getElementById(tabId).classList.add('active');
    });
  });
}

function initFilterList() {
  // Source list functionality
  const sourceList = document.getElementById('source-list');
  const destinationList = document.getElementById('destination-list');
  const addAllBtn = document.getElementById('add-all-btn');
  const clearAllBtn = document.getElementById('clear-all-btn');
  // Add all items to destination list
  addAllBtn.addEventListener('click', () => {
    sourceList.querySelectorAll('.selectable').forEach(item => {
      const houseId = parseInt(item.id);
      const house = selectingHouseMap.get(houseId);
      protectedHouseMap.set(houseId, house);
      updateProtectedHouseList(protectedHouseMap);
    });
  });
  // Clear all items from destination list
  clearAllBtn.addEventListener('click', () => {
    destinationList.innerHTML = '';
    protectedHouseMap.clear();
    updateProtectedHouseList(protectedHouseMap);
  });
}

function initButton() {
  // Bind apply button with heading input.
  const apply_btn = document.getElementById('apply-btn');
  apply_btn.addEventListener('click', () => {
    const amount = document.getElementById('money-input').value;
    if (amount) {
      onTriggerBtn(FnLabel.kApply, parseInt(amount));
    } else {
      onTriggerBtn(FnLabel.kApply, 23333);
    }
  });
  // Add rest buttons.
  let btnList = document.getElementById('btn-list');
  for (let i = BtnFnLabelFirst; i <= BtnFnLabelLast; i++) {
    const btn = document.createElement('button');
    btn.id = `btn${i}`;
    btn.textContent = strFnLabel(i);
    btn.addEventListener('click', () => onTriggerBtn(i, /*val*/ undefined));
    btnList.appendChild(btn);
  }
}

function createCheckbox(fnLabel, content, onChange) {
  const group = document.createElement('div');
  group.className = 'checkbox-group';
  const label = document.createElement('label');
  label.className = 'checkbox-label';
  const input = document.createElement('input');
  input.type = 'checkbox';
  input.id = `checkbox${fnLabel}`;
  input.addEventListener('change', (e) => {
    const checked = input.checked;
    // Only allowed to chagnge state from this script.
    input.checked = !input.checked;
    onChange(checked);
  });
  const text = document.createElement('span');
  text.textContent = content;
  label.appendChild(input);
  label.appendChild(text);
  group.appendChild(label);
  return group;
}

function initCheckbox() {
  let checkboxList = document.getElementById('checkbox-list');
  for (let i = CheckboxFnLabelFirst; i <= CheckboxFnLabelLast; i++) {
    const checkbox = createCheckbox(i, strFnLabel(i), (checked) => {
      onTriggerCheckbox(i, checked);
    });
    checkboxList.appendChild(checkbox);
  }
}

function updateSelectingHouseList(houses) {
  // Do not clear all by set innerHTML and add all here, frequently changing
  // items even nothing changed causes click event randomly missing.
  const list = document.getElementById('source-list');
  // Update selecting map.
  selectingHouseMap.clear();
  houses.forEach(element => {
    const houseId = element[0];
    const house = element[1];
    selectingHouseMap.set(houseId, house);
  });
  // Remove items not in the map.
  let existHouseIds = new Set();
  list.querySelectorAll('.selectable').forEach(item => {
    const existHouseId = parseInt(item.id);
    existHouseIds.add(existHouseId);
    if (!selectingHouseMap.has(existHouseId)) {
      list.removeChild(item);
    }
  });
  // Add items not in the list.
  selectingHouseMap.forEach((house, houseId) => {
    if (!existHouseIds.has(houseId)) {
      const houseName = house.name;
      const item = document.createElement('div');
      item.id = houseId;
      item.className = 'selectable';
      item.textContent = houseName;
      item.addEventListener('click', () => {
        protectedHouseMap.set(houseId, house);
        updateProtectedHouseList(protectedHouseMap);
      });
      list.appendChild(item);
    }
  });
}

function updateProtectedHouseList(houses) {
  const list = document.getElementById('destination-list');
  list.innerHTML = '';
  houses.forEach((house, houseId) => {
    const houseName = house.name;
    const item = document.createElement('div');
    item.id = houseId;
    item.className = 'selectable';
    item.textContent = houseName;
    item.addEventListener('click', () => {
      protectedHouseMap.delete(houseId);
      updateProtectedHouseList(protectedHouseMap);
    });
    list.appendChild(item);
  });
  onUpdateProtectedHouseList(houses);
}

function onStateUpdate(state) {
  // Update your GUI with the new state
  console.debug('Received state update:', state);
  // Update checkboxes
  if (state.ckbox_states) {
    state.ckbox_states.forEach(element => {
      const label = element[0];
      const checkboxState = element[1];
      const checkbox = document.getElementById(`checkbox${label}`);
      if (checkbox) {
        checkbox.checked = checkboxState.activate;
        checkbox.disabled = !checkboxState.enable;
      }
    });
  }
  // Update house lists
  if (state.selecting_houses) {
    updateSelectingHouseList(state.selecting_houses);
  }
}

function initClient() {
  // TODO: set port
  client = new YRTRClient('ws://localhost:35271', onStateUpdate);
  // Connect to server
  client.connect();
  // Clean up on page unload
  window.addEventListener('beforeunload', function () {
    client.disconnect();
  });
}

function onTriggerBtn(fnLabel, val) {
  console.log(`Button [${fnLabel}]${strFnLabel(fnLabel)} clicked`);
  if (client) {
    if (fnLabel == FnLabel.kApply) {
      client.sendInput(FnLabel.kApply, val);
    } else {
      client.sendButton(fnLabel);
    }
  } else {
    console.warn(`Client not connecting to the server`);
  }
}

function onTriggerCheckbox(fnLabel, checked) {
  console.log(`Checkbox [${fnLabel}]${strFnLabel(fnLabel)} is now ${checked}`);
  if (client) {
    client.sendCheckbox(fnLabel, checked);
  } else {
    console.warn(`Client not connecting to the server`);
  }
}

function onUpdateProtectedHouseList(houses) {
  let sideMap = Array.from(houses);
  client.sendProtectedList(sideMap);
}

document.addEventListener('DOMContentLoaded', function () {
  initTab();
  initFilterList();
  initButton();
  initCheckbox();
  initClient();
});
