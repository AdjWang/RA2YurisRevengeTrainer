// Only run this script in tauri context.
import {
  BtnFnLabelFirst, BtnFnLabelLast,
  CheckboxFnLabelFirst, CheckboxFnLabelLast,
  strFnLabel
} from './protocol';

export function isTauriDesktop() {
  const isTauri = '__TAURI__' in window;
  const isWeb = !isTauri;
  const isMobile = navigator.maxTouchPoints > 0;

  // const isTauriMobile = isTauri && isMobile;
  const isTauriDesktop = isTauri && !isMobile;
  // const isWebMobile = isWeb && isMobile;
  // const isWebDesktop = isWeb && !isMobile;
  return isTauriDesktop;
}

// Get websocket port number.
export async function tauriGetWsPort() {
  if (!isTauriDesktop()) {
    return "";
  }
  const invoke = window.__TAURI__.core.invoke;
  return await invoke("get_ws_port");
}

export async function tauriGetLabelHotkeyString(labelName) {
  if (!isTauriDesktop()) {
    return "";
  }
  const invoke = window.__TAURI__.core.invoke;
  return await invoke("get_label_hotkey_string", { labelName: labelName });
}
