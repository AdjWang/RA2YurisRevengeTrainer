// Only run this script in tauri context.
import {
  BtnFnLabelFirst, BtnFnLabelLast,
  CheckboxFnLabelFirst, CheckboxFnLabelLast,
  getFnLabelHotkey
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

export async function registerHotkeys() {
  if (!isTauriDesktop()) {
    return;
  }
  const invoke = window.__TAURI__.core.invoke;
  for (let i = BtnFnLabelFirst; i <= BtnFnLabelLast; i++) {
    const ret = await invoke("register_hotkeys", { fnLabel: i, keyName: getFnLabelHotkey(i) });
    console.log(`register ${ret}`);
  }
  for (let i = CheckboxFnLabelFirst; i <= CheckboxFnLabelLast; i++) {
    const ret = await invoke("register_hotkeys", { fnLabel: i, keyName: getFnLabelHotkey(i) });
    console.log(`register ${ret}`);
  }
}
