use global_hotkey::HotKeyState;
use log::{debug, error, info, warn};
use once_cell::sync::OnceCell;
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::fs;
use std::path::PathBuf;
use std::str::FromStr;
use std::sync::Mutex;
use std::time::Duration;
use tauri::Emitter;
use tauri::Manager;
use tauri_plugin_global_shortcut::{Code, GlobalShortcutExt, Modifiers, Shortcut};
use tauri_plugin_log::{Target, TargetKind};

// =============================================================================
// Global.
// =============================================================================

static CONFIG_FILENAME: &str = "ra2_trainer.toml";
static RUNTIME_CONFIG_FILENAME: &str = "ra2_trainer_runtime.toml";
static DEBOUNCE_DURATION: Duration = Duration::from_millis(1000);

#[derive(Default)]
struct AppState {
    cfg: OnceCell<Config>,
    rt_cfg: Option<RuntimeConfig>,
    registered_hotkeys: HashMap<String /* label_name */, String /* key_name */>,
}

// =============================================================================
// Static config. Required by user, never change in runtime.
// =============================================================================

#[derive(Debug, Deserialize, Serialize, Clone)]
struct Config {
    ra2_trainer: Ra2TrainerConfig,
    hotkeys: HashMap<String /* label_name */, String /* key_name */>,
}

#[derive(Debug, Deserialize, Serialize, Clone)]
struct Ra2TrainerConfig {
    port: u16,
}

// =============================================================================
// Runtime config. Optional, provided by user or generate in runtime.
// =============================================================================

#[derive(Debug, Deserialize, Serialize, Clone)]
struct RuntimeConfig {
    window: WindowConfig, // New window config section
}

#[derive(Debug, Deserialize, Serialize, Clone)]
struct WindowConfig {
    width: u32,
    height: u32,
}

impl Default for WindowConfig {
    fn default() -> Self {
        Self {
            width: 400,
            height: 600,
        }
    }
}

// =============================================================================
// Command functions.
// =============================================================================

#[tauri::command]
fn get_ws_port(state: tauri::State<'_, Mutex<AppState>>) -> Result<u16, String> {
    let state = state.lock().unwrap();
    match state.cfg.get() {
        Some(config) => Ok(config.ra2_trainer.port),
        None => Err("Failed to load config. Check log file for more details.".to_string()),
    }
}

#[tauri::command]
fn get_key_name_by_label(state: tauri::State<'_, Mutex<AppState>>, label_name: &str) -> String {
    let state = state.lock().unwrap();
    match state.registered_hotkeys.get(label_name) {
        Some(key_name) => key_name.clone(),
        None => "".to_string(),
    }
}

// =============================================================================
// Internal functions.
// =============================================================================

fn read_config() -> Result<Config, String> {
    let exe_path = std::env::current_exe().map_err(|e| format!("Failed to get exe path: {}", e))?;
    let config_path = exe_path.parent().unwrap().join(CONFIG_FILENAME);
    info!("Loading config from {:?}", config_path);
    if !config_path.exists() {
        warn!("Config file not found, creating default config");
        let default_config = Config {
            ra2_trainer: Ra2TrainerConfig { port: 8080 },
            hotkeys: HashMap::new(),
        };
        return Ok(default_config);
    }
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

fn read_runtime_config() -> Result<RuntimeConfig, String> {
    let exe_path = std::env::current_exe().map_err(|e| format!("Failed to get exe path: {}", e))?;
    let config_path = exe_path.parent().unwrap().join(RUNTIME_CONFIG_FILENAME);
    info!("Loading runtime config from {:?}", config_path);
    if !config_path.exists() {
        info!("Runtime config file not found, creating default");
        let default_config = RuntimeConfig {
            window: WindowConfig::default(),
        };
        save_runtime_config_to_path(&config_path, &default_config)?;
        return Ok(default_config);
    }
    let config_content = match fs::read_to_string(&config_path) {
        Ok(content) => content,
        Err(e) => {
            return Err(format!(
                "Failed to read runtime config file at {:?}: {}",
                config_path, e
            ))
        }
    };
    match toml::from_str(&config_content) {
        Ok(c) => Ok(c),
        Err(e) => Err(format!("Failed to parse runtime config: {}", e)),
    }
}

fn save_runtime_config_to_path(
    config_path: &PathBuf,
    config: &RuntimeConfig,
) -> Result<(), String> {
    let toml_string =
        toml::to_string(config).map_err(|e| format!("Failed to serialize config: {}", e))?;
    fs::write(config_path, toml_string).map_err(|e| format!("Failed to write config: {}", e))
}

fn save_runtime_config(config: &RuntimeConfig) -> Result<(), String> {
    let exe_path = std::env::current_exe().map_err(|e| format!("Failed to get exe path: {}", e))?;
    let config_path = exe_path.parent().unwrap().join(RUNTIME_CONFIG_FILENAME);
    save_runtime_config_to_path(&config_path, config)
}

fn start_window_size_saver(app_handle: tauri::AppHandle, init_size: (u32, u32)) {
    std::thread::spawn(move || {
        let mut last_saved_size = init_size;
        loop {
            // Check every debounce duration.
            std::thread::sleep(DEBOUNCE_DURATION);
            // Get current window size from state.
            let state = app_handle.state::<Mutex<AppState>>();
            let state_guard = state.lock().unwrap();
            if let Some(rt_cfg) = state_guard.rt_cfg.as_ref() {
                let current_size = (rt_cfg.window.width, rt_cfg.window.height);
                // Save if size changed.
                if current_size != last_saved_size {
                    if let Ok(_) = save_runtime_config(&rt_cfg) {
                        last_saved_size = current_size;
                        info!("Saved window size: {}x{}", current_size.0, current_size.1);
                    }
                }
            }
        }
    });
}

fn register_hotkeys(
    app: &tauri::App,
    hotkeys: &HashMap<String, String>,
) -> HashMap<String, String> {
    #[cfg(desktop)]
    {
        let mut registered_hotkeys = HashMap::new();
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
    #[cfg(not(desktop))]
    {
        HashMap::new()
    }
}

// =============================================================================
// Main.
// =============================================================================

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_global_shortcut::Builder::new().build())
        .invoke_handler(tauri::generate_handler![get_ws_port, get_key_name_by_label,])
        .setup(move |app: &mut tauri::App| {
            let app_path = std::env::current_exe().unwrap();
            let app_dir = app_path.parent().unwrap();
            let logger = tauri_plugin_log::Builder::new()
                .targets([
                    Target::new(TargetKind::Stdout),
                    Target::new(TargetKind::Folder {
                        path: PathBuf::from(app_dir),
                        file_name: Some("ra2_trainer_frontend.log".to_string()),
                    }),
                ])
                .build();
            app.handle().plugin(logger).unwrap();
            // Init global states.
            assert!(app.manage(Mutex::new(AppState {
                cfg: OnceCell::new(),
                rt_cfg: None,
                registered_hotkeys: HashMap::new(),
            })));
            if let Some(window) = app.get_webview_window("main") {
                let state = app.handle().state::<Mutex<AppState>>();
                let mut state_guard = state.lock().unwrap();
                // Load runtime config.
                match read_runtime_config() {
                    Ok(config) => {
                        // Apply window size.
                        let _ = window.set_size(tauri::PhysicalSize::new(
                            config.window.width,
                            config.window.height,
                        ));
                        info!(
                            "Applied saved window size: {}x{}",
                            config.window.width, config.window.height
                        );
                        // Set up window resize listener with debouncing.
                        let init_size = (config.window.width, config.window.height);
                        start_window_size_saver(app.handle().clone(), init_size);
                        // Register window on resize event.
                        let app_handle = app.handle().clone();
                        window.on_window_event(move |event| {
                            if let tauri::WindowEvent::Resized(size) = event {
                                debug!("Window resized to: {} x {}", size.width, size.height);
                                let state = app_handle.state::<Mutex<AppState>>();
                                let mut state_guard = state.lock().unwrap();
                                match state_guard.rt_cfg.as_mut() {
                                    Some(rt_cft) => {
                                        rt_cft.window.width = size.width;
                                        rt_cft.window.height = size.height;
                                    }
                                    None => error!("Runtime config not loaded"),
                                };
                            }
                        });
                        // Init state.
                        state_guard.rt_cfg = Some(config);
                    }
                    Err(e) => {
                        error!("Failed to load runtime config: {}", e);
                    }
                };
                // Load config.
                match read_config() {
                    Ok(config) => {
                        // Init state.
                        state_guard.registered_hotkeys = register_hotkeys(app, &config.hotkeys);
                        state_guard.cfg.set(config).unwrap();
                    }
                    Err(e) => {
                        error!("Failed to load config: {}", e);
                    }
                };
            }
            Ok(())
        })
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
