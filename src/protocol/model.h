#pragma once
#include <string>

#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/container/btree_map.h"
#include "absl/container/flat_hash_map.h"
#include "absl/synchronization/mutex.h"
__YRTR_END_THIRD_PARTY_HEADERS

namespace yrtr {
// MVC -- model.

enum class FnLabel {
  kInvalid,
  // Tip
  kState,
  kStateOk,
  kStateIdle,
  kMoney,
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
  kIAMGhost,
  // Checkbox
  kGod,
  kInstBuild,
  kUnlimitSuperWeapon,
  kUnlimitRadar,
  kInstFire,
  kInstTurn,
  kRangeToYourBase,
  kFireToYourBase,
  kFreezeGapGenerator,
  kFreezeUnit,
  kSellTheWorld,
  kUnlimitPower,
  kBuildEveryWhere,
  kAutoRepair,
  kEnermyRevertRepair,
  kSocialismTheBest,
  kMakeAttackedMine,
  kMakeCapturedMine,
  kMakeGarrisonedMine,
  kInvadeMode,
  kUnlimitTech,
  kFastReload,
  kUnlimitFirePower,
  kInstChrono,
  kSpySpy,
  kInfantrySlip,
  kEverythingElited,
  kAdjustGameSpeed,
  kCount,
};

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
