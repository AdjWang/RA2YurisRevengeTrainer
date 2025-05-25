#include "frontend/desktop/gui.h"

#include <ranges>
#include <thread>
#include <unordered_map>

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "base/logging.h"
#include "base/thread.h"
#include "frontend/desktop/config.h"
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

bool Checkbox(const std::string& label, bool* v) {
  return Checkbox(label.c_str(), v);
}

bool Selectable(const std::string& label, bool selected = false,
                ImGuiSelectableFlags flags = 0,
                const ImVec2& size = ImVec2(0, 0)) {
  return Selectable(label.c_str(), selected, flags, size);
}
}  // namespace ImGui

namespace yrtr {
namespace frontend {

namespace {
inline std::string_view get_log_header() { return "Gui "; }

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

Gui::Gui(Lang lang) : lang_(lang) {}

Gui::~Gui() {}

void Gui::UpdateState(const State& state) {
  DCHECK(yrtr::IsWithinRendererThread());
  // This should be the only point writing to state.
  state_ = state;
}

void Gui::Render() {
  DCHECK(yrtr::IsWithinRendererThread());
  PushStyle();
  RenderClientArea();
  PopStyle();
}

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
  ImGui::Text(GetGuiStr(GuiLabel::kMoney));
  ImGui::SameLine();  // Keep the following item on the same line
  ImGui::SetNextItemWidth(100);
  static char input[128] = "";
  // Triggered by press enter.
  bool triggered = ImGui::InputText(
      "##input", input, IM_ARRAYSIZE(input),
      ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue);
  char* end = nullptr;
  uint32_t input_val = std::strtoul(input, &end, 10);
  ImGui::SameLine();
  if (ImGui::Button(GetFnStr(FnLabel::kApply)) || triggered) {
    auto it = input_cbs_.find(FnLabel::kApply);
    if (it != input_cbs_.end()) {
      it->second(input_val);
    } else {
      HLOG_F(WARNING, "Not found handler for label=Apply");
    }
  }
  for (auto& [label, handler] : btn_cbs_) {
    if (ImGui::Button(GetFnStr(label))) {
      if (handler != nullptr) {
        handler();
      } else {
        HLOG_F(WARNING, "Not found handler for label={}", StrFnLabel(label));
      }
    }
  }
  const CheckboxStateMap& ckbox_states = state_.ckbox_states;
  for (auto& [label, handler] : ckbox_cbs_) {
    bool enable = true;
    bool activate = false;
    auto it = ckbox_states.find(label);
    if (it != ckbox_states.end()) {
      enable = it->second.enable;
      activate = it->second.activate;
    }
    ImGui::BeginDisabled(!enable);
    if (ImGui::Checkbox(GetFnStr(label), &activate)) {
      if (handler != nullptr) {
        handler(activate);
      } else {
        HLOG_F(WARNING, "Not found handler for label={}", StrFnLabel(label));
      }
    }
    ImGui::EndDisabled();
  }
}

void Gui::RenderTabFilters() {
  float client_width =
      ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x * 2;
  // Readonly.
  const SideMap& selecting_houses = state_.selecting_houses;
  // Read and write.
  SideMap protected_houses = state_.protected_houses;
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
  // Trigger update data if modified any item.
  if (!AreEqual(protected_houses, state_.protected_houses)) {
    if (house_list_cb_ != nullptr) {
      house_list_cb_(std::move(protected_houses));
    } else {
      HLOG_F(WARNING, "Not found handler for house list update");
    }
  }
}

void Gui::Trigger(FnLabel label) const {
  DCHECK(yrtr::IsWithinRendererThread());
  if (label == FnLabel::kInvalid) {
    HLOG_F(WARNING, "Invalid label");
    return;
  }
  // There's have only one input widget now.
  DCHECK_LE(input_cbs_.size(), 1u);
  auto it_input = input_cbs_.find(label);
  if (it_input != input_cbs_.end()) {
    // Set credit.
    it_input->second(233333);
    return;
  }
  auto it_btn = btn_cbs_.find(label);
  if (it_btn != btn_cbs_.end()) {
    it_btn->second();
    return;
  }
  const CheckboxStateMap& ckbox_states = state_.ckbox_states;
  auto it_ckbox_state = ckbox_states.find(label);
  auto it_ckbox = ckbox_cbs_.find(label);
  if (it_ckbox_state != ckbox_states.end() && it_ckbox != ckbox_cbs_.end()) {
    bool enable = it_ckbox_state->second.enable;
    if (enable) {
      bool activate = it_ckbox_state->second.activate;
      it_ckbox->second(!activate);
    }
    return;
  }
  // Report errors.
  if (it_input == input_cbs_.end() && it_btn == btn_cbs_.end() &&
      it_ckbox == ckbox_cbs_.end()) {
    HLOG_F(WARNING, "Not found handler for label={}", StrFnLabel(label));
    return;
  }
  if (it_ckbox_state == ckbox_states.end()) {
    HLOG_F(WARNING, "Not found state for label={}", StrFnLabel(label));
    return;
  }
  UNREACHABLE();
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

void Gui::AddHouseListListener(HouseListHandler cb) {
  house_list_cb_ = std::move(cb);
}

std::string Gui::GetGuiStr(GuiLabel label) {
  const char8_t* gui_str = yrtr::frontend::GetGuiStr(label, lang_);
  return std::string(reinterpret_cast<const char*>(gui_str));
}

std::string Gui::GetFnStr(FnLabel label) {
  Config* config = Config::instance();
  DCHECK_NOTNULL(config);
  return config->GetFnStrWithKey(label);
}

}  // namespace frontend
}  // namespace yrtr
