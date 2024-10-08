#include "config.h"
#include "logging.h"
#include "gui.h"
#include "char_table.h"

namespace ImGui {
template <class... TArgs>
void Format(const fmt::format_string<TArgs...>& fmt_str, TArgs... args) {
    Text(fmt::format(fmt_str, std::forward<TArgs>(args)...).c_str());
}
}  // namespace ImGui

namespace yrtr {

static std::unordered_set<int> kDisabledHotkey;
void DisableHotkeyGUI(int key) {
    kDisabledHotkey.emplace(key);
}

const ImWchar* GetGlyphRanges(ImFontAtlas* font_atlas) {
    if (config::GetGlobalConfig().lang == Lang::kZh) {
        static const ImWchar ranges[] = {
            0x0020, 0x0020,
            0x0028, 0x0029,
            0x002C, 0x002E,
            0x0030, 0x0030,
            0x0030, 0x0031,
            0x0031, 0x0032,
            0x0032, 0x0033,
            0x0033, 0x0034,
            0x0034, 0x0035,
            0x0035, 0x0036,
            0x0036, 0x0037,
            0x0037, 0x0038,
            0x0038, 0x0039,
            0x0039, 0x003A,
            0x0041, 0x005A,
            0x006B, 0x006B,
            0x006E, 0x006F,
            0x0075, 0x0075,
            0x0077, 0x0077,
            0x4E07, 0x4E07,
            0x4E09, 0x4E09,
            0x4E2D, 0x4E2D,
            0x4E3A, 0x4E3B,
            0x4E45, 0x4E45,
            0x4E49, 0x4E49,
            0x4EA7, 0x4EA7,
            0x4EFB, 0x4EFB,
            0x4F1A, 0x4F1A,
            0x4F4D, 0x4F4D,
            0x4FB5, 0x4FB5,
            0x4FEE, 0x4FEE,
            0x5168, 0x5168,
            0x519B, 0x519B,
            0x51FB, 0x51FB,
            0x5220, 0x5220,
            0x5229, 0x5229,
            0x5230, 0x5230,
            0x529B, 0x529B,
            0x52A0, 0x52A1,
            0x52A8, 0x52A8,
            0x5347, 0x5347,
            0x5355, 0x5356,
            0x5360, 0x5360,
            0x5373, 0x5373,
            0x5668, 0x5668,
            0x56FE, 0x56FE,
            0x5730, 0x5730,
            0x5927, 0x5927,
            0x5BB6, 0x5BB6,
            0x5C11, 0x5C11,
            0x5C4B, 0x5C4B,
            0x5C81, 0x5C81,
            0x5E7D, 0x5E7D,
            0x5EFA, 0x5EFA,
            0x5F00, 0x5F00,
            0x5F0F, 0x5F0F,
            0x5F39, 0x5F39,
            0x5FEB, 0x5FEB,
            0x6001, 0x6001,
            0x610F, 0x610F,
            0x620F, 0x6212,
            0x623F, 0x623F,
            0x6280, 0x6280,
            0x6539, 0x6539,
            0x653B, 0x653B,
            0x6548, 0x6548,
            0x654C, 0x654C,
            0x65B0, 0x65B0,
            0x65B9, 0x65B9,
            0x65E0, 0x65E0,
            0x65F6, 0x65F6,
            0x662F, 0x662F,
            0x672A, 0x672A,
            0x677F, 0x677F,
            0x6781, 0x6781,
            0x6838, 0x6838,
            0x68C0, 0x68C0,
            0x6A21, 0x6A21,
            0x6B66, 0x6B66,
            0x6C38, 0x6C38,
            0x6D4B, 0x6D4B,
            0x6E38, 0x6E38,
            0x6ED1, 0x6ED1,
            0x7075, 0x7075,
            0x72B6, 0x72B6,
            0x73A9, 0x73A9,
            0x7406, 0x7406,
            0x751F, 0x751F,
            0x7535, 0x7535,
            0x7565, 0x7565,
            0x75EA, 0x75EA,
            0x762B, 0x762B,
            0x7684, 0x7684,
            0x77AC, 0x77AC,
            0x793E, 0x793E,
            0x79D1, 0x79D1,
            0x7A0B, 0x7A0B,
            0x7A7A, 0x7A7A,
            0x7ACB, 0x7ACB,
            0x7B51, 0x7B51,
            0x7EA7, 0x7EA7,
            0x7F1D, 0x7F1D,
            0x80DC, 0x80DC,
            0x81EA, 0x81EA,
            0x836F, 0x836F,
            0x8840, 0x8840,
            0x884C, 0x884C,
            0x88C2, 0x88C2,
            0x88C5, 0x88C5,
            0x8B66, 0x8B66,
            0x8C03, 0x8C03,
            0x8D85, 0x8D85,
            0x8D8A, 0x8D8A,
            0x8EAB, 0x8EAB,
            0x8F6C, 0x8F6C,
            0x8FBE, 0x8FBE,
            0x8FD0, 0x8FD0,
            0x8FD9, 0x8FD9,
            0x8FDC, 0x8FDC,
            0x901F, 0x9020,
            0x9053, 0x9053,
            0x90E8, 0x90E8,
            0x91CD, 0x91CD,
            0x91CF, 0x91CF,
            0x94B1, 0x94B1,
            0x95F4, 0x95F4,
            0x961F, 0x961F,
            0x9650, 0x9650,
            0x9664, 0x9664,
            0x968F, 0x968F,
            0x96F7, 0x96F7,
            0x978B, 0x978B,
            0x9886, 0x9886,
            0x9A7B, 0x9A7B,
            0,
        };
        return &ranges[0];
    } else if (config::GetGlobalConfig().lang == Lang::kEn) {
        return font_atlas->GetGlyphRangesDefault();
    } else {
        UNREACHABLE();
    }
}

namespace {
static std::string GetFnStrWithKey(FnLabel label) {
    int hot_key = config::GetHotkey(label);
    if (kDisabledHotkey.contains(hot_key) || hot_key == GLFW_KEY_UNKNOWN) {
        return fmt::format("{}()", ((const char *)GetFnStr(
                                       label, config::GetGlobalConfig().lang)));
    }
    const char8_t *fn_str = GetFnStr(label, config::GetGlobalConfig().lang);
    const char8_t* key_str = config::KeyToString(hot_key);
    return fmt::format("{}({})", (const char*)fn_str, (const char*)key_str);
}

// https://github.com/ocornut/imgui/issues/707#issuecomment-254610737
static void SetupStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(0.31f, 0.25f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.74f, 0.74f, 0.94f, 1.00f);
    // style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.68f, 0.68f, 0.68f, 0.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.62f, 0.70f, 0.72f, 0.56f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.33f, 0.14f, 0.47f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.97f, 0.31f, 0.13f, 0.81f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.42f, 0.75f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.65f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.62f, 0.80f, 0.15f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.64f, 0.80f, 0.30f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.67f, 0.80f, 0.59f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.48f, 0.53f, 0.67f);
    // style.Colors[ImGuiCol_ComboBg] = ImVec4(0.89f, 0.98f, 1.00f, 0.99f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.48f, 0.47f, 0.47f, 0.71f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.47f, 0.99f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.79f, 0.18f, 0.78f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.42f, 0.82f, 1.00f, 0.81f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.72f, 1.00f, 1.00f, 0.86f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.65f, 0.78f, 0.84f, 0.80f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.88f, 0.94f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.68f, 0.74f, 0.80f);//ImVec4(0.46f, 0.84f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.80f, 0.30f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    // style.Colors[ImGuiCol_CloseButton] = ImVec4(0.41f, 0.75f, 0.98f, 0.50f);
    // style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 0.47f, 0.41f, 0.60f);
    // style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 0.16f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.99f, 0.54f, 0.43f);
    // style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.82f, 0.92f, 1.00f, 0.90f);
    style.Alpha = 1.0f;
    // style.WindowFillAlphaDefault = 1.0f;
    style.FrameRounding = 4;
    style.IndentSpacing = 12.0f;
}

static void BeepEnable() {
    std::thread t([]() {
        Beep(600, 100);
        Beep(1000, 100);
    });
    t.detach();
}

static void BeepDisable() {
    std::thread t([]() {
        Beep(1000, 100);
        Beep(600, 100);
    });
    t.detach();
}
}  // namespace

GuiContext::GuiContext(GLFWwindow* window, const State& state)
    : state_(state) {
    // setup high dpi scale factor
    ImGui_ImplWin32_EnableDpiAwareness();
    hdpi_scale_factor_ =
        ImGui_ImplWin32_GetDpiScaleForMonitor(MonitorFromWindow(
            glfwGetWin32Window(window), MONITOR_DEFAULTTONEAREST));
    LOG(INFO, "High dpi scale factor={:.2f}", hdpi_scale_factor_);
    // setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    // Load fonts
    std::filesystem::path font_path(config::GetGlobalConfig().font_path);
    if (!std::filesystem::exists(font_path)) {
        LOG(FATAL, "Not found font={}", font_path.string());
    }
    io.Fonts->Clear();
    ImFont* font = io.Fonts->AddFontFromFileTTF(
        font_path.string().c_str(),
        config::kFontScale,
        nullptr,
        GetGlyphRanges(io.Fonts));
    CHECK(font != nullptr) << std::format("Failed to add font from={}",
                                          font_path.string());
    io.Fonts->Build();
    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();
    // ImGui::StyleColorsClassic();
    SetupStyle();

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

void GuiContext::UpdateViewport(GLFWwindow* window, int width, int height) {
    UNREFERENCED_PARAMETER(width);
    UNREFERENCED_PARAMETER(height);
    hdpi_scale_factor_ =
        ImGui_ImplWin32_GetDpiScaleForMonitor(MonitorFromWindow(
            glfwGetWin32Window(window), MONITOR_DEFAULTTONEAREST));
    LOG(INFO, "High dpi scale factor={:.2f}", hdpi_scale_factor_);
}

#define TRIGGER_INPUT(label, val)                             \
    do {                                                      \
        if (input_cbs_.contains(FnLabel::k##label)) {         \
            input_cbs_[FnLabel::k##label](val);               \
            BeepEnable();                                     \
        } else {                                              \
            LOG(WARN, "Not found handler for label=" #label); \
        }                                                     \
    } while (0)

#define HANDLE_INPUT(label, val)                                            \
    do {                                                                    \
        if (input_cbs_.contains(FnLabel::k##label)) {                       \
            if (ImGui::Button(GetFnStrWithKey(FnLabel::k##label).data())) { \
                input_cbs_[FnLabel::k##label](val);                         \
                BeepEnable();                                               \
            }                                                               \
        } else {                                                            \
            LOG(WARN, "Not found handler for label=" #label);               \
        }                                                                   \
    } while (0)

#define HANDLE_BUTTON(label)                                                \
    do {                                                                    \
        if (btn_cbs_.contains(FnLabel::k##label)) {                         \
            if (ImGui::Button(GetFnStrWithKey(FnLabel::k##label).data())) { \
                btn_cbs_[FnLabel::k##label]();                              \
                BeepEnable();                                               \
            }                                                               \
        } else {                                                            \
            LOG(WARN, "Not found handler for label=" #label);               \
        }                                                                   \
    } while (0)

#define HANDLE_CHECKBOX(label)                                             \
    do {                                                                   \
        if (ckbox_cbs_.contains(FnLabel::k##label)) {                      \
            const CheckboxState& ckbox_state =                             \
                state_.ckbox_states.at(FnLabel::k##label);                 \
            bool enable = ckbox_state.enable;                              \
            bool activate = ckbox_state.activate;                          \
            ImGui::BeginDisabled(!enable);                                 \
            if (ImGui::Checkbox(GetFnStrWithKey(FnLabel::k##label).data(), \
                                &activate)) {                              \
                ckbox_cbs_[FnLabel::k##label](activate);                   \
                if (activate) {                                            \
                    BeepEnable();                                          \
                } else {                                                   \
                    BeepDisable();                                         \
                }                                                          \
            }                                                              \
            ImGui::EndDisabled();                                          \
        }                                                                  \
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
    ImGui::SetWindowFontScale(hdpi_scale_factor_);

    ImGui::Format(
        "{}: {}",
        (const char *)GetFnStr(FnLabel::kState, config::GetGlobalConfig().lang),
        state_.game_state);
    ImGui::Text((const char *)GetFnStr(FnLabel::kMoney,
                                       config::GetGlobalConfig().lang));
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

#define HANDLE_TRIGGER_BUTTON(label_name)      \
    do {                                       \
        if (label == FnLabel::k##label_name) { \
            if (btn_cbs_.contains(label)) {    \
                btn_cbs_[label]();             \
                BeepEnable();                  \
            }                                  \
            return;                            \
        }                                      \
    } while (0)

#define HANDLE_TRIGGER_CHECKBOX(label_name)           \
    do {                                              \
        if (label == FnLabel::k##label_name) {        \
            if (ckbox_cbs_.contains(label)) {         \
                const CheckboxState& ckbox_state =    \
                    state_.ckbox_states.at(label);    \
                bool enable = ckbox_state.enable;     \
                bool activate = ckbox_state.activate; \
                if (enable) {                         \
                    ckbox_cbs_[label](!activate);     \
                    if (!activate) {                  \
                        BeepEnable();                 \
                    } else {                          \
                        BeepDisable();                \
                    }                                 \
                }                                     \
            }                                         \
            return;                                   \
        }                                             \
    } while (0)

void GuiContext::Trigger(FnLabel label) {
    if (label == FnLabel::kInvalid) {
        return;
    }
    if (label == FnLabel::kApply) {
        if (input_cbs_.contains(label)) {
            // Set credit
            input_cbs_[label](config::kApplyCredit);
            BeepEnable();
        }
    }
    HANDLE_TRIGGER_BUTTON(FastBuild);
    HANDLE_TRIGGER_BUTTON(DeleteUnit);
    HANDLE_TRIGGER_BUTTON(ClearShroud);
    HANDLE_TRIGGER_BUTTON(GiveMeABomb);
    HANDLE_TRIGGER_BUTTON(UnitLevelUp);
    HANDLE_TRIGGER_BUTTON(UnitSpeedUp);
    HANDLE_TRIGGER_BUTTON(IAMWinner);
    HANDLE_TRIGGER_BUTTON(ThisIsMine);
    HANDLE_TRIGGER_BUTTON(IAMGhost);

    HANDLE_TRIGGER_CHECKBOX(God);
    HANDLE_TRIGGER_CHECKBOX(InstBuild);
    HANDLE_TRIGGER_CHECKBOX(UnlimitSuperWeapon);
    HANDLE_TRIGGER_CHECKBOX(UnlimitRadar);
    HANDLE_TRIGGER_CHECKBOX(InstFire);
    HANDLE_TRIGGER_CHECKBOX(InstTurn);
    HANDLE_TRIGGER_CHECKBOX(RangeToYourBase);
    HANDLE_TRIGGER_CHECKBOX(FireToYourBase);
    HANDLE_TRIGGER_CHECKBOX(FreezeGapGenerator);
    HANDLE_TRIGGER_CHECKBOX(FreezeUnit);
    HANDLE_TRIGGER_CHECKBOX(SellTheWorld);
    HANDLE_TRIGGER_CHECKBOX(UnlimitPower);
    HANDLE_TRIGGER_CHECKBOX(BuildEveryWhere);
    HANDLE_TRIGGER_CHECKBOX(AutoRepair);
    HANDLE_TRIGGER_CHECKBOX(EnermyRevertRepair);
    HANDLE_TRIGGER_CHECKBOX(SocialismTheBest);
    HANDLE_TRIGGER_CHECKBOX(MakeAttackedMine);
    HANDLE_TRIGGER_CHECKBOX(MakeCapturedMine);
    HANDLE_TRIGGER_CHECKBOX(MakeGarrisonedMine);
    HANDLE_TRIGGER_CHECKBOX(InvadeMode);
    HANDLE_TRIGGER_CHECKBOX(UnlimitTech);
    HANDLE_TRIGGER_CHECKBOX(FastReload);
    HANDLE_TRIGGER_CHECKBOX(UnlimitFirePower);
    HANDLE_TRIGGER_CHECKBOX(InstChrono);
    HANDLE_TRIGGER_CHECKBOX(SpySpy);
    HANDLE_TRIGGER_CHECKBOX(InfantrySlip);
    HANDLE_TRIGGER_CHECKBOX(UnitLeveledUp);
    HANDLE_TRIGGER_CHECKBOX(AdjustGameSpeed);
}
#undef HANDLE_HOTKEY_BUTTON
#undef HANDLE_HOTKEY_CHECKBOX

void GuiContext::AddButtonListener(FnLabel label, ButtonHandler cb) {
    btn_cbs_.emplace(label, std::move(cb));
}

void GuiContext::AddInputListener(FnLabel label, InputHandler cb) {
    input_cbs_.emplace(label, std::move(cb));
}

void GuiContext::AddCheckboxListener(FnLabel label, CheckboxHandler cb) {
    ckbox_cbs_.emplace(label, std::move(cb));
}

}  // namespace yrtr
