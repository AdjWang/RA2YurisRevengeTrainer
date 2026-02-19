use tauri::Manager;

#[tauri::command]
fn register_hotkeys(fn_label: i64, key_name: &str) -> String {
    format!("fn_label={} key_name={}", fn_label, key_name)
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    let backend_url = "http://localhost:35271";

    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![register_hotkeys])
        .setup(move |app: &mut tauri::App| {
            // Pass the backend URL to the frontend
            if let Some(webview) = app.get_webview_window("main") {
                let _ = webview.eval(&format!(
                    "window.backendUrl = '{}'; window.checkBackend();",
                    backend_url
                ));
            }
            Ok(())
        })
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
