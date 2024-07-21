#pragma once
#include "vendor.h"
#include "vendor.h"
#include "char_table.h"

namespace yrtr {

class GuiContext {
public:
    using ButtonHandler = std::function<bool(GuiContext*)>;
    using InputHandler = std::function<bool(GuiContext*, uint32_t)>;
    using CheckboxHandler = std::function<bool(GuiContext*, bool)>;

    GuiContext(GLFWwindow* window);
    ~GuiContext();

    void set_state(std::string_view state) { state_ = state; }
    void UpdateViewport(GLFWwindow* window, int width, int height);
    void EnableCheckbox(FnLabel label);
    void DisableCheckbox(FnLabel label);
    void DeactivateCheckbox(FnLabel label);

    void Update() {}
    void PreRender();
    void Render() const;

    void RenderClientArea();
    void AddButtonListener(FnLabel label, ButtonHandler cb);
    void AddInputListener(FnLabel label, InputHandler cb);
    void AddCheckboxListener(FnLabel label, CheckboxHandler cb);

private:
    float hdpi_scale_factor_;
    std::string state_;

    struct CheckboxState {
        bool enable = true;
        bool activate = false;
        CheckboxHandler cb = nullptr;
    };

    std::unordered_map<FnLabel, ButtonHandler> btn_cbs_;
    std::unordered_map<FnLabel, InputHandler> input_cbs_;
    std::unordered_map<FnLabel, CheckboxState> ckbox_cbs_;
};

}  // namespace yrtr
