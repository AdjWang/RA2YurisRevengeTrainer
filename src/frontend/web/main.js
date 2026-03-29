import {
  BtnFnLabelFirst, BtnFnLabelLast,
  CheckboxFnLabelFirst, CheckboxFnLabelLast,
  FnLabel, strFnLabel
} from './protocol';
import { YRTRClient } from './client';
import { Localization } from './localization';
import { isTauriDesktop, tauriGetWsPort, tauriGetKeyNameByLabel } from './tauri_init';

var client = undefined;
var selectingHouseMap = new Map();
var protectedHouseMap = new Map();
var localization = new Localization();

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
  const addAllBtn = document.getElementById('AddAll');
  const clearAllBtn = document.getElementById('ClearAll');
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

async function getLabelTextContent(label) {
  let labelName = strFnLabel(label);
  let hotkeyStr = isTauriDesktop() ? await tauriGetKeyNameByLabel(labelName) : "";
  if (hotkeyStr != "") {
    return localization.getFnStr(`${labelName}`) + ` (${hotkeyStr})`;
  } else {
    return localization.getFnStr(`${labelName}`);
  }
}

async function initButton() {
  // Bind apply button with heading input.
  let apply_btn = document.getElementById('Apply');
  apply_btn.textContent = await getLabelTextContent(FnLabel.kApply);
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
    btn.id = strFnLabel(i);
    btn.textContent = await getLabelTextContent(i);
    btn.addEventListener('click', () => onTriggerBtn(i, /*val*/ undefined));
    btnList.appendChild(btn);
  }
}

async function createCheckbox(id, onChange) {
  const group = document.createElement('div');
  group.className = 'checkbox-group';
  const label = document.createElement('label');
  label.className = 'checkbox-label';
  const input = document.createElement('input');
  input.type = 'checkbox';
  input.id = strFnLabel(id);
  input.addEventListener('change', (e) => {
    const checked = input.checked;
    // Only allowed to chagnge state from this script.
    input.checked = !input.checked;
    onChange(checked);
  });
  const text = document.createElement('span');
  text.textContent = await getLabelTextContent(id);
  label.appendChild(input);
  label.appendChild(text);
  group.appendChild(label);
  return group;
}

async function initCheckbox() {
  let checkboxList = document.getElementById('checkbox-list');
  for (let i = CheckboxFnLabelFirst; i <= CheckboxFnLabelLast; i++) {
    const checkbox = await createCheckbox(i, (checked) => {
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
      const labelName = strFnLabel(label);
      const checkbox = document.getElementById(labelName);
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

function initClient(hostname, port) {
  client = new YRTRClient(`ws://${hostname}:${port}`, onStateUpdate);
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

function applyLocalization() {
  const gui_labels = [
    "Money",
    "Assist",
    "Filter",
    "SelectingHouseList",
    "ProtectedHouseList",
    "AddAll",
    "ClearAll",
  ];
  gui_labels.forEach(label => {
    let element = document.getElementById(label);
    if (element) {
      element.textContent = localization.getGuiStr(label);
    }
  });
}

document.addEventListener('DOMContentLoaded', async () => {
  initTab();
  initFilterList();
  await initButton();
  await initCheckbox();
  applyLocalization();
  if (isTauriDesktop()) {
    let port = await tauriGetWsPort();
    initClient("localhost", port);
  } else {
    initClient(window.location.hostname, window.location.port);
  }
});

// Listen event from tauri wrapper.
window.addEventListener('message', (event) => {
  if (event.data.event == "hotkey-pressed") {
    const keyName = event.data.payload.key;
    const labelName = event.data.payload.labelName;
    console.debug(`hotkey=${keyName} label=${labelName}`);
    const element = document.getElementById(labelName);
    if (element != null) {
      element.click();
    } else {
      console.error(`Failed to get element of id=${labelName}`);
    }
  }
});
