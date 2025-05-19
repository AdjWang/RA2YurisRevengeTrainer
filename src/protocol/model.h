#pragma once
#include <string>

#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/container/btree_map.h"
#include "absl/container/flat_hash_map.h"
#include "absl/synchronization/mutex.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace yrtr {
// MVC -- model.

enum class FnLabel {
  kInvalid = -1,
  // Button
  kApply,
  kFastBuild,
  kDeleteUnit,
  kClearShroud,
  kGiveMeABomb,
  kUnitLevelUp,
  kUnitSpeedUp,
  kIAMWinner,
  kThisIsMine,
  // Checkbox
  kGod,
  kInstBuild,
  kUnlimitSuperWeapon,
  kInstFire,
  kInstTurn,
  kRangeToYourBase,
  kFireToYourBase,
  kFreezeGapGenerator,
  kSellTheWorld,
  kBuildEveryWhere,
  kAutoRepair,
  // TODO
  kSocialismTheBest,
  kMakeGarrisonedMine,
  kInvadeMode,
  kUnlimitTech,
  kUnlimitFirePower,
  kInstChrono,
  kSpySpy,
  kAdjustGameSpeed,
  kCount,
};

constexpr std::string_view StrFnLabel(FnLabel label) {
  switch (label) {
    case FnLabel::kInvalid:             return "Invalid";
    // Button
    case FnLabel::kApply:               return "Apply";
    case FnLabel::kFastBuild:           return "FastBuild";
    case FnLabel::kDeleteUnit:          return "DeleteUnit";
    case FnLabel::kClearShroud:         return "ClearShroud";
    case FnLabel::kGiveMeABomb:         return "GiveMeABomb";
    case FnLabel::kUnitLevelUp:         return "UnitLevelUp";
    case FnLabel::kUnitSpeedUp:         return "UnitSpeedUp";
    case FnLabel::kIAMWinner:           return "IAMWinner";
    case FnLabel::kThisIsMine:          return "ThisIsMine";
    // Checkbox
    case FnLabel::kGod:                 return "God";
    case FnLabel::kInstBuild:           return "InstBuild";
    case FnLabel::kUnlimitSuperWeapon:  return "UnlimitSuperWeapon";
    case FnLabel::kInstFire:            return "InstFire";
    case FnLabel::kInstTurn:            return "InstTurn";
    case FnLabel::kRangeToYourBase:     return "RangeToYourBase";
    case FnLabel::kFireToYourBase:      return "FireToYourBase";
    case FnLabel::kFreezeGapGenerator:  return "FreezeGapGenerator";
    case FnLabel::kSellTheWorld:        return "SellTheWorld";
    case FnLabel::kBuildEveryWhere:     return "BuildEveryWhere";
    case FnLabel::kAutoRepair:          return "AutoRepair";
    case FnLabel::kSocialismTheBest:    return "SocialismTheBest";
    case FnLabel::kMakeGarrisonedMine:  return "MakeGarrisonedMine";
    case FnLabel::kInvadeMode:          return "InvadeMode";
    case FnLabel::kUnlimitTech:         return "UnlimitTech";
    case FnLabel::kUnlimitFirePower:    return "UnlimitFirePower";
    case FnLabel::kInstChrono:          return "InstChrono";
    case FnLabel::kSpySpy:              return "SpySpy";
    case FnLabel::kAdjustGameSpeed:     return "AdjustGameSpeed";
    case FnLabel::kCount:               return "Count";
    default:                            return "unknown";
  }
}

struct CheckboxState {
  bool enable = true;
  bool activate = false;
};

using UniqId = uint32_t;

struct SideDesc {
  UniqId uniq_id;
  std::string name;

  std::string item_name() const { return name; }
};

using CheckboxStateMap = absl::flat_hash_map<FnLabel, CheckboxState>;
using SideMap = absl::btree_map<UniqId, SideDesc>;

// Stored in backend.
struct State {
  // Write by controller, read by view.
  // Export checkbox states to controller to bind them with the game state
  // instead of gui state.
  absl::Mutex ckbox_states_mu;
  CheckboxStateMap ckbox_states ABSL_GUARDED_BY(ckbox_states_mu);
  // "House" emm..., fine, classic westwood naming convention. Use map to drop
  // duplications.
  absl::Mutex houses_mu;
  SideMap selecting_houses ABSL_GUARDED_BY(houses_mu);

  // Read by controller, write by view.
  SideMap protected_houses ABSL_GUARDED_BY(houses_mu);
};

}  // namespace yrtr
