#include "logging.h"
#include "trainer_func.h"

namespace yrtr {

namespace {
static void OnBtnApply() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

static void OnBtnFastBuild() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

static void OnBtnDeleteUnit() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

static void OnBtnClearShroud() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

static void OnBtnGiveMeABomb() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

static void OnBtnUnitLevelUp() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

static void OnBtnUnitSpeedUp() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

static void OnBtnIAMWinner() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

static void OnBtnThisIsMine() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

static void OnBtnIAMGhost() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

static void OnCkboxGod(bool activate) {
    DLOG(INFO, "Trigger {}", __FUNCTION__, activate);

}

static void OnCkboxInstBuild(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxUnlimitSuperWeapon(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxUnlimitRadar(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxInstShot(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxInstTurn(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxRangeToYourBase(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxFireToYourBase(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxFreezeGapGenerator(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxFreezeUnit(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxSellTheWorld(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxUnlimitPower(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxBuildEveryWhere(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxAutoRepaire(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxEnermyRepaireDown(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxSocialismTheBest(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxMakeAttackedMine(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxMakeOccupiedMine(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxMakeGarrisonedMine(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxInvadeMode(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxUnlimitTech(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxInstLoad(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxUnlimitFirePower(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxInstChrono(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxSpySpy(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxInfantrySlip(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxUnitLeveledUp(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

static void OnCkboxAdjustGameSpeed(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}


static void InitButtons(ImGuiContext& ctx) {
    ctx.AddButtonListener(FnLabel::kApply, OnBtnApply);
    ctx.AddButtonListener(FnLabel::kFastBuild, OnBtnFastBuild);
    ctx.AddButtonListener(FnLabel::kDeleteUnit, OnBtnDeleteUnit);
    ctx.AddButtonListener(FnLabel::kClearShroud, OnBtnClearShroud);
    ctx.AddButtonListener(FnLabel::kGiveMeABomb, OnBtnGiveMeABomb);
    ctx.AddButtonListener(FnLabel::kUnitLevelUp, OnBtnUnitLevelUp);
    ctx.AddButtonListener(FnLabel::kUnitSpeedUp, OnBtnUnitSpeedUp);
    ctx.AddButtonListener(FnLabel::kIAMWinner, OnBtnIAMWinner);
    ctx.AddButtonListener(FnLabel::kThisIsMine, OnBtnThisIsMine);
    ctx.AddButtonListener(FnLabel::kIAMGhost, OnBtnIAMGhost);
}

static void InitCheckboxes(ImGuiContext& ctx) {
    ctx.AddCheckboxListener(FnLabel::kGod, OnCkboxGod);
    ctx.AddCheckboxListener(FnLabel::kInstBuild, OnCkboxInstBuild);
    ctx.AddCheckboxListener(FnLabel::kUnlimitSuperWeapon, OnCkboxUnlimitSuperWeapon);
    ctx.AddCheckboxListener(FnLabel::kUnlimitRadar, OnCkboxUnlimitRadar);
    ctx.AddCheckboxListener(FnLabel::kInstShot, OnCkboxInstShot);
    ctx.AddCheckboxListener(FnLabel::kInstTurn, OnCkboxInstTurn);
    ctx.AddCheckboxListener(FnLabel::kRangeToYourBase, OnCkboxRangeToYourBase);
    ctx.AddCheckboxListener(FnLabel::kFireToYourBase, OnCkboxFireToYourBase);
    ctx.AddCheckboxListener(FnLabel::kFreezeGapGenerator, OnCkboxFreezeGapGenerator);
    ctx.AddCheckboxListener(FnLabel::kFreezeUnit, OnCkboxFreezeUnit);
    ctx.AddCheckboxListener(FnLabel::kSellTheWorld, OnCkboxSellTheWorld);
    ctx.AddCheckboxListener(FnLabel::kUnlimitPower, OnCkboxUnlimitPower);
    ctx.AddCheckboxListener(FnLabel::kBuildEveryWhere, OnCkboxBuildEveryWhere);
    ctx.AddCheckboxListener(FnLabel::kAutoRepaire, OnCkboxAutoRepaire);
    ctx.AddCheckboxListener(FnLabel::kEnermyRepaireDown, OnCkboxEnermyRepaireDown);
    ctx.AddCheckboxListener(FnLabel::kSocialismTheBest, OnCkboxSocialismTheBest);
    ctx.AddCheckboxListener(FnLabel::kMakeAttackedMine, OnCkboxMakeAttackedMine);
    ctx.AddCheckboxListener(FnLabel::kMakeOccupiedMine, OnCkboxMakeOccupiedMine);
    ctx.AddCheckboxListener(FnLabel::kMakeGarrisonedMine, OnCkboxMakeGarrisonedMine);
    ctx.AddCheckboxListener(FnLabel::kInvadeMode, OnCkboxInvadeMode);
    ctx.AddCheckboxListener(FnLabel::kUnlimitTech, OnCkboxUnlimitTech);
    ctx.AddCheckboxListener(FnLabel::kInstLoad, OnCkboxInstLoad);
    ctx.AddCheckboxListener(FnLabel::kUnlimitFirePower, OnCkboxUnlimitFirePower);
    ctx.AddCheckboxListener(FnLabel::kInstChrono, OnCkboxInstChrono);
    ctx.AddCheckboxListener(FnLabel::kSpySpy, OnCkboxSpySpy);
    ctx.AddCheckboxListener(FnLabel::kInfantrySlip, OnCkboxInfantrySlip);
    ctx.AddCheckboxListener(FnLabel::kUnitLeveledUp, OnCkboxUnitLeveledUp);
    ctx.AddCheckboxListener(FnLabel::kAdjustGameSpeed, OnCkboxAdjustGameSpeed);
}
}

void InitGUI(ImGuiContext& ctx) {
    InitButtons(ctx);
    InitCheckboxes(ctx);
}

}  // namespace yrtr
