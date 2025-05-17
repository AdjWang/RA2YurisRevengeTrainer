#pragma once
#include "base/macro.h"

namespace yrtr {

class GuiContext {
 public:
  GuiContext() {}
  virtual ~GuiContext() {}

  virtual void UpdateViewport(int width, int height) = 0;
  virtual void BeginFrame() = 0;
  virtual void EndFrame() = 0;
  virtual void Render() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(GuiContext);
};

}  // namespace yrtr
