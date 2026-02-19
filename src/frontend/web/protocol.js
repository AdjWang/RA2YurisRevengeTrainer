export {
  BtnFnLabelFirst, BtnFnLabelLast,
  CheckboxFnLabelFirst, CheckboxFnLabelLast,
  FnLabel, strFnLabel, getFnLabelHotkey
};

const BtnFnLabelFirst = 1;
const BtnFnLabelLast = 8;
const CheckboxFnLabelFirst = 9;
const CheckboxFnLabelLast = 28;
// Enum mapping
const FnLabel = {
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
  kMakeCapturedMine: 21,
  kMakeGarrisonedMine: 22,
  kInvadeMode: 23,
  kUnlimitTech: 24,
  kUnlimitFirePower: 25,
  kInstChrono: 26,
  kSpySpy: 27,
  kAdjustGameSpeed: 28,
  kCount: 29
};

// Helper function to get string representation of FnLabel. Only for internal
// use. To get localized name, use getGuiStr and getFnStr.
function strFnLabel(label) {
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
    case FnLabel.kAdjustGameSpeed: return "AdjustGameSpeed";
    case FnLabel.kCount: return "Count";
    default: return "unknown";
  }
}

function getFnLabelHotkey(label) {
  switch (label) {
    // Button
    case FnLabel.kApply: return "Digit1";
    case FnLabel.kIAMWinner: return "Digit2";
    case FnLabel.kDeleteUnit: return "Digit3";
    case FnLabel.kClearShroud: return "Digit4";
    case FnLabel.kGiveMeABomb: return "Digit5";
    case FnLabel.kUnitLevelUp: return "Digit6";
    case FnLabel.kUnitSpeedUp: return "Digit7";
    case FnLabel.kFastBuild: return "Digit8";
    case FnLabel.kThisIsMine: return "Digit9";
    // Checkbox
    case FnLabel.kGod: return "KeyQ";
    case FnLabel.kInstBuild: return "KeyW";
    case FnLabel.kUnlimitSuperWeapon: return "KeyE";
    case FnLabel.kInstFire: return "KeyT";
    case FnLabel.kInstTurn: return "KeyY";
    case FnLabel.kRangeToYourBase: return "KeyU";
    case FnLabel.kFireToYourBase: return "KeyI";
    case FnLabel.kFreezeGapGenerator: return "KeyO";
    case FnLabel.kSellTheWorld: return "KeyA";
    case FnLabel.kBuildEveryWhere: return "KeyD";
    case FnLabel.kAutoRepair: return "KeyF";
    case FnLabel.kSocialismMajesty: return "KeyH";
    case FnLabel.kMakeCapturedMine: return "KeyL";
    case FnLabel.kMakeGarrisonedMine: return "KeyZ";
    case FnLabel.kInvadeMode: return "KeyX";
    case FnLabel.kUnlimitTech: return "KeyV";
    case FnLabel.kUnlimitFirePower: return "KeyB";
    case FnLabel.kInstChrono: return "KeyN";
    case FnLabel.kSpySpy: return "KeyM";
    case FnLabel.kAdjustGameSpeed: return "Period";   /* . */
    default: return "Invalid";
  }
}
