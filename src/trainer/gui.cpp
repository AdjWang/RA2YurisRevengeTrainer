#include "common/logging.h"
#include "common/win32utils.h"
#include "trainer/gui.h"
#include "trainer/char_table.h"

namespace yrtr {

const ImWchar* GetGlyphRangesChinese() {
    static const ImWchar ranges[] = {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        // 0x2000, 0x206F, // General Punctuation
        // 0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
        // 0x31F0, 0x31FF, // Katakana Phonetic Extensions
        // 0xFF00, 0xFFEF, // Half-width characters
        // 0xFFFD, 0xFFFD, // Invalid
        0x4e00, 0x9FAF, // CJK Ideograms
        0,
    };
    return &ranges[0];
}

ImGuiContext::ImGuiContext(GLFWwindow* window) {
    // setup high dpi scale factor
    ImGui_ImplWin32_EnableDpiAwareness();
    hdpi_scale_factor_ = ImGui_ImplWin32_GetDpiScaleForHwnd(GetDesktopWindow());
    LOG(INFO, "high dpi scale factor={:.2f}", hdpi_scale_factor_);
    // setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    // Load fonts
    std::filesystem::path font_path(win32::GetDefaultFontPath() /
                                    "simhei.ttf");
    if (!std::filesystem::exists(font_path)) {
        LOG(WARN, "Not found font={}", font_path.string());
    }
    ImFont* font = io.Fonts->AddFontFromFileTTF(
        font_path.string().c_str(),
        18.0f * hdpi_scale_factor_,
        nullptr,
        GetGlyphRangesChinese());
    CHECK(font != nullptr);
    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();
    // ImGui::StyleColorsClassic();
    ImGui::GetStyle().ScaleAllSizes(hdpi_scale_factor_);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

ImGuiContext::~ImGuiContext() {
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();
}

void ImGuiContext::PreRender() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiContext::Render() const {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#define HANDLE_BUTTON(label)                                      \
    do {                                                          \
        if (ImGui::Button(GetFnChar(FnLabel::k##label))) {        \
            if (btn_cbs_.contains(FnLabel::k##label)) {           \
                btn_cbs_[FnLabel::k##label]();                    \
            } else {                                              \
                LOG(WARN, "Not found handler for label=" #label); \
            }                                                     \
        }                                                         \
    } while (0)

#define HANDLE_CHECKBOX(label)                                    \
    do {                                                          \
        if (ImGui::Checkbox(GetFnChar(FnLabel::k##label),         \
                            &ckbox_##label##_)) {                 \
            if (ckbox_cbs_.contains(FnLabel::k##label)) {         \
                ckbox_cbs_[FnLabel::k##label](ckbox_##label##_);  \
            } else {                                              \
                LOG(WARN, "Not found handler for label="#label); \
            }                                                     \
        }                                                         \
    } while (0)

void ImGuiContext::RenderClientArea() {
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("Features", nullptr,
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text(GetFnChar(FnLabel::kMoney));
    ImGui::SameLine();  // Keep the following item on the same line
    ImGui::SetNextItemWidth(100);
    static char input[128] = "";
    if (ImGui::InputText("##input", input, IM_ARRAYSIZE(input),
                         ImGuiInputTextFlags_CharsDecimal |
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
    }
    ImGui::SameLine();

    HANDLE_BUTTON(Apply);
    HANDLE_BUTTON(FastBuild);
    HANDLE_BUTTON(DeleteUnit);
    HANDLE_BUTTON(ClearShroud);
    HANDLE_BUTTON(GiveMeABomb);
    HANDLE_BUTTON(UnitLevelUp);
    HANDLE_BUTTON(UnitSpeedUp);
    HANDLE_BUTTON(IAMWinner);
    HANDLE_BUTTON(ThisIsMine);
    HANDLE_BUTTON(IAMGhost);

    HANDLE_CHECKBOX(God);
    HANDLE_CHECKBOX(InstBuild);
    HANDLE_CHECKBOX(UnlimitSuperWeapon);
    HANDLE_CHECKBOX(UnlimitRadar);
    HANDLE_CHECKBOX(InstShot);
    HANDLE_CHECKBOX(InstTurn);
    HANDLE_CHECKBOX(RangeToYourBase);
    HANDLE_CHECKBOX(FireToYourBase);
    HANDLE_CHECKBOX(FreezeGapGenerator);
    HANDLE_CHECKBOX(FreezeUnit);
    HANDLE_CHECKBOX(SellTheWorld);
    HANDLE_CHECKBOX(UnlimitPower);
    HANDLE_CHECKBOX(BuildEveryWhere);
    HANDLE_CHECKBOX(AutoRepaire);
    HANDLE_CHECKBOX(EnermyRepaireDown);
    HANDLE_CHECKBOX(SocialismTheBest);
    HANDLE_CHECKBOX(MakeAttackedMine);
    HANDLE_CHECKBOX(MakeOccupiedMine);
    HANDLE_CHECKBOX(MakeGarrisonedMine);
    HANDLE_CHECKBOX(InvadeMode);
    HANDLE_CHECKBOX(UnlimitTech);
    HANDLE_CHECKBOX(InstLoad);
    HANDLE_CHECKBOX(UnlimitFirePower);
    HANDLE_CHECKBOX(InstChrono);
    HANDLE_CHECKBOX(SpySpy);
    HANDLE_CHECKBOX(InfantrySlip);
    HANDLE_CHECKBOX(UnitLeveledUp);

    ImGui::End();
}
#undef HANDLE_CHECKBOX

void ImGuiContext::AddCheckboxListener(FnLabel label,
                                       std::function<void(bool)> cb) {
    ckbox_cbs_.emplace(label, std::move(cb));
}

const char* ImGuiContext::GetFnChar(FnLabel label) {
    return (const char*)kZhLabels.at(label);
}

}  // namespace yrtr