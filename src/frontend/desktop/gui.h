#pragma once
#include <functional>
#include <map>

#include "base/macro.h"
#include "frontend/desktop/char_table.h"
#include "protocol/model.h"

namespace yrtr {
namespace frontend {
// MVC -- view.

class Gui {
 public:
  using InputHandler = std::function<void(uint32_t)>;
  using ButtonHandler = std::function<void()>;
  using CheckboxHandler = std::function<void(bool)>;
  using HouseListHandler = std::function<void(SideMap&&)>;

  Gui(Lang lang);
  ~Gui();

  void UpdateState(const State& state);
  void Render();
  // Provides a way to trigger from outside event, e.g., hotkeys. Triggered
  // widgets are represented on gui and would change states, just like which
  // triggered through gui.
  void Trigger(FnLabel label) const;

  void AddButtonListener(FnLabel label, ButtonHandler cb);
  void AddInputListener(FnLabel label, InputHandler cb);
  void AddCheckboxListener(FnLabel label, CheckboxHandler cb);
  void AddHouseListListener(HouseListHandler cb);

 private:
  State state_;
  const Lang lang_;

  std::map<FnLabel, InputHandler> input_cbs_;
  std::map<FnLabel, ButtonHandler> btn_cbs_;
  std::map<FnLabel, CheckboxHandler> ckbox_cbs_;
  HouseListHandler house_list_cb_;

  void RenderClientArea();
  void RenderTabAssists();
  void RenderTabFilters();

  std::string GetGuiStr(GuiLabel label);
  std::string GetFnStr(FnLabel label);

  DISALLOW_COPY_AND_ASSIGN(Gui);
};

}  // namespace frontend
}  // namespace yrtr
