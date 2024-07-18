#pragma once
#include "vendor.h"
#include "vendor.h"
#include "char_table.h"

namespace yrtr {

class ImGuiContext {
public:
    ImGuiContext(GLFWwindow* window);
    ~ImGuiContext();

    void set_state(std::string_view state) { state_ = state; }

    void Update() {}
    void PreRender();
    void Render() const;

    void RenderClientArea();
    void AddButtonListener(FnLabel label, std::function<void()> cb);
    void AddCheckboxListener(FnLabel label, std::function<void(bool)> cb);

private:
    float hdpi_scale_factor_;
    std::string state_;

    struct CheckboxState {
        bool activate = false;
        std::function<void(bool)> cb = nullptr;
    };

    std::unordered_map<FnLabel, std::function<void()>> btn_cbs_;
    std::unordered_map<FnLabel, CheckboxState> ckbox_cbs_;

    const char* GetFnChar(FnLabel label);
};

}  // namespace yrtr
