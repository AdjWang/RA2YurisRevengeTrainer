const BtnFnLabelFirst = 1;
const BtnFnLabelLast = 8;
const CheckboxFnLabelFirst = 9;
const CheckboxFnLabelLast = 27;
// Enum mapping
const FnLabel = {
  kInvalid: -1,
  // Button
  kApply: 0,
  kFastBuild: 1,
  kDeleteUnit: 2,
  kClearShroud: 3,
  kGiveMeABomb: 4,
  kUnitLevelUp: 5,
  kUnitSpeedUp: 6,
  kIAMWinner: 7,
  kThisIsMine: 8,
  // Checkbox
  kGod: 9,
  kInstBuild: 10,
  kUnlimitSuperWeapon: 11,
  kInstFire: 12,
  kInstTurn: 13,
  kRangeToYourBase: 14,
  kFireToYourBase: 15,
  kFreezeGapGenerator: 16,
  kSellTheWorld: 17,
  kBuildEveryWhere: 18,
  kAutoRepair: 19,
  kSocialismMajesty: 20,
  kMakeGarrisonedMine: 21,
  kInvadeMode: 22,
  kUnlimitTech: 23,
  kUnlimitFirePower: 24,
  kInstChrono: 25,
  kSpySpy: 26,
  kAdjustGameSpeed: 27,
  kCount: 28
};

// Helper function to get string representation of FnLabel
function strFnLabel(label) {
  switch (label) {
    case FnLabel.kInvalid: return "Invalid";
    // Button
    case FnLabel.kApply: return "Apply";
    case FnLabel.kFastBuild: return "FastBuild";
    case FnLabel.kDeleteUnit: return "DeleteUnit";
    case FnLabel.kClearShroud: return "ClearShroud";
    case FnLabel.kGiveMeABomb: return "GiveMeABomb";
    case FnLabel.kUnitLevelUp: return "UnitLevelUp";
    case FnLabel.kUnitSpeedUp: return "UnitSpeedUp";
    case FnLabel.kIAMWinner: return "IAMWinner";
    case FnLabel.kThisIsMine: return "ThisIsMine";
    // Checkbox
    case FnLabel.kGod: return "God";
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
    case FnLabel.kMakeGarrisonedMine: return "MakeGarrisonedMine";
    case FnLabel.kInvadeMode: return "InvadeMode";
    case FnLabel.kUnlimitTech: return "UnlimitTech";
    case FnLabel.kUnlimitFirePower: return "UnlimitFirePower";
    case FnLabel.kInstChrono: return "InstChrono";
    case FnLabel.kSpySpy: return "SpySpy";
    case FnLabel.kAdjustGameSpeed: return "AdjustGameSpeed";
    case FnLabel.kCount: return "Count";
    default: return "unknown";
  }
}
