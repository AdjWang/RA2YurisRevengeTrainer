#pragma once
#include <functional>
#include <memory>

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "frontend/desktop/glfw.h"
#include "base/logging.h"
#include "base/macro.h"
#include "frontend/desktop/context.h"

namespace yrtr {
namespace frontend {

#define __YRTR_WINDOW_TYPE GLFWwindow*

class ImGuiWindow : public GuiContext {
 public:
  ImGuiWindow(__YRTR_WINDOW_TYPE window);
  virtual ~ImGuiWindow();

  void UpdateViewport(int width, int height) override;
  void BeginFrame() override;
  void EndFrame() override;
  void Render() override;

 private:
  __YRTR_WINDOW_TYPE window_;
  float hdpi_scale_factor_;

  DISALLOW_COPY_AND_ASSIGN(ImGuiWindow);
};

}  // namespace frontend
}  // namespace yrtr
