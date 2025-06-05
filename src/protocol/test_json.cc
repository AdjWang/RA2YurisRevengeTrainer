#include "base/logging.h"
#include "nlohmann/json.hpp"
#include "protocol/model.h"
using json = nlohmann::json;

namespace yrtr {
static void InitStates(State& state) {
  state.ckbox_states.emplace(FnLabel::kGod,                CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kInstBuild,          CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kUnlimitSuperWeapon, CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kInstFire,           CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kInstTurn,           CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kRangeToYourBase,    CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kFireToYourBase,     CheckboxState{.enable=false, .activate=false});
  state.ckbox_states.emplace(FnLabel::kFreezeGapGenerator, CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kSellTheWorld,       CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kBuildEveryWhere,    CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kAutoRepair,         CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kSocialismMajesty,   CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kMakeGarrisonedMine, CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kInvadeMode,         CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kUnlimitTech,        CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kUnlimitFirePower,   CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kInstChrono,         CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kSpySpy,             CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kAdjustGameSpeed,    CheckboxState{.enable=true, .activate=false});
  state.selecting_houses.emplace(456, SideDesc{456, "456"});
  state.protected_houses.emplace(123, SideDesc{123, "123"});
}
}  // namespace yrtr

int main() {
  yrtr::logging::InitLogging();
  yrtr::State state_send;
  yrtr::InitStates(state_send);

  json data(state_send);
  LOG(INFO) << data.dump();
  LOG_F(INFO, "data length={}", data.dump().size());

  yrtr::State state_recv = json::parse(data.dump());
  json data2(state_recv);
  LOG(INFO) << data2.dump();

  return 0;
}
