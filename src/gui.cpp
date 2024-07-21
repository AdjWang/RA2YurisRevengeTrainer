#include "logging.h"
#include "win32utils.h"
#include "gui.h"
#include "char_table.h"

namespace ImGui {
template <class... TArgs>
void Format(const fmt::format_string<TArgs...>& fmt_str, TArgs... args) {
    Text(fmt::format(fmt_str, std::forward<TArgs>(args)...).c_str());
}
}

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

GuiContext::GuiContext(GLFWwindow* window) {
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
    // TODO: make a config
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

GuiContext::~GuiContext() {
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();
}

void GuiContext::PreRender() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GuiContext::Render() const {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GuiContext::EnableCheckbox(FnLabel label) {
    if (ckbox_cbs_.contains(label)) {
        ckbox_cbs_[label].enable = true;
    }
}

void GuiContext::DisableCheckbox(FnLabel label) {
    if (ckbox_cbs_.contains(label)) {
        CheckboxState& ckbox_state = ckbox_cbs_[label];
        if (ckbox_state.activate) {
            ckbox_state.activate = false;
            ckbox_state.cb(this, false);
        }
        ckbox_state.enable = false;
    }
}

void GuiContext::DeactivateCheckbox(FnLabel label) {
    if (ckbox_cbs_.contains(label)) {
        ckbox_cbs_[label].activate = false;
    }
}

#define TRIGGER_INPUT(label, val)                             \
    do {                                                      \
        if (input_cbs_.contains(FnLabel::k##label)) {         \
            input_cbs_[FnLabel::k##label](this, val);         \
        } else {                                              \
            LOG(WARN, "Not found handler for label=" #label); \
        }                                                     \
    } while (0)

#define HANDLE_INPUT(label, val)                               \
    do {                                                       \
        if (input_cbs_.contains(FnLabel::k##label)) {          \
            if (ImGui::Button(GetFnChar(FnLabel::k##label))) { \
                input_cbs_[FnLabel::k##label](this, val);      \
            }                                                  \
        } else {                                               \
            LOG(WARN, "Not found handler for label=" #label);  \
        }                                                      \
    } while (0)

#define HANDLE_BUTTON(label)                                   \
    do {                                                       \
        if (btn_cbs_.contains(FnLabel::k##label)) {            \
            if (ImGui::Button(GetFnChar(FnLabel::k##label))) { \
                btn_cbs_[FnLabel::k##label](this);             \
            }                                                  \
        } else {                                               \
            LOG(WARN, "Not found handler for label=" #label);  \
        }                                                      \
    } while (0)

#define HANDLE_CHECKBOX(label)                                          \
    do {                                                                \
        if (ckbox_cbs_.contains(FnLabel::k##label)) {                   \
            CheckboxState& ckbox_state = ckbox_cbs_[FnLabel::k##label]; \
            ImGui::BeginDisabled(!ckbox_state.enable);                  \
            if (ImGui::Checkbox(GetFnChar(FnLabel::k##label),           \
                                &ckbox_state.activate)) {               \
                if (!ckbox_state.cb(this, ckbox_state.activate)) {      \
                    ckbox_state.activate = false;                       \
                }                                                       \
            }                                                           \
            ImGui::EndDisabled();                                       \
        }                                                               \
    } while (0)

void GuiContext::RenderClientArea() {
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("Features", nullptr,
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Format("{}: {}", GetFnChar(FnLabel::kState), state_);
    ImGui::Text(GetFnChar(FnLabel::kMoney));
    ImGui::SameLine();  // Keep the following item on the same line
    ImGui::SetNextItemWidth(100);
    static char input[128] = "";
    bool triggered = ImGui::InputText("##input", input, IM_ARRAYSIZE(input),
                                      ImGuiInputTextFlags_CharsDecimal |
                                      ImGuiInputTextFlags_EnterReturnsTrue);
    char* end = nullptr;
    uint32_t input_val = std::strtoul(input, &end, 10);
    ImGui::SameLine();
    HANDLE_INPUT(Apply, input_val);
    if (triggered) {
        TRIGGER_INPUT(Apply, input_val);
    }

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
    HANDLE_CHECKBOX(InstFire);
    HANDLE_CHECKBOX(InstTurn);
    HANDLE_CHECKBOX(RangeToYourBase);
    HANDLE_CHECKBOX(FireToYourBase);
    HANDLE_CHECKBOX(FreezeGapGenerator);
    HANDLE_CHECKBOX(FreezeUnit);
    HANDLE_CHECKBOX(SellTheWorld);
    HANDLE_CHECKBOX(UnlimitPower);
    HANDLE_CHECKBOX(BuildEveryWhere);
    HANDLE_CHECKBOX(AutoRepair);
    HANDLE_CHECKBOX(EnermyRevertRepair);
    HANDLE_CHECKBOX(SocialismTheBest);
    HANDLE_CHECKBOX(MakeAttackedMine);
    HANDLE_CHECKBOX(MakeCapturedMine);
    HANDLE_CHECKBOX(MakeGarrisonedMine);
    HANDLE_CHECKBOX(InvadeMode);
    HANDLE_CHECKBOX(UnlimitTech);
    HANDLE_CHECKBOX(FastReload);
    HANDLE_CHECKBOX(UnlimitFirePower);
    HANDLE_CHECKBOX(InstChrono);
    HANDLE_CHECKBOX(SpySpy);
    HANDLE_CHECKBOX(InfantrySlip);
    HANDLE_CHECKBOX(UnitLeveledUp);
    HANDLE_CHECKBOX(AdjustGameSpeed);

    ImGui::End();
}
#undef TRIGGER_INPUT
#undef HANDLE_INPUT
#undef HANDLE_BUTTON
#undef HANDLE_CHECKBOX

void GuiContext::AddButtonListener(FnLabel label, ButtonHandler cb) {
    btn_cbs_.emplace(label, std::move(cb));
}

void GuiContext::AddInputListener(FnLabel label, InputHandler cb) {
    input_cbs_.emplace(label, std::move(cb));
}

void GuiContext::AddCheckboxListener(FnLabel label, CheckboxHandler cb) {
    ckbox_cbs_.emplace(label, CheckboxState {
        .activate = false,
        .cb = std::move(cb),
    });
}

}  // namespace yrtr
