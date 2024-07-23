#pragma once
#include "vendor.h"
#include "char_table.h"

namespace yrtr {

struct CheckboxState {
    bool enable = true;
    bool activate = false;
};
struct State {
    std::string game_state;
    std::unordered_map<FnLabel, CheckboxState> ckbox_states;
};

}  // namespace yrtr
