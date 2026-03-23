use once_cell::sync::OnceCell;
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::fs;
use std::str::FromStr;
use std::sync::Mutex;
use tauri::Emitter;
use tauri::Manager;
use tauri_plugin_global_shortcut::{Code, GlobalShortcutExt, Modifiers, Shortcut};

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
fn get_label_hotkey_string(state: tauri::State<'_, Mutex<AppState>>, label_name: &str) -> String {
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
    println!("Loading config from {:?}", config_path);
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

fn register_hotkeys(app: &tauri::App, hotkeys: &HashMap<String, String>) -> HashMap<String, String> {
    #[cfg(desktop)]
    {
        let mut registered_hotkeys = HashMap::new();
        let mut builder = tauri_plugin_global_shortcut::Builder::new();
        for (label_name, key_name) in hotkeys.clone() {
            let key_code = Code::from_str(&key_name).unwrap();
            let shortcut = Shortcut::new(Some(Modifiers::ALT), key_code);
            match app.global_shortcut().register(shortcut) {
                Ok(_) => {
                    registered_hotkeys.insert(
                        label_name.clone(),
                        key_name.clone(),
                    );
                    builder = builder.with_handler(move |app_handle, _shortcut, event| {
                        println!("Hotkey event for label '{}': {:?}", label_name, event);
                        app_handle.emit("hotkey-pressed", serde_json::json!({
                            "labelName": label_name.clone(),
                        })).unwrap();
                    });
                },
                Err(e) => println!("Failed to register hotkey for label '{}': {}", label_name, e),
            }
        }
        let _ = app.handle().plugin(builder.build());
        registered_hotkeys
    }
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_global_shortcut::Builder::new().build())
        .invoke_handler(tauri::generate_handler![get_label_hotkey_string])
        .setup(move |app: &mut tauri::App| {
            assert!(app.manage(Mutex::new(AppState{
                    config: OnceCell::new(),
                    registered_hotkeys: HashMap::new(),
            })));
            let state = app.handle().state::<Mutex<AppState>>();
            let mut state_guard = state.lock().unwrap();
            // Load config.
            if let Some(webview) = app.get_webview_window("main") {
                match read_config() {
                    Ok(config) => {
                        let backend_url = format!("http://localhost:{}", config.ra2_trainer.port);

                        state_guard.config.set(config).unwrap();
                        let registered_hotkeys =
                            register_hotkeys(app, &state_guard.config.get().unwrap().hotkeys);
                        state_guard.registered_hotkeys = registered_hotkeys;

                        // Pass the backend URL to the frontend.
                        let _ = webview.eval(&format!("window.backendUrl = '{}'; window.waitBackend();", backend_url));
                    }
                    Err(e) => {
                        let _ = webview.eval(&format!("window.errorMessage = '{}'; window.waitBackend();", e));
                    },
                };
            }
            Ok(())
        })
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
