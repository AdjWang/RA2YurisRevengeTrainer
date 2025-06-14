#pragma once
#include <cstdint>
#include <string_view>

#include "base/logging.h"
#include "base/macro.h"
#include "protocol/model.h"

namespace yrtr {
namespace frontend {

enum class Lang {
  kZh,
  kEn,
};

inline constexpr std::string_view StrLang(Lang lang) {
  switch (lang) {
    case Lang::kZh: return "zh";
    case Lang::kEn: return "en";
    default:        return "unknown";
  }
}

enum class GuiLabel {
  kTitle,
  kState,
  kStateOk,
  kStateIdle,
  kMoney,
  kAssist,
  kFilter,
  kSelectingHouseList,
  kProtectedHouseList,
  kAddAll,
  kClearAll,
  kCount,
};

constexpr const char8_t* GetGuiStrZh(GuiLabel label) {
  switch (label) {
    case GuiLabel::kTitle:              return u8"辅助工具";
    case GuiLabel::kState:              return u8"状态";
    case GuiLabel::kStateOk:            return u8"游戏运行中";
    case GuiLabel::kStateIdle:          return u8"未检测到游戏";
    case GuiLabel::kMoney:              return u8"钱";
    case GuiLabel::kAssist:             return u8"功能";
    case GuiLabel::kFilter:             return u8"过滤";
    case GuiLabel::kSelectingHouseList: return u8"选中阵营";
    case GuiLabel::kProtectedHouseList: return u8"保护阵营";
    case GuiLabel::kAddAll:             return u8"添加全部";
    case GuiLabel::kClearAll:           return u8"删除全部";
    default:                            return u8"unknown";
  }
}

constexpr const char8_t* GetGuiStrEn(GuiLabel label) {
  switch (label) {
    case GuiLabel::kTitle:              return u8"Assist Tool";
    case GuiLabel::kState:              return u8"State";
    case GuiLabel::kStateOk:            return u8"Game running";
    case GuiLabel::kStateIdle:          return u8"Game not running";
    case GuiLabel::kMoney:              return u8"Money";
    case GuiLabel::kAssist:             return u8"Assist";
    case GuiLabel::kFilter:             return u8"Filter";
    case GuiLabel::kSelectingHouseList: return u8"Selecting house";
    case GuiLabel::kProtectedHouseList: return u8"Protected house";
    case GuiLabel::kAddAll:             return u8"Add all";
    case GuiLabel::kClearAll:           return u8"Clear all";
    default:                            return u8"unknown";
  }
}

inline const char8_t* GetGuiStr(GuiLabel label, Lang lang) {
  if (lang == Lang::kZh) {
    return GetGuiStrZh(label);
  } else if (lang == Lang::kEn) {
    return GetGuiStrEn(label);
  } else {
    UNREACHABLE();
  }
}

constexpr const char8_t* GetFnStrZh(FnLabel label) {
  switch (label) {
    case FnLabel::kApply:               return u8"修改";
    case FnLabel::kFastBuild:           return u8"快速建造";
    case FnLabel::kDeleteUnit:          return u8"删除单位";
    case FnLabel::kClearShroud:         return u8"地图全开";
    case FnLabel::kGiveMeABomb:         return u8"核弹攻击";
    case FnLabel::kUnitLevelUp:         return u8"单位升级";
    case FnLabel::kUnitSpeedUp:         return u8"单位加速";
    case FnLabel::kIAMWinner:           return u8"立即胜利";
    case FnLabel::kThisIsMine:          return u8"这是我的";
    case FnLabel::kGod:                 return u8"无敌";
    case FnLabel::kInstBuild:           return u8"瞬间建造";
    case FnLabel::kUnlimitSuperWeapon:  return u8"无限超武";
    case FnLabel::kInstFire:            return u8"极速攻击";
    case FnLabel::kInstTurn:            return u8"极速转身";
    case FnLabel::kRangeToYourBase:     return u8"远程攻击";
    case FnLabel::kFireToYourBase:      return u8"远程警戒";
    case FnLabel::kFreezeGapGenerator:  return u8"瘫痪裂缝产生器";
    case FnLabel::kSellTheWorld:        return u8"卖卖卖";
    case FnLabel::kBuildEveryWhere:     return u8"随意建筑";
    case FnLabel::kAutoRepair:          return u8"自动修理";
    case FnLabel::kSocialismMajesty:    return u8"社会主义万岁";
    case FnLabel::kMakeCapturedMine:    return u8"全是我的-工程师占领";
    case FnLabel::kMakeGarrisonedMine:  return u8"全是我的-房屋驻军";
    case FnLabel::kInvadeMode:          return u8"侵略模式";
    case FnLabel::kUnlimitTech:         return u8"全科技";
    case FnLabel::kUnlimitFirePower:    return u8"大量弹药-重新建造生效";
    case FnLabel::kInstChrono:          return u8"瞬间超时空";
    case FnLabel::kSpySpy:              return u8"无间道";
    case FnLabel::kAdjustGameSpeed:     return u8"任务调速";
    default:                            return u8"unknown";
  }
}

constexpr const char8_t* GetFnStrEn(FnLabel label) {
  switch (label) {
    case FnLabel::kApply:               return u8"Apply";
    case FnLabel::kFastBuild:           return u8"Fast build";
    case FnLabel::kDeleteUnit:          return u8"Delete unit";
    case FnLabel::kClearShroud:         return u8"Clear shroud";
    case FnLabel::kGiveMeABomb:         return u8"Give me a bomb";
    case FnLabel::kUnitLevelUp:         return u8"Selected units level up";
    case FnLabel::kUnitSpeedUp:         return u8"Selected units speed up";
    case FnLabel::kIAMWinner:           return u8"I am winner";
    case FnLabel::kThisIsMine:          return u8"This is mine";
    case FnLabel::kGod:                 return u8"I am god";
    case FnLabel::kInstBuild:           return u8"Instant build";
    case FnLabel::kUnlimitSuperWeapon:  return u8"No super weapon cd";
    case FnLabel::kInstFire:            return u8"No fire cd";
    case FnLabel::kInstTurn:            return u8"Instant turn";
    case FnLabel::kRangeToYourBase:     return u8"Range to your base";
    case FnLabel::kFireToYourBase:      return u8"Fire to your base";
    case FnLabel::kFreezeGapGenerator:  return u8"Disable gap generator";
    case FnLabel::kSellTheWorld:        return u8"Sell the world";
    case FnLabel::kBuildEveryWhere:     return u8"Build everywhere";
    case FnLabel::kAutoRepair:          return u8"Auto repair";
    case FnLabel::kSocialismMajesty:    return u8"Socialism majesty";
    case FnLabel::kMakeCapturedMine:    return u8"Make captured mine";
    case FnLabel::kMakeGarrisonedMine:  return u8"Make garrisoned mine";
    case FnLabel::kInvadeMode:          return u8"Invade mode";
    case FnLabel::kUnlimitTech:         return u8"Unlimit technology";
    case FnLabel::kUnlimitFirePower:    return u8"Unlimit gun power";
    case FnLabel::kInstChrono:          return u8"Instant chrono";
    case FnLabel::kSpySpy:              return u8"Spy spy";
    case FnLabel::kAdjustGameSpeed:     return u8"Enable game speed adjustment";
    default:                            return u8"unknown";
  }
}

inline const char8_t* GetFnStr(FnLabel label, Lang lang) {
  if (lang == Lang::kZh) {
    return GetFnStrZh(label);
  } else if (lang == Lang::kEn) {
    return GetFnStrEn(label);
  } else {
    UNREACHABLE();
  }
}

}  // namespace frontend
}  // namespace yrtr
