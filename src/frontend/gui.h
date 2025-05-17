#pragma once
#include <functional>
#include <unordered_map>

#include "base/macro.h"
#include "frontend/char_table.h"
#include "protocol/model.h"

namespace yrtr {
// MVC -- view.

class Gui {
 public:
  using ButtonHandler = std::function<void()>;
  using InputHandler = std::function<void(uint32_t)>;
  using CheckboxHandler = std::function<void(bool)>;

  Gui(State& state, Lang lang);
  ~Gui();

  void Render();
  // Provides a way to trigger from outside event, e.g., hotkeys. Triggered
  // widgets are represented on gui and would change states, just like which
  // triggered through gui.
  void Trigger(FnLabel label) const;

  void AddButtonListener(FnLabel label, ButtonHandler cb);
  void AddInputListener(FnLabel label, InputHandler cb);
  void AddCheckboxListener(FnLabel label, CheckboxHandler cb);

 private:
  // Be care of race conditions.
  State& state_;
  const Lang lang_;

  std::unordered_map<FnLabel, ButtonHandler> btn_cbs_;
  std::unordered_map<FnLabel, InputHandler> input_cbs_;
  std::unordered_map<FnLabel, CheckboxHandler> ckbox_cbs_;

  void RenderClientArea();
  void RenderTabAssists();
  void RenderTabFilters();

  std::string GetGuiStr(GuiLabel label);
  std::string GetFnStr(FnLabel label);

  DISALLOW_COPY_AND_ASSIGN(Gui);
};

}  // namespace yrtr
