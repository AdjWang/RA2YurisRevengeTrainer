#include "frontend/gui.h"

#include <ranges>
#include <thread>
#include <unordered_map>

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "base/logging.h"
#include "base/thread.h"
#include "imgui.h"

// I'm gonna kick all c_str()s' ass.
namespace ImGui {
template <class... TArgs>
void Format(const std::format_string<TArgs...>& fmt_str, TArgs... args) {
  Text(std::format(fmt_str, std::forward<TArgs>(args)...).c_str());
}

void Text(const std::string& cpp_str) {
  Text(cpp_str.c_str());
}

bool Begin(const std::string& name, bool* p_open = (bool*)0,
           ImGuiWindowFlags flags = 0) {
  return Begin(name.c_str(), p_open, flags);
}

bool BeginTabItem(const std::string& label, bool* p_open = (bool*)0,
                  ImGuiTabItemFlags flags = 0) {
  return BeginTabItem(label.c_str(), p_open, flags);
}

bool Button(const std::string& label, const ImVec2& size = ImVec2(0, 0)) {
  return Button(label.c_str(), size);
}

bool Selectable(const std::string& label, bool selected = false,
                ImGuiSelectableFlags flags = 0,
                const ImVec2& size = ImVec2(0, 0)) {
  return Selectable(label.c_str(), selected, flags, size);
}
}  // namespace ImGui

namespace yrtr {

namespace {
inline std::string_view get_log_header() { return "Gui "; }

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

static void PushStyle() {
  // Customize window style here.
  // ImGui::PushStyleColor(id, col);
  // ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
}

static void PopStyle() {
  // Customize window style here.
  // ImGui::PopStyleColor(1);
  // ImGui::PopStyleVar(1);
}
} // namespace

Gui::Gui(State& state, Lang lang)
    : state_(state),
      lang_(lang) {}

Gui::~Gui() {}

void Gui::Render() {
  DCHECK(yrtr::IsWithinRendererThread());
  PushStyle();
  RenderClientArea();
  PopStyle();
}

#define TRIGGER_INPUT(label, val)                             \
  do {                                                        \
    auto it = input_cbs_.find(FnLabel::k##label);             \
    if (it != input_cbs_.end()) {                             \
      it->second(val);                                        \
      BeepEnable();                                           \
    } else {                                                  \
      HLOG_F(WARNING, "Not found handler for label=" #label); \
    }                                                         \
  } while (0)

#define HANDLE_INPUT(label, val)                                \
  do {                                                          \
    if (ImGui::Button(GetFnStr(FnLabel::k##label))) {           \
      auto it = input_cbs_.find(FnLabel::k##label);             \
      if (it != input_cbs_.end()) {                             \
        it->second(val);                                        \
        BeepEnable();                                           \
      } else {                                                  \
        HLOG_F(WARNING, "Not found handler for label=" #label); \
      }                                                         \
    }                                                           \
  } while (0)

#define HANDLE_BUTTON(label)                                    \
  do {                                                          \
    if (ImGui::Button(GetFnStr(FnLabel::k##label))) {           \
      auto it = btn_cbs_.find(FnLabel::k##label);               \
      if (it != btn_cbs_.end()) {                               \
        it->second();                                           \
        BeepEnable();                                           \
      } else {                                                  \
        HLOG_F(WARNING, "Not found handler for label=" #label); \
      }                                                         \
    }                                                           \
  } while (0)

#define HANDLE_CHECKBOX(label)                                               \
  do {                                                                       \
    if (ckbox_cbs_.contains(FnLabel::k##label)) {                            \
      const CheckboxState& ckbox_state = ckbox_states.at(FnLabel::k##label); \
      bool enable = ckbox_state.enable;                                      \
      bool activate = ckbox_state.activate;                                  \
      ImGui::BeginDisabled(!enable);                                         \
      if (ImGui::Checkbox(GetFnStr(FnLabel::k##label).data(), &activate)) {  \
        ckbox_cbs_[FnLabel::k##label](activate);                             \
        if (activate) {                                                      \
          BeepEnable();                                                      \
        } else {                                                             \
          BeepDisable();                                                     \
        }                                                                    \
      }                                                                      \
      ImGui::EndDisabled();                                                  \
    } else {                                                                 \
      bool activate = false;                                                 \
      if (ImGui::Checkbox(GetFnStr(FnLabel::k##label).data(), &activate)) {  \
        HLOG_F(WARNING, "Not found handler for label=" #label);              \
      }                                                                      \
    }                                                                        \
  } while (0)

void Gui::RenderClientArea() {
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

  ImGui::Begin(GetGuiStr(GuiLabel::kTitle), nullptr,
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_AlwaysAutoResize);
  if (ImGui::BeginTabBar("Assist Tool Tabs")) {
    if (ImGui::BeginTabItem(GetGuiStr(GuiLabel::kAssist))) {
      RenderTabAssists();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem(GetGuiStr(GuiLabel::kFilter))) {
      RenderTabFilters();
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::End();
}

void Gui::RenderTabAssists() {
  ImGui::Text(GetFnStr(FnLabel::kMoney));
  ImGui::SameLine();  // Keep the following item on the same line
  ImGui::SetNextItemWidth(100);
  static char input[128] = "";
  bool triggered = ImGui::InputText(
      "##input", input, IM_ARRAYSIZE(input),
      ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue);
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

  CheckboxStateMap ckbox_states;
  {
    absl::MutexLock lk(&state_.ckbox_states_mu);
    ckbox_states = state_.ckbox_states;
  }
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
  HANDLE_CHECKBOX(EverythingElited);
  HANDLE_CHECKBOX(AdjustGameSpeed);

}
#undef TRIGGER_INPUT
#undef HANDLE_INPUT
#undef HANDLE_BUTTON
#undef HANDLE_CHECKBOX

void Gui::RenderTabFilters() {
  float client_width =
      ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x * 2;
  // Read, modify and update.
  SideMap selecting_houses;
  SideMap protected_houses;
  {
    absl::MutexLock lk(&state_.houses_mu);
    // Readonly.
    selecting_houses = state_.selecting_houses;
    // Read and write.
    protected_houses = state_.protected_houses;
  }
  ImGui::BeginGroup();
  ImGui::Text(GetGuiStr(GuiLabel::kSelectingHouseList));
  ImGui::SameLine();
  if (ImGui::Button(GetGuiStr(GuiLabel::kAddAll))) {
    for (auto& [uniq_id, desc] : selecting_houses) {
      protected_houses.try_emplace(uniq_id, desc);
    }
  }
  ImGui::BeginChild("Source", ImVec2(client_width / 2, 0), true,
                    ImGuiWindowFlags_None);
  if (ImGui::BeginListBox("##ListBox", ImVec2(-FLT_MIN, -FLT_MIN))) {
    for (auto& [uniq_id, desc] : selecting_houses) {
      if (ImGui::Selectable(desc.item_name())) {
        protected_houses.try_emplace(uniq_id, desc);
      }
    }
    ImGui::EndListBox();
  }
  ImGui::EndChild();
  ImGui::EndGroup();

  ImGui::SameLine();

  // Right panel: Destination ListBox
  ImGui::BeginGroup();
  ImGui::Text(GetGuiStr(GuiLabel::kProtectedHouseList));
  ImGui::SameLine();
  if (ImGui::Button(GetGuiStr(GuiLabel::kClearAll))) {
    protected_houses.clear();
  }

  ImGui::BeginChild("Destination", ImVec2(client_width / 2, 0), true,
                    ImGuiWindowFlags_None);
  if (ImGui::BeginListBox("##ListBox", ImVec2(-FLT_MIN, -FLT_MIN))) {
    absl::InlinedVector<SideDesc, 10> erased_houses;
    for (const SideDesc& desc : std::views::values(protected_houses)) {
      if (ImGui::Selectable(desc.item_name())) {
        erased_houses.emplace_back(desc);
      }
    }
    for (const SideDesc& desc : erased_houses) {
      protected_houses.erase(desc.uniq_id);
    }
    ImGui::EndListBox();
  }

  ImGui::EndChild();
  ImGui::EndGroup();
  {
    absl::MutexLock lk(&state_.houses_mu);
    // Do not write back selecting_houses, it's readonly here.
    state_.protected_houses = protected_houses;
  }
}

void Gui::Trigger(FnLabel label) const {
  DCHECK(yrtr::IsWithinRendererThread());
  if (label == FnLabel::kInvalid) {
    return;
  }
  NOT_IMPLEMENTED();
}

void Gui::AddButtonListener(FnLabel label, ButtonHandler cb) {
  btn_cbs_.emplace(label, std::move(cb));
}

void Gui::AddInputListener(FnLabel label, InputHandler cb) {
  input_cbs_.emplace(label, std::move(cb));
}

void Gui::AddCheckboxListener(FnLabel label, CheckboxHandler cb) {
  ckbox_cbs_.emplace(label, std::move(cb));
}

std::string Gui::GetGuiStr(GuiLabel label) {
  const char8_t* gui_str = yrtr::GetGuiStr(label, lang_);
  return std::string(reinterpret_cast<const char*>(gui_str));
}

std::string Gui::GetFnStr(FnLabel label) {
  const char8_t* fn_str = yrtr::GetFnStr(label, lang_);
  return std::string(reinterpret_cast<const char*>(fn_str));
}

}  // namespace yrtr
