#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include <d3d9.h>

#include "base/logging.h"
#include "base/thread.h"
#include "config.h"
#include "gui/window.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "memory_api.h"

using namespace yrtr;

// Data
static LPDIRECT3D9 g_pD3D = nullptr;
static LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
static bool g_DeviceLost = false;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow) {
  logging::InitLogging(logging::LogSink::kStd);
  SetupGameLoopThreadOnce();
  SetupRendererThreadOnce();
  ImGui::SetAllocatorFunctions(MemAllocFunc, MemFreeFunc);
  // Load configurations.
  char exe_path[MAX_PATH] = {0};
  DWORD nSize = GetModuleFileNameA(hInstance, exe_path, _countof(exe_path));
  if (nSize == 0) {
    DWORD err = GetLastError();
    std::string message = std::system_category().message(err);
    LOG_F(FATAL, "Failed to get module file name, err=[{}]{}", err, message);
    UNREACHABLE();
  }
  // Search configuration file at the same directory with the dll.
  CHECK(Config::Load(fs::canonical(fs::path(exe_path)).parent_path(),
                     RuntimeEnv::kTest));
  gui::Window::Params params{
      .name = "Test window",
      .width = 1280,
      .height = 800,
      .with_border = true,
      .hInstance = hInstance,
      .nShowCmd = nCmdShow,
      .wndproc = WndProc,
      .hIcon = NULL,
      .hCursor = NULL,
  };
  // Create application window
  gui::Window::Init(params, [&params](void* data) {
    HWND hwnd = reinterpret_cast<HWND>(data);
    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd)) {
      CleanupDeviceD3D();
      ::UnregisterClassA(params.name.c_str(), params.hInstance);
      exit(1);
    }
    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);
  });
  gui::Window::instance()->SetD3DDevice(g_pd3dDevice);
  gui::Window::instance()->InitPannel();

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  bool done = false;
  while (!done) {
    // Poll and handle messages (inputs, window resize, etc.)
    // See the WndProc() function below for our to dispatch events to the Win32
    // backend.
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
      if (msg.message == WM_QUIT) done = true;
    }
    if (done) break;

    // Handle lost D3D9 device
    if (g_DeviceLost) {
      HRESULT hr = g_pd3dDevice->TestCooperativeLevel();
      if (hr == D3DERR_DEVICELOST) {
        ::Sleep(10);
        continue;
      }
      if (hr == D3DERR_DEVICENOTRESET) ResetDevice();
      g_DeviceLost = false;
    }

    // Handle window resize (we don't resize directly in the WM_SIZE handler)
    if (g_ResizeWidth != 0 && g_ResizeHeight != 0) {
      g_d3dpp.BackBufferWidth = g_ResizeWidth;
      g_d3dpp.BackBufferHeight = g_ResizeHeight;
      g_ResizeWidth = g_ResizeHeight = 0;
      ResetDevice();
    }

    void* ctx = gui::Window::instance()->PreRenderPannelGUI();
    CHECK_NOTNULL(ctx);

    // 1. Show the big demo window (Most of the sample code is in
    // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
    // ImGui!).
    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to create a named window.
    {
      static float f = 0.0f;
      static int counter = 0;
      // Create a window called "Hello, world!" and append into it.
      ImGui::Begin("Hello, world!");
      // Display some text (you can use a format strings too)
      ImGui::Text("This is some useful text.");
      // Edit bools storing our window open/close state
      ImGui::Checkbox("Demo Window", &show_demo_window);
      ImGui::Checkbox("Another Window", &show_another_window);
      // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      // Edit 3 floats representing a color
      ImGui::ColorEdit3("clear color", (float*)&clear_color);

      // Buttons return true when clicked (most widgets return true when
      // edited/activated)
      if (ImGui::Button("Button")) counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGuiIO& io = ImGui::GetIO();
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window) {
      // Pass a pointer to our bool variable (the window will have a closing
      // button that will clear the bool when clicked)
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me")) show_another_window = false;
      ImGui::End();
    }
    // 4. Scarlet template window.
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 10.0f));
    ImGui::Begin("🩸SCARLET");
    static bool enable = false;
    ImGui::Checkbox("Itemxxx", &enable);
    ImGui::SameLine();
    if (ImGui::Button("↻")) {}
    ImGui::End();
    ImGui::PopStyleVar(1);

    // Rendering
    ImGui::EndFrame();
    gui::Window::instance()->RenderPannelGUI(ctx);

    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    D3DCOLOR clear_col_dx =
        D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f),
                      (int)(clear_color.y * clear_color.w * 255.0f),
                      (int)(clear_color.z * clear_color.w * 255.0f),
                      (int)(clear_color.w * 255.0f));
    g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                        clear_col_dx, 1.0f, 0);
    if (g_pd3dDevice->BeginScene() >= 0) {
      gui::Window::instance()->Render();
      g_pd3dDevice->EndScene();
    }

    HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
    if (result == D3DERR_DEVICELOST) g_DeviceLost = true;
  }

  // Cleanup
  gui::Window::Destroy();
  CleanupDeviceD3D();
  return 0;
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd) {
  if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr) return false;

  // Create the D3DDevice
  ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
  g_d3dpp.Windowed = TRUE;
  g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  // Need to use an explicit format with alpha if needing per-pixel alpha
  // composition.
  g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
  g_d3dpp.EnableAutoDepthStencil = TRUE;
  g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;  // Present with vsync
  // Present without vsync, maximum unthrottled framerate
  // g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
  if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                           D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp,
                           &g_pd3dDevice) < 0)
    return false;

  return true;
}

void CleanupDeviceD3D() {
  if (g_pd3dDevice) {
    g_pd3dDevice->Release();
    g_pd3dDevice = nullptr;
  }
  if (g_pD3D) {
    g_pD3D->Release();
    g_pD3D = nullptr;
  }
}

void ResetDevice() {
  gui::Window* window = gui::Window::instance();
  if (window != nullptr) {
    window->InvalidateD3DDeviceObjs();
  }
  HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
  if (hr == D3DERR_INVALIDCALL) IM_ASSERT(0);
  if (window != nullptr) {
    window->CreateD3DDeviceObjs(g_pd3dDevice);
  }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0  // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if
// dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your
// main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to
// your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from
// your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

  switch (msg) {
    case WM_SIZE:
      if (wParam == SIZE_MINIMIZED) return 0;
      g_ResizeWidth = (UINT)LOWORD(lParam);  // Queue resize
      g_ResizeHeight = (UINT)HIWORD(lParam);
      return 0;
    case WM_SYSCOMMAND:
      if ((wParam & 0xfff0) == SC_KEYMENU)  // Disable ALT application menu
        return 0;
      break;
    case WM_DESTROY:
      ::PostQuitMessage(0);
      return 0;
    case WM_DPICHANGED:
      if (ImGui::GetIO().ConfigFlags &
          ImGuiConfigFlags_DpiEnableScaleViewports) {
        // const int dpi = HIWORD(wParam);
        // printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f *
        // 100.0f);
        const RECT* suggested_rect = (RECT*)lParam;
        ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top,
                       suggested_rect->right - suggested_rect->left,
                       suggested_rect->bottom - suggested_rect->top,
                       SWP_NOZORDER | SWP_NOACTIVATE);
      }
      break;
  }
  return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
