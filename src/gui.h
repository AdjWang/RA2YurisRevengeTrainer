#pragma once
#include "macro.h"
#include "vendor.h"
#include "char_table.h"
#include "state.h"

namespace yrtr {

extern void DisableHotkeyGUI(int key);

class GuiContext {
public:
    using ButtonHandler = std::function<void()>;
    using InputHandler = std::function<void(uint32_t)>;
    using CheckboxHandler = std::function<void(bool)>;

    GuiContext(GLFWwindow* window, const State& state);
    ~GuiContext();

    void UpdateViewport(GLFWwindow* window, int width, int height);

    void Update() {}
    void PreRender();
    void Render() const;

    void RenderClientArea();

    void Trigger(FnLabel label);

    void AddButtonListener(FnLabel label, ButtonHandler cb);
    void AddInputListener(FnLabel label, InputHandler cb);
    void AddCheckboxListener(FnLabel label, CheckboxHandler cb);

private:
    const State& state_;
    float hdpi_scale_factor_;

    std::unordered_map<FnLabel, ButtonHandler> btn_cbs_;
    std::unordered_map<FnLabel, InputHandler> input_cbs_;
    std::unordered_map<FnLabel, CheckboxHandler> ckbox_cbs_;

    DISALLOW_COPY_AND_ASSIGN(GuiContext);
};

}  // namespace yrtr
