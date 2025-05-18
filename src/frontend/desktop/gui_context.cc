#include "frontend/desktop/gui_context.h"

#include "base/thread.h"
#include "frontend/desktop/config.h"
#include "frontend/desktop/context.h"
#include "frontend/desktop/glfw.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "misc/freetype/imgui_freetype.h"

namespace yrtr {
namespace frontend {

namespace {
inline std::string_view get_log_header() { return "ImGuiWindow "; }

void* MemAllocFunc(size_t sz, void*) { return malloc(sz); }
void MemFreeFunc(void* ptr, void*) { free(ptr); }

// scarlet-light style from ImThemes
static void SetupStyle() {
  ImGuiStyle& style = ImGui::GetStyle();
  style.Alpha = 0.9f;
  style.DisabledAlpha = 0.6000000238418579f;
  style.WindowPadding = ImVec2(8.0f, 8.0f);
  style.WindowRounding = 0.0f;
  style.WindowBorderSize = 1.0f;
  style.WindowMinSize = ImVec2(20.0f, 20.0f);
  style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
  style.WindowMenuButtonPosition = ImGuiDir_Right;
  style.ChildRounding = 0.0f;
  style.ChildBorderSize = 1.0f;
  style.PopupRounding = 0.0f;
  style.PopupBorderSize = 1.0f;
  style.FramePadding = ImVec2(5.199999809265137f, 4.099999904632568f);
  style.FrameRounding = 0.0f;
  style.FrameBorderSize = 0.0f;
  style.ItemSpacing = ImVec2(8.0f, 7.599999904632568f);
  style.ItemInnerSpacing = ImVec2(8.399999618530273f, 4.300000190734863f);
  style.CellPadding = ImVec2(4.300000190734863f, 2.299999952316284f);
  style.IndentSpacing = 20.0f;
  style.ColumnsMinSpacing = 6.0f;
  style.ScrollbarSize = 20.0f;
  style.ScrollbarRounding = 0.0f;
  style.GrabMinSize = 15.89999961853027f;
  style.GrabRounding = 0.0f;
  style.TabRounding = 0.0f;
  style.TabBorderSize = 0.0f;
  style.ColorButtonPosition = ImGuiDir_Right;
  style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
  style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

  style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.9450980424880981f, 0.9411764740943909f, 0.9411764740943909f, 1.0f);
  style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.9803921580314636f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.300000011920929f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.6666666865348816f, 0.6313725709915161f, 0.6313725709915161f, 0.4000000059604645f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.9686274528503418f, 0.501960813999176f, 0.501960813999176f, 0.4000000059604645f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.95686274766922f, 0.1607843190431595f, 0.1607843190431595f, 0.4000000059604645f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.9686274528503418f, 0.501960813999176f, 0.501960813999176f, 0.8078431487083435f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.95686274766922f, 0.1607843190431595f, 0.1607843190431595f, 0.8111587762832642f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.9921568632125854f, 0.929411768913269f, 0.929411768913269f, 0.5098039507865906f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.5299999713897705f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.686274528503418f, 0.686274528503418f, 0.686274528503418f, 0.800000011920929f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4862745106220245f, 0.4862745106220245f, 0.4862745106220245f, 0.800000011920929f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4862745106220245f, 0.4862745106220245f, 0.4862745106220245f, 1.0f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.2470588237047195f, 0.01568627543747425f, 0.01568627543747425f, 0.4000000059604645f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4627451002597809f, 0.05490196123719215f, 0.05490196123719215f, 0.4000000059604645f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.4156862795352936f, 0.1372549086809158f, 0.04313725605607033f, 1.0f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.6666666865348816f, 0.6313725709915161f, 0.6313725709915161f, 0.4000000059604645f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9686274528503418f, 0.501960813999176f, 0.501960813999176f, 1.0f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.95686274766922f, 0.1607843190431595f, 0.1607843190431595f, 1.0f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.4627451002597809f, 0.05490196123719215f, 0.05490196123719215f, 0.1630901098251343f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.9686274528503418f, 0.501960813999176f, 0.501960813999176f, 0.800000011920929f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.95686274766922f, 0.1607843190431595f, 0.1607843190431595f, 1.0f);
  style.Colors[ImGuiCol_Separator] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 0.6200000047683716f);
  style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.9686274528503418f, 0.501960813999176f, 0.501960813999176f, 0.7803921699523926f);
  style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.95686274766922f, 0.1607843190431595f, 0.1607843190431595f, 1.0f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.3490196168422699f, 0.3490196168422699f, 0.3490196168422699f, 0.1700000017881393f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9686274528503418f, 0.501960813999176f, 0.501960813999176f, 0.6705882549285889f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.95686274766922f, 0.1607843190431595f, 0.1607843190431595f, 0.9490196108818054f);
  style.Colors[ImGuiCol_Tab] = ImVec4(0.9843137264251709f, 0.6745098233222961f, 0.7764706015586853f, 0.929411768913269f);
  style.Colors[ImGuiCol_TabHovered] = ImVec4(0.9764705896377563f, 0.5098039507865906f, 0.4235294163227081f, 0.800000011920929f);
  style.Colors[ImGuiCol_TabActive] = ImVec4(0.9686274528503418f, 0.501960813999176f, 0.501960813999176f, 1.0f);
  style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.9176470637321472f, 0.9254902005195618f, 0.9333333373069763f, 0.9861999750137329f);
  style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.7411764860153198f, 0.8196078538894653f, 0.9137254953384399f, 1.0f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8745098114013672f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.4470588266849518f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.9686274528503418f, 0.501960813999176f, 0.501960813999176f, 1.0f);
  style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.5686274766921997f, 0.5686274766921997f, 0.6392157077789307f, 1.0f);
  style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.6784313917160034f, 0.6784313917160034f, 0.7372549176216125f, 1.0f);
  style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.2980392277240753f, 0.2980392277240753f, 0.2980392277240753f, 0.09000000357627869f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.5372549295425415f, 0.47843137383461f, 0.47843137383461f, 0.3490196168422699f);
  style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.95686274766922f, 0.1607843190431595f, 0.1607843190431595f, 0.9490196108818054f);
  style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.95686274766922f, 0.1607843190431595f, 0.1607843190431595f, 0.800000011920929f);
  style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.6980392336845398f, 0.6980392336845398f, 0.6980392336845398f, 0.699999988079071f);
  style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f);
  style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.3499999940395355f);
}
}  // namespace

ImGuiWindow::ImGuiWindow(__YRTR_WINDOW_TYPE window)
    : window_(window),
      hdpi_scale_factor_(1.0f) {
  ImGui::SetAllocatorFunctions(MemAllocFunc, MemFreeFunc);
  // Setup high dpi scale factor.
  if (Config::instance()->enable_dpi_awareness()) {
    ImGui_ImplWin32_EnableDpiAwareness();
    hdpi_scale_factor_ = ImGui_ImplWin32_GetDpiScaleForHwnd(GetDesktopWindow());
    hdpi_scale_factor_ = std::max(hdpi_scale_factor_, 0.01f);
  }
  const float font_size_pixels = 15.0f * hdpi_scale_factor_;
  HLOG_F(INFO, "High dpi scale factor={:.2f}", hdpi_scale_factor_);
  // Setup Dear ImGui context.
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  // Disable imgui.ini
  io.IniFilename = NULL;
  // Enable Keyboard Controls.
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // Enable Gamepad Controls.
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  // Enable Docking.
  // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  // Allow popup windows outside of the main window.
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  DCHECK_NOTNULL(Config::instance());
  bool merge_mode = false;
  const fs::path& font_path = Config::instance()->font_path();
  if (font_path != "") {
    static const ImWchar ranges[] = {
        // 0x0020, 0x00FF,  // Basic Latin + Latin Supplement
        0x2000, 0x206F,  // General Punctuation
        0x3000, 0x30FF,  // CJK Symbols and Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF,  // Katakana Phonetic Extensions
        0xFF00, 0xFFEF,  // Half-width characters
        0xFFFD, 0xFFFD,  // Invalid
        0x4e00, 0x9FAF,  // CJK Ideograms
        0,
    };
    ImFont* font = io.Fonts->AddFontFromFileTTF(
        font_path.string().c_str(), font_size_pixels, nullptr, ranges);
    CHECK_NOTNULL(font);
    merge_mode = true;
  }
  const fs::path& fontex_path = Config::instance()->fontex_path();
  static const ImWchar rangesex[] = {0x1, 0x1FFFF, 0};
  static ImFontConfig cfg;
  cfg.OversampleH = cfg.OversampleV = 1;
  cfg.MergeMode = merge_mode;
  cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
  ImFont* fontemj = io.Fonts->AddFontFromFileTTF(
      fontex_path.string().data(), font_size_pixels, &cfg, rangesex);
  CHECK_NOTNULL(fontemj);
  io.Fonts->Build();
  // Setup Dear ImGui style
  SetupStyle();
  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
  style.ScaleAllSizes(hdpi_scale_factor_);
  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, /*install_callbacks*/ true);
  ImGui_ImplOpenGL3_Init("#version 130");
}

ImGuiWindow::~ImGuiWindow() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiWindow::UpdateViewport(int width, int height) {
  UNREFERENCED_PARAMETER(width);
  UNREFERENCED_PARAMETER(height);
  hdpi_scale_factor_ = ImGui_ImplWin32_GetDpiScaleForMonitor(
      MonitorFromWindow(glfwGetWin32Window(window_), MONITOR_DEFAULTTONEAREST));
  LOG_F(INFO, "High dpi scale factor={:.2f}", hdpi_scale_factor_);
}

void ImGuiWindow::BeginFrame() {
  DCHECK(IsWithinRendererThread());
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiWindow::EndFrame() {
  DCHECK(IsWithinRendererThread());
  ImGui::EndFrame();
}

void ImGuiWindow::Render() {
  DCHECK(IsWithinRendererThread());
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  // Update platform.
  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
  }
}

}  // namespace frontend
}  // namespace yrtr
