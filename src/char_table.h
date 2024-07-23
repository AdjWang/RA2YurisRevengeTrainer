#pragma once
#include "vendor.h"

namespace yrtr {

enum class FnLabel {
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
    kUnitLeveledUp,
    kAdjustGameSpeed,
};

constexpr const char8_t* GetFnStr(FnLabel label) {
    switch(label){
        case FnLabel::kState:               return u8"状态";
        case FnLabel::kStateOk:             return u8"游戏运行中";
        case FnLabel::kStateIdle:           return u8"未检测到游戏";
        case FnLabel::kMoney:               return u8"钱";
        case FnLabel::kApply:               return u8"修改";
        case FnLabel::kFastBuild:           return u8"快速建造";
        case FnLabel::kDeleteUnit:          return u8"删除单位";
        case FnLabel::kClearShroud:         return u8"地图全开";
        case FnLabel::kGiveMeABomb:         return u8"核弹攻击";
        case FnLabel::kUnitLevelUp:         return u8"单位升级";
        case FnLabel::kUnitSpeedUp:         return u8"单位加速";
        case FnLabel::kIAMWinner:           return u8"立即胜利";
        case FnLabel::kThisIsMine:          return u8"这是我的";
        case FnLabel::kIAMGhost:            return u8"成为幽灵玩家";
        case FnLabel::kGod:                 return u8"无敌";
        case FnLabel::kInstBuild:           return u8"瞬间建造";
        case FnLabel::kUnlimitSuperWeapon:  return u8"无限超武";
        case FnLabel::kUnlimitRadar:        return u8"永久雷达";
        case FnLabel::kInstFire:            return u8"极速攻击";
        case FnLabel::kInstTurn:            return u8"极速转身";
        case FnLabel::kRangeToYourBase:     return u8"远程攻击";
        case FnLabel::kFireToYourBase:      return u8"远程警戒";
        case FnLabel::kFreezeGapGenerator:  return u8"瘫痪裂缝产生器";
        case FnLabel::kFreezeUnit:          return u8"瘫痪敌方单位";
        case FnLabel::kSellTheWorld:        return u8"卖卖卖";
        case FnLabel::kUnlimitPower:        return u8"无限电力";
        case FnLabel::kBuildEveryWhere:     return u8"随意建筑";
        case FnLabel::kAutoRepair:          return u8"自动修理";
        case FnLabel::kEnermyRevertRepair:  return u8"敌方血越修越少";
        case FnLabel::kSocialismTheBest:    return u8"社会主义万岁";
        case FnLabel::kMakeAttackedMine:    return u8"全是我的-攻击";
        case FnLabel::kMakeCapturedMine:    return u8"全是我的-占领";
        case FnLabel::kMakeGarrisonedMine:  return u8"全是我的-房屋驻军";
        case FnLabel::kInvadeMode:          return u8"侵略模式";
        case FnLabel::kUnlimitTech:         return u8"全科技";
        case FnLabel::kFastReload:          return u8"极速重装-重新建造生效";
        case FnLabel::kUnlimitFirePower:    return u8"大量弹药-重新建造生效";
        case FnLabel::kInstChrono:          return u8"瞬间超时空";
        case FnLabel::kSpySpy:              return u8"无间道";
        case FnLabel::kInfantrySlip:        return u8"滑板鞋";
        case FnLabel::kUnitLeveledUp:       return u8"部队全部三级";
        case FnLabel::kAdjustGameSpeed:     return u8"任务调速";
        default: return u8"unknown";
    }
}

}  // namespace yrtr
