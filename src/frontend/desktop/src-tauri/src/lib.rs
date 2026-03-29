use global_hotkey::HotKeyState;
use log::{debug, error, info};
use once_cell::sync::OnceCell;
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::fs;
use std::path::PathBuf;
use std::str::FromStr;
use std::sync::Mutex;
use tauri::Emitter;
use tauri::Manager;
use tauri_plugin_global_shortcut::{Code, GlobalShortcutExt, Modifiers, Shortcut};
use tauri_plugin_log::{Target, TargetKind};

#[derive(Default)]
struct AppState {
    config: OnceCell<Config>,
    registered_hotkeys: HashMap<String /* label_name */, String /* key_name */>,
}

#[derive(Debug, Deserialize, Serialize)]
struct Config {
    ra2_trainer: Ra2TrainerConfig,
    hotkeys: HashMap<String /* label_name */, String /* key_name */>,
}

#[derive(Debug, Deserialize, Serialize)]
struct Ra2TrainerConfig {
    port: u16,
}

#[tauri::command]
fn get_ws_port(state: tauri::State<'_, Mutex<AppState>>) -> Result<u16, String> {
    let state = state.lock().unwrap();
    match state.config.get() {
        Some(config) => Ok(config.ra2_trainer.port),
        None => Err("Failed to load config. Check log file for more details.".to_string()),
    }
}

#[tauri::command]
fn get_key_name_by_label(state: tauri::State<'_, Mutex<AppState>>, label_name: &str) -> String {
    // Get key code from config.
    let state = state.lock().unwrap();
    match state.registered_hotkeys.get(label_name) {
        Some(key_name) => key_name.clone(),
        None => "".to_string(),
    }
}

fn read_config() -> Result<Config, String> {
    // Get the executable directory.
    let exe_path = std::env::current_exe().map_err(|e| format!("Failed to get exe path: {}", e))?;
    let config_path = exe_path.parent().unwrap().join("ra2_trainer.toml");
    // Read and parse config file.
    info!("Loading config from {:?}", config_path);
    let config_content = match fs::read_to_string(&config_path) {
        Ok(content) => content,
        Err(e) => {
            return Err(format!(
                "Failed to read config file at {:?}: {}",
                config_path, e
            ))
        }
    };
    match toml::from_str(&config_content) {
        Ok(c) => Ok(c),
        Err(e) => Err(format!("Failed to parse config: {}", e)),
    }
}

fn register_hotkeys(
    app: &tauri::App,
    hotkeys: &HashMap<String, String>,
) -> HashMap<String, String> {
    #[cfg(desktop)]
    {
        let mut registered_hotkeys = HashMap::new();
        // Register hotkeys.
        for (label_name, key_name) in hotkeys {
            let key_code: Code = Code::from_str(key_name).unwrap();
            let shortcut = Shortcut::new(Some(Modifiers::ALT), key_code);
            let shortcut_label_name = label_name.clone();
            match app
                .global_shortcut()
                .on_shortcut(shortcut, move |app_handle, shortcut, event| {
                    if event.state == HotKeyState::Pressed {
                        debug!("Hotkey event for label '{}': {:?}", shortcut.key, event);
                        app_handle
                            .emit(
                                "hotkey-pressed",
                                serde_json::json!({
                                    "key": format!("{}", shortcut.key),
                                    "labelName": shortcut_label_name,
                                }),
                            )
                            .unwrap();
                    }
                }) {
                Ok(_) => {
                    registered_hotkeys.insert(label_name.clone(), key_name.clone());
                    debug!("Registered hotkey for label '{}': {}", label_name, key_name);
                }
                Err(e) => error!(
                    "Failed to register hotkey for label '{}': {}",
                    label_name, e
                ),
            }
        }
        registered_hotkeys
    }
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_global_shortcut::Builder::new().build())
        .invoke_handler(tauri::generate_handler![
            get_ws_port,
            get_key_name_by_label,
        ])
        .setup(move |app: &mut tauri::App| {
            let app_path = std::env::current_exe().unwrap();
            let app_dir = app_path.parent().unwrap();
            app.handle().plugin(
                tauri_plugin_log::Builder::new()
                    .targets([
                        Target::new(TargetKind::Stdout),
                        Target::new(TargetKind::Folder {
                            path: PathBuf::from(app_dir),
                            file_name: Some("ra2_trainer_frontend.log".to_string()),
                        }),
                    ])
                    .build(),
            ).unwrap();
            assert!(app.manage(Mutex::new(AppState {
                config: OnceCell::new(),
                registered_hotkeys: HashMap::new(),
            })));
            let state = app.handle().state::<Mutex<AppState>>();
            let mut state_guard = state.lock().unwrap();
            // Load config.
            if let Some(_webview) = app.get_webview_window("main") {
                match read_config() {
                    Ok(config) => {
                        state_guard.config.set(config).unwrap();
                        state_guard.registered_hotkeys =
                            register_hotkeys(app, &state_guard.config.get().unwrap().hotkeys);
                    }
                    Err(e) => { error!("{}", e); }
                };
            }
            Ok(())
        })
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
