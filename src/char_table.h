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

inline static const std::unordered_map<FnLabel, const char8_t*> kZhLabels{
    {FnLabel::kState,               u8"状态"},
    {FnLabel::kStateOk,             u8"游戏运行中"},
    {FnLabel::kStateIdle,           u8"未检测到游戏"},
    {FnLabel::kMoney,               u8"钱"},
    {FnLabel::kApply,               u8"修改"},
    {FnLabel::kFastBuild,           u8"快速建造"},
    {FnLabel::kDeleteUnit,          u8"删除单位"},
    {FnLabel::kClearShroud,         u8"地图全开"},
    {FnLabel::kGiveMeABomb,         u8"核弹攻击"},
    {FnLabel::kUnitLevelUp,         u8"单位升级"},
    {FnLabel::kUnitSpeedUp,         u8"单位加速"},
    {FnLabel::kIAMWinner,           u8"立即胜利"},
    {FnLabel::kThisIsMine,          u8"这是我的"},
    {FnLabel::kIAMGhost,            u8"成为幽灵玩家"},
    {FnLabel::kGod,                 u8"无敌"},
    {FnLabel::kInstBuild,           u8"瞬间建造"},
    {FnLabel::kUnlimitSuperWeapon,  u8"无限超武"},
    {FnLabel::kUnlimitRadar,        u8"永久雷达"},
    {FnLabel::kInstFire,            u8"极速攻击"},
    {FnLabel::kInstTurn,            u8"极速转身"},
    {FnLabel::kRangeToYourBase,     u8"远程攻击"},
    {FnLabel::kFireToYourBase,      u8"远程警戒"},
    {FnLabel::kFreezeGapGenerator,  u8"瘫痪裂缝产生器"},
    {FnLabel::kFreezeUnit,          u8"瘫痪敌方单位"},
    {FnLabel::kSellTheWorld,        u8"卖卖卖"},
    {FnLabel::kUnlimitPower,        u8"无限电力"},
    {FnLabel::kBuildEveryWhere,     u8"随意建筑"},
    {FnLabel::kAutoRepair,          u8"自动修理"},
    {FnLabel::kEnermyRevertRepair,  u8"敌方血越修越少"},
    {FnLabel::kSocialismTheBest,    u8"社会主义万岁"},
    {FnLabel::kMakeAttackedMine,    u8"全是我的-攻击"},
    {FnLabel::kMakeCapturedMine,    u8"全是我的-占领"},
    {FnLabel::kMakeGarrisonedMine,  u8"全是我的-房屋驻军"},
    {FnLabel::kInvadeMode,          u8"侵略模式"},
    {FnLabel::kUnlimitTech,         u8"全科技"},
    {FnLabel::kFastReload,          u8"极速重装-重新建造生效"},
    {FnLabel::kUnlimitFirePower,    u8"大量弹药-重新建造生效"},
    {FnLabel::kInstChrono,          u8"瞬间超时空"},
    {FnLabel::kSpySpy,              u8"无间道"},
    {FnLabel::kInfantrySlip,        u8"滑板鞋"},
    {FnLabel::kUnitLeveledUp,       u8"部队全部三级"},
    {FnLabel::kAdjustGameSpeed,     u8"任务调速"},
};

inline const char* GetFnChar(FnLabel label) {
    return (const char*)kZhLabels.at(label);
}

}  // namespace yrtr
