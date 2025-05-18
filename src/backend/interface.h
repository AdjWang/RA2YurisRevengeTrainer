#pragma once

#include "base/macro.h"
#include "base/task_queue.h"
#include "tool/assist/game.h"
#include "tool/assist/gui.h"
#include "tool/assist/model.h"
#include "tool/tool.h"

namespace assist {

class Assist {
 public:
  Assist(const yrtr::ToolConfig& cfg);
  void SetActive(bool active);
  void Update(double /*delta*/);
  void Render(void* ctx);

 private:
  const yrtr::ToolConfig& cfg_;
  State state_;
  GuiContext gui_;
  Trainer game_;
  yrtr::TaskQueue game_loop_ch_;
  yrtr::TaskQueue render_loop_ch_;

  DISALLOW_COPY_AND_ASSIGN(Assist);
};

}  // namespace assist
