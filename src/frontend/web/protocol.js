// [from, to]
export const BtnFnLabelFirst = 1;
export const BtnFnLabelLast = 8;
export const CheckboxFnLabelFirst = 9;
export const CheckboxFnLabelLast = 30;
export const SliderFnLabelFirst = 31;
export const SliderFnLabelLast = 31;
// Enum mapping
export const FnLabel = {
  kInvalid: -1,
  // Button
  kApply: 0,
  kIAMWinner: 1,
  kDeleteUnit: 2,
  kClearShroud: 3,
  kGiveMeABomb: 4,
  kUnitLevelUp: 5,
  kUnitSpeedUp: 6,
  kFastBuild: 7,
  kThisIsMine: 8,
  // Checkbox
  kGod: 9,
  kNoPowerDrain: 10,
  kInstBuild: 11,
  kUnlimitSuperWeapon: 12,
  kInstFire: 13,
  kInstTurn: 14,
  kRangeToYourBase: 15,
  kFireToYourBase: 16,
  kFreezeGapGenerator: 17,
  kSellTheWorld: 18,
  kBuildEveryWhere: 19,
  kAutoRepair: 20,
  kSocialismMajesty: 21,
  kMakeCapturedMine: 22,
  kMakeGarrisonedMine: 23,
  kInvadeMode: 24,
  kUnlimitTech: 25,
  kUnlimitFirePower: 26,
  kInstChrono: 27,
  kSpySpy: 28,
  kSelectEnemy: 29,
  kFreezeGame: 30,
  // Slider
  kAdjustGameSpeed: 31,
  kCount: 32
};

// Helper function to get string representation of FnLabel. Only for internal
// use. To get localized name, use getGuiStr and getFnStr.
export function strFnLabel(label) {
  switch (label) {
    case FnLabel.kInvalid: return "Invalid";
    // Button
    case FnLabel.kApply: return "Apply";
    case FnLabel.kIAMWinner: return "IAMWinner";
    case FnLabel.kDeleteUnit: return "DeleteUnit";
    case FnLabel.kClearShroud: return "ClearShroud";
    case FnLabel.kGiveMeABomb: return "GiveMeABomb";
    case FnLabel.kUnitLevelUp: return "UnitLevelUp";
    case FnLabel.kUnitSpeedUp: return "UnitSpeedUp";
    case FnLabel.kFastBuild: return "FastBuild";
    case FnLabel.kThisIsMine: return "ThisIsMine";
    // Checkbox
    case FnLabel.kGod: return "God";
    case FnLabel.kNoPowerDrain: return "NoPowerDrain";
    case FnLabel.kInstBuild: return "InstBuild";
    case FnLabel.kUnlimitSuperWeapon: return "UnlimitSuperWeapon";
    case FnLabel.kInstFire: return "InstFire";
    case FnLabel.kInstTurn: return "InstTurn";
    case FnLabel.kRangeToYourBase: return "RangeToYourBase";
    case FnLabel.kFireToYourBase: return "FireToYourBase";
    case FnLabel.kFreezeGapGenerator: return "FreezeGapGenerator";
    case FnLabel.kSellTheWorld: return "SellTheWorld";
    case FnLabel.kBuildEveryWhere: return "BuildEveryWhere";
    case FnLabel.kAutoRepair: return "AutoRepair";
    case FnLabel.kSocialismMajesty: return "SocialismMajesty";
    case FnLabel.kMakeCapturedMine: return "MakeCapturedMine";
    case FnLabel.kMakeGarrisonedMine: return "MakeGarrisonedMine";
    case FnLabel.kInvadeMode: return "InvadeMode";
    case FnLabel.kUnlimitTech: return "UnlimitTech";
    case FnLabel.kUnlimitFirePower: return "UnlimitFirePower";
    case FnLabel.kInstChrono: return "InstChrono";
    case FnLabel.kSpySpy: return "SpySpy";
    case FnLabel.kSelectEnemy: return "SelectEnemy";
    case FnLabel.kFreezeGame: return "FreezeGame";
    // Slider
    case FnLabel.kAdjustGameSpeed: return "AdjustGameSpeed";
    case FnLabel.kCount: return "Count";
    default: return "unknown";
  }
}
