#pragma once
#include "macro.h"
#include "vendor.h"

namespace yrtr {

class ImGuiContext;

namespace win32{
class MemoryAPI;
}

class TrainerTarget {
public:
    static TrainerTarget& instance() { return *trainer_target_.get(); }
    static void Init(std::string_view exe_name);

    bool attached() const { return attached_; }
    void Update();

private:
    static std::unique_ptr<TrainerTarget> trainer_target_;

    std::string exe_name_;
    bool attached_;
    DWORD pid_;
    std::unique_ptr<win32::MemoryAPI> mem_api_;

    TrainerTarget(std::string_view exe_name);
    TrainerTarget(TrainerTarget&&) = delete;
    TrainerTarget& operator=(TrainerTarget&&) = delete;

    DISALLOW_COPY_AND_ASSIGN(TrainerTarget);
};

void InitGUI(ImGuiContext& ctx);

}
