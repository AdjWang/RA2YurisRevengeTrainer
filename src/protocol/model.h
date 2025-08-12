#pragma once
#include <map>
#include <string>
#include <unordered_map>

#include "base/logging.h"
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
  kIAMWinner,
  kDeleteUnit,
  kClearShroud,
  kGiveMeABomb,
  kUnitLevelUp,
  kUnitSpeedUp,
  kFastBuild,
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
  kSocialismMajesty,
  kMakeCapturedMine,
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
    case FnLabel::kIAMWinner:           return "IAMWinner";
    case FnLabel::kDeleteUnit:          return "DeleteUnit";
    case FnLabel::kClearShroud:         return "ClearShroud";
    case FnLabel::kGiveMeABomb:         return "GiveMeABomb";
    case FnLabel::kUnitLevelUp:         return "UnitLevelUp";
    case FnLabel::kUnitSpeedUp:         return "UnitSpeedUp";
    case FnLabel::kFastBuild:           return "FastBuild";
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
    case FnLabel::kSocialismMajesty:    return "SocialismMajesty";
    case FnLabel::kMakeCapturedMine:    return "MakeCapturedMine";
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

static constexpr FnLabel StrToFnLabel(std::string_view str) {
  // Button
  if (str == "kApply")              return FnLabel::kApply;
  if (str == "kIAMWinner")          return FnLabel::kIAMWinner;
  if (str == "kDeleteUnit")         return FnLabel::kDeleteUnit;
  if (str == "kClearShroud")        return FnLabel::kClearShroud;
  if (str == "kGiveMeABomb")        return FnLabel::kGiveMeABomb;
  if (str == "kUnitLevelUp")        return FnLabel::kUnitLevelUp;
  if (str == "kUnitSpeedUp")        return FnLabel::kUnitSpeedUp;
  if (str == "kFastBuild")          return FnLabel::kFastBuild;
  if (str == "kThisIsMine")         return FnLabel::kThisIsMine;
  // Checkbox
  if (str == "kGod")                return FnLabel::kGod;
  if (str == "kInstBuild")          return FnLabel::kInstBuild;
  if (str == "kUnlimitSuperWeapon") return FnLabel::kUnlimitSuperWeapon;
  if (str == "kInstFire")           return FnLabel::kInstFire;
  if (str == "kInstTurn")           return FnLabel::kInstTurn;
  if (str == "kRangeToYourBase")    return FnLabel::kRangeToYourBase;
  if (str == "kFireToYourBase")     return FnLabel::kFireToYourBase;
  if (str == "kFreezeGapGenerator") return FnLabel::kFreezeGapGenerator;
  if (str == "kSellTheWorld")       return FnLabel::kSellTheWorld;
  if (str == "kBuildEveryWhere")    return FnLabel::kBuildEveryWhere;
  if (str == "kAutoRepair")         return FnLabel::kAutoRepair;
  if (str == "kSocialismMajesty")   return FnLabel::kSocialismMajesty;
  if (str == "kMakeCapturedMine")   return FnLabel::kMakeCapturedMine;
  if (str == "kMakeGarrisonedMine") return FnLabel::kMakeGarrisonedMine;
  if (str == "kInvadeMode")         return FnLabel::kInvadeMode;
  if (str == "kUnlimitTech")        return FnLabel::kUnlimitTech;
  if (str == "kUnlimitFirePower")   return FnLabel::kUnlimitFirePower;
  if (str == "kInstChrono")         return FnLabel::kInstChrono;
  if (str == "kSpySpy")             return FnLabel::kSpySpy;
  if (str == "kAdjustGameSpeed")    return FnLabel::kAdjustGameSpeed;
  return FnLabel::kInvalid;
}

struct CheckboxState {
  bool enable = true;
  bool activate = false;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CheckboxState, enable, activate);
};

using UniqId = uint32_t;

struct SideDesc {
  UniqId uniq_id;
  std::string name;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SideDesc, uniq_id, name);

  std::string item_name() const { return name; }
};

using CheckboxStateMap = std::unordered_map<FnLabel, CheckboxState>;
using SideMap = std::map<UniqId, SideDesc>;

// Only compare keys.
inline bool AreEqual(const SideMap& lhs, const SideMap& rhs) {
  return lhs.size() == rhs.size() &&
         std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                    [](auto a, auto b) { return a.first == b.first; });
}

// Deprecated in websocket.
static constexpr std::string_view kApiGetState = "/state";
// Deprecated in websocket.
static constexpr std::string_view kApiPostEvent = "/event";

// Stored in backend.
struct State {
  // Write by controller, read by view.
  // Export checkbox states to controller to bind them with the game state
  // instead of gui state.
  CheckboxStateMap ckbox_states;
  // "House" emm..., fine, classic westwood naming convention. Use map to drop
  // duplications.
  SideMap selecting_houses;
  // Read by controller, write by view.
  SideMap protected_houses;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(State, ckbox_states, selecting_houses,
                                 protected_houses);
};

template <class T>
struct Event {
  std::string type;
  FnLabel label;
  T val;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Event, type, label, val);
};

inline std::string MakeGetStateEvent() {
  Event<State> event{
      .type = "get_state",
      .label = FnLabel::kInvalid,
      .val = State{},
  };
  return json(event).dump();
}

inline std::string MakeGetStateEvent(State&& state) {
  Event<State> event{
      .type = "get_state",
      .label = FnLabel::kInvalid,
      .val = std::move(state),
  };
  return json(event).dump();
}

inline Event<State> ParseGetStateEvent(const json& data) {
  auto event = data.get<Event<State>>();
  DCHECK_EQ(event.type, "get_state");
  DCHECK_EQ(static_cast<int>(event.label), static_cast<int>(FnLabel::kInvalid));
  return event;
}

inline std::string MakeInputEvent(FnLabel label, uint32_t val) {
  Event<uint32_t> event{
      .type = "input",
      .label = label,
      .val = val,
  };
  return json(event).dump();
}

inline Event<uint32_t> ParseInputEvent(const json& data) {
  auto event = data.get<Event<uint32_t>>();
  DCHECK_EQ(event.type, "input");
  DCHECK_NE(static_cast<int>(event.label), static_cast<int>(FnLabel::kInvalid));
  return event;
}

inline std::string MakeButtonEvent(FnLabel label) {
  Event<int> event{
      .type = "button",
      .label = label,
      .val = -1,
  };
  return json(event).dump();
}

inline Event<int> ParseButtonEvent(const json& data) {
  auto event = data.get<Event<int>>();
  DCHECK_EQ(event.type, "button");
  DCHECK_NE(static_cast<int>(event.label), static_cast<int>(FnLabel::kInvalid));
  return event;
}

inline std::string MakeCheckboxEvent(FnLabel label, bool activate) {
  Event<bool> event{
      .type = "checkbox",
      .label = label,
      .val = activate,
  };
  return json(event).dump();
}

inline Event<bool> ParseCheckboxEvent(const json& data) {
  auto event = data.get<Event<bool>>();
  DCHECK_EQ(event.type, "checkbox");
  DCHECK_NE(static_cast<int>(event.label), static_cast<int>(FnLabel::kInvalid));
  return event;
}

inline std::string MakeProtectedListEvent(SideMap&& side_map) {
  Event<SideMap> event{
      .type = "protected_list",
      .label = FnLabel::kInvalid,
      .val = std::move(side_map),
  };
  return json(event).dump();
}

inline Event<SideMap> ParseProtectedListEvent(const json& data) {
  auto event = data.get<Event<SideMap>>();
  DCHECK_EQ(event.type, "protected_list");
  DCHECK_EQ(static_cast<int>(event.label), static_cast<int>(FnLabel::kInvalid));
  return event;
}

}  // namespace yrtr
