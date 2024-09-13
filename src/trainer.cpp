#include "config.h"
#include "logging.h"
#include "gui.h"
#include "trainer.h"
#include "win32utils.h"
#include "ce_scripts.h"

namespace yrtr {

std::unique_ptr<Trainer> Trainer::trainer_target_(nullptr);

namespace {
#define BIND_FN(fn) std::bind_front(&Trainer::##fn, Trainer::instance())
static void InitButtons(GuiContext& ctx) {
    ctx.AddInputListener(FnLabel::kApply,        BIND_FN(OnBtnApply));
    ctx.AddButtonListener(FnLabel::kFastBuild,   BIND_FN(OnBtnFastBuild));
    ctx.AddButtonListener(FnLabel::kDeleteUnit,  BIND_FN(OnBtnDeleteUnit));
    ctx.AddButtonListener(FnLabel::kClearShroud, BIND_FN(OnBtnClearShroud));
    ctx.AddButtonListener(FnLabel::kGiveMeABomb, BIND_FN(OnBtnGiveMeABomb));
    ctx.AddButtonListener(FnLabel::kUnitLevelUp, BIND_FN(OnBtnUnitLevelUp));
    ctx.AddButtonListener(FnLabel::kUnitSpeedUp, BIND_FN(OnBtnUnitSpeedUp));
    ctx.AddButtonListener(FnLabel::kIAMWinner,   BIND_FN(OnBtnIAMWinner));
    ctx.AddButtonListener(FnLabel::kThisIsMine,  BIND_FN(OnBtnThisIsMine));
    ctx.AddButtonListener(FnLabel::kIAMGhost,    BIND_FN(OnBtnIAMGhost));
}

static void InitCheckboxes(GuiContext& ctx) {
    ctx.AddCheckboxListener(FnLabel::kGod,                BIND_FN(OnCkboxGod));
    ctx.AddCheckboxListener(FnLabel::kInstBuild,          BIND_FN(OnCkboxInstBuild));
    ctx.AddCheckboxListener(FnLabel::kUnlimitSuperWeapon, BIND_FN(OnCkboxUnlimitSuperWeapon));
    ctx.AddCheckboxListener(FnLabel::kUnlimitRadar,       BIND_FN(OnCkboxUnlimitRadar));
    ctx.AddCheckboxListener(FnLabel::kInstFire,           BIND_FN(OnCkboxInstFire));
    ctx.AddCheckboxListener(FnLabel::kInstTurn,           BIND_FN(OnCkboxInstTurn));
    ctx.AddCheckboxListener(FnLabel::kRangeToYourBase,    BIND_FN(OnCkboxRangeToYourBase));
    ctx.AddCheckboxListener(FnLabel::kFireToYourBase,     BIND_FN(OnCkboxFireToYourBase));
    ctx.AddCheckboxListener(FnLabel::kFreezeGapGenerator, BIND_FN(OnCkboxFreezeGapGenerator));
    ctx.AddCheckboxListener(FnLabel::kFreezeUnit,         BIND_FN(OnCkboxFreezeUnit));
    ctx.AddCheckboxListener(FnLabel::kSellTheWorld,       BIND_FN(OnCkboxSellTheWorld));
    ctx.AddCheckboxListener(FnLabel::kUnlimitPower,       BIND_FN(OnCkboxUnlimitPower));
    ctx.AddCheckboxListener(FnLabel::kBuildEveryWhere,    BIND_FN(OnCkboxBuildEveryWhere));
    ctx.AddCheckboxListener(FnLabel::kAutoRepair,         BIND_FN(OnCkboxAutoRepair));
    ctx.AddCheckboxListener(FnLabel::kEnermyRevertRepair, BIND_FN(OnCkboxEnermyRevertRepair));
    ctx.AddCheckboxListener(FnLabel::kSocialismTheBest,   BIND_FN(OnCkboxSocialismTheBest));
    ctx.AddCheckboxListener(FnLabel::kMakeAttackedMine,   BIND_FN(OnCkboxMakeAttackedMine));
    ctx.AddCheckboxListener(FnLabel::kMakeCapturedMine,   BIND_FN(OnCkboxMakeCapturedMine));
    ctx.AddCheckboxListener(FnLabel::kMakeGarrisonedMine, BIND_FN(OnCkboxMakeGarrisonedMine));
    ctx.AddCheckboxListener(FnLabel::kInvadeMode,         BIND_FN(OnCkboxInvadeMode));
    ctx.AddCheckboxListener(FnLabel::kUnlimitTech,        BIND_FN(OnCkboxUnlimitTech));
    ctx.AddCheckboxListener(FnLabel::kFastReload,         BIND_FN(OnCkboxFastReload));
    ctx.AddCheckboxListener(FnLabel::kUnlimitFirePower,   BIND_FN(OnCkboxUnlimitFirePower));
    ctx.AddCheckboxListener(FnLabel::kInstChrono,         BIND_FN(OnCkboxInstChrono));
    ctx.AddCheckboxListener(FnLabel::kSpySpy,             BIND_FN(OnCkboxSpySpy));
    ctx.AddCheckboxListener(FnLabel::kInfantrySlip,       BIND_FN(OnCkboxInfantrySlip));
    ctx.AddCheckboxListener(FnLabel::kUnitLeveledUp,      BIND_FN(OnCkboxUnitLeveledUp));
    ctx.AddCheckboxListener(FnLabel::kAdjustGameSpeed,    BIND_FN(OnCkboxAdjustGameSpeed));
}
#undef BIND_FN

static void InitStates(State& state) {
    state.game_state = std::string((const char*)GetFnStr(FnLabel::kStateIdle, config::GetGlobalConfig().lang));
    state.ckbox_states.emplace(FnLabel::kGod,                CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kInstBuild,          CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kUnlimitSuperWeapon, CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kUnlimitRadar,       CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kInstFire,           CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kInstTurn,           CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kRangeToYourBase,    CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kFireToYourBase,     CheckboxState{.enable=false, .activate=false});
    state.ckbox_states.emplace(FnLabel::kFreezeGapGenerator, CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kFreezeUnit,         CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kSellTheWorld,       CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kUnlimitPower,       CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kBuildEveryWhere,    CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kAutoRepair,         CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kEnermyRevertRepair, CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kSocialismTheBest,   CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kMakeAttackedMine,   CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kMakeCapturedMine,   CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kMakeGarrisonedMine, CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kInvadeMode,         CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kUnlimitTech,        CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kFastReload,         CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kUnlimitFirePower,   CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kInstChrono,         CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kSpySpy,             CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kInfantrySlip,       CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kUnitLeveledUp,      CheckboxState{.enable=true, .activate=false});
    state.ckbox_states.emplace(FnLabel::kAdjustGameSpeed,    CheckboxState{.enable=true, .activate=false});
}

static void ResetStates(State& state) {
    for (CheckboxState& ckbox_state : std::views::values(state.ckbox_states)) {
        ckbox_state.activate = false;
        ckbox_state.enable = true;
    }
    state.ckbox_states[FnLabel::kFireToYourBase].enable = false;
}
}  // namespace

void Trainer::Init(std::string_view exe_name, GuiContext& gui_ctx, State& state) {
    trainer_target_.reset(new Trainer(exe_name, state));
    InitStates(state);
    InitButtons(gui_ctx);
    InitCheckboxes(gui_ctx);
}

Trainer::Trainer(std::string_view exe_name, State& state)
    : state_(state),
      exe_name_(exe_name),
      attached_(false) {}

#define CHECK_MEMAPI_OR_REPORT()                                              \
    if (mem_api_ == nullptr) {                                                \
        LOG(WARN, "MemoryAPI not initialized. Ensure the game has started."); \
        return;                                                               \
    }

// For some unknown reason, spanning between game instances may cause previous
// handle invalid. Give it a chance to retry.
#define CHECK_REPORT(exp)                                                      \
    if (!(exp)) {                                                              \
        LOG(WARN, __FUNCTION__ " Retry execute " #exp);                        \
        mem_api_.reset();                                                      \
        UpdateProcState();                                                     \
        if (!(exp)) {                                                          \
            LOG(WARN, __FUNCTION__ " Failed to execute " #exp);                \
            return;                                                            \
        }                                                                      \
    }

void Trainer::OnBtnApply(uint32_t val) {
    DLOG(INFO, "Trigger {} val={}", __FUNCTION__, val);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(IsGaming());
    CHECK_REPORT(WriteCredit(val));
}

void Trainer::OnBtnFastBuild() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(IsGaming());
    uint32_t data;
    CHECK_REPORT(mem_api_->ReadMemory(0x00A83D4C, &data));
    for (uint32_t i = 1, j = 0; i <= 5; j += 4, i++) {
        CHECK_REPORT(mem_api_->WriteMemory(data + j + 0x5378, 15));
    }
}

void Trainer::OnBtnDeleteUnit() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(IsGaming());
    CHECK_REPORT(mem_api_->CreateRemoteThread(AsmDeleteUnit));
}

void Trainer::OnBtnClearShroud() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(IsGaming());
    CHECK_REPORT(mem_api_->CreateRemoteThread(AsmClearShroud));
}

void Trainer::OnBtnGiveMeABomb() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(IsGaming());
    CHECK_REPORT(mem_api_->CreateRemoteThread(AsmNuclearBomb));
}

void Trainer::OnBtnUnitLevelUp() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(IsGaming());
    CHECK_REPORT(mem_api_->CreateRemoteThread(AsmUnitLevelUp));
}

void Trainer::OnBtnUnitSpeedUp() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(IsGaming());
    CHECK_REPORT(mem_api_->CreateRemoteThread(AsmUnitSpeedUp));
}

void Trainer::OnBtnIAMWinner() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(IsGaming());
    CHECK_REPORT(mem_api_->WriteMemory(0x00A83D49, static_cast<uint8_t>(1)));
}

void Trainer::OnBtnThisIsMine() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(IsGaming());
    CHECK_REPORT(mem_api_->CreateRemoteThread(AsmThisIsMine));
}

void Trainer::OnBtnIAMGhost() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(IsGaming());
    CHECK_REPORT(TobeGhost());
}

// Use a macro instead of a function because fmt::format_string has to be a
// constexpr
#define FMT_AUTOASSEMBLE(script, activate)                                   \
    mem_api_->AutoAssemble(fmt::format(script, fmt::arg("proc", exe_name_)), \
                           activate)

void Trainer::OnCkboxGod(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kGodPlayer, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kAntiChrono, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kAntiChronoDisbuild, activate));
    CHECK_REPORT(AntiChronoDisable(activate));
    state_.ckbox_states[FnLabel::kGod].activate = activate;
}

void Trainer::OnCkboxInstBuild(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInstBuild, activate));
    state_.ckbox_states[FnLabel::kInstBuild].activate = activate;
}

void Trainer::OnCkboxUnlimitSuperWeapon(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    state_.ckbox_states[FnLabel::kUnlimitSuperWeapon].activate = activate;
}

void Trainer::OnCkboxUnlimitRadar(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    state_.ckbox_states[FnLabel::kUnlimitRadar].activate = activate;
}

void Trainer::OnCkboxInstFire(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInstFire, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInstTurnBattery, activate));
    state_.ckbox_states[FnLabel::kInstFire].activate = activate;
}

void Trainer::OnCkboxInstTurn(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInstTurnRound, activate));
    state_.ckbox_states[FnLabel::kInstTurn].activate = activate;
}

void Trainer::OnCkboxRangeToYourBase(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kRangeToYourBase, activate));
    state_.ckbox_states[FnLabel::kRangeToYourBase].activate = activate;
    if (activate) {
        state_.ckbox_states[FnLabel::kFireToYourBase].enable = true;
    } else {
        state_.ckbox_states[FnLabel::kFireToYourBase].enable = false;
        if(state_.ckbox_states[FnLabel::kFireToYourBase].activate) {
            OnCkboxFireToYourBase(false);
        }
    }
}

void Trainer::OnCkboxFireToYourBase(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kFireToYourBase, activate));
    state_.ckbox_states[FnLabel::kFireToYourBase].activate = activate;
}

void Trainer::OnCkboxFreezeGapGenerator(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kFreezeGAGAP, activate));
    state_.ckbox_states[FnLabel::kFreezeGapGenerator].activate = activate;
}

void Trainer::OnCkboxFreezeUnit(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kFreezeUnit, activate));
    state_.ckbox_states[FnLabel::kFreezeUnit].activate = activate;
}

void Trainer::OnCkboxSellTheWorld(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kSellTheWorldOnCursor, activate));
    CHECK_REPORT(EnableSellAllBelong(activate));
    CHECK_REPORT(EnableSellAllBuilder(activate));
    state_.ckbox_states[FnLabel::kSellTheWorld].activate = activate;
}

void Trainer::OnCkboxUnlimitPower(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kUnlimitPower, activate));
    state_.ckbox_states[FnLabel::kUnlimitPower].activate = activate;
}

void Trainer::OnCkboxBuildEveryWhere(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kBuildEveryWhereGround, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kBuildEveryWhereWater, activate));
    state_.ckbox_states[FnLabel::kBuildEveryWhere].activate = activate;
}

void Trainer::OnCkboxAutoRepair(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kAutoRepair, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kAutoRepairNeutral, activate));
    state_.ckbox_states[FnLabel::kAutoRepair].activate = activate;
}

void Trainer::OnCkboxEnermyRevertRepair(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kRevertRepair, activate));
    state_.ckbox_states[FnLabel::kEnermyRevertRepair].activate = activate;
}

void Trainer::OnCkboxSocialismTheBest(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kRevertYuri, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kPermanentYuri, activate));
    state_.ckbox_states[FnLabel::kSocialismTheBest].activate = activate;
}

void Trainer::OnCkboxMakeAttackedMine(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kThisIsMineOnAttack, activate));
    state_.ckbox_states[FnLabel::kMakeAttackedMine].activate = activate;
}

void Trainer::OnCkboxMakeCapturedMine(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kThisIsMineOnCapture, activate));
    state_.ckbox_states[FnLabel::kMakeCapturedMine].activate = activate;
}

void Trainer::OnCkboxMakeGarrisonedMine(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kThisIsMineOnGarrison, activate));
    state_.ckbox_states[FnLabel::kMakeGarrisonedMine].activate = activate;
}

void Trainer::OnCkboxInvadeMode(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(InvadeMode(activate));
    state_.ckbox_states[FnLabel::kInvadeMode].activate = activate;
}

void Trainer::OnCkboxUnlimitTech(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kUnlimitTech, activate));
    state_.ckbox_states[FnLabel::kUnlimitTech].activate = activate;
}

void Trainer::OnCkboxFastReload(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kFastReload, activate));
    state_.ckbox_states[FnLabel::kFastReload].activate = activate;
}

void Trainer::OnCkboxUnlimitFirePower(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kUnlimitFirePower, activate));
    state_.ckbox_states[FnLabel::kUnlimitFirePower].activate = activate;
}

void Trainer::OnCkboxInstChrono(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInstChronoMove, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInstChronoAttack, activate));
    state_.ckbox_states[FnLabel::kInstChrono].activate = activate;
}

void Trainer::OnCkboxSpySpy(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kSpySpy, activate));
    state_.ckbox_states[FnLabel::kSpySpy].activate = activate;
}

void Trainer::OnCkboxInfantrySlip(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInfantrySlip, activate));
    state_.ckbox_states[FnLabel::kInfantrySlip].activate = activate;
}

void Trainer::OnCkboxUnitLeveledUp(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kUnitLeveledUp, activate));
    state_.ckbox_states[FnLabel::kUnitLeveledUp].activate = activate;
}

void Trainer::OnCkboxAdjustGameSpeed(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(
        mem_api_->WriteMemory(0x00A8EDDC, static_cast<uint8_t>(activate)));
    state_.ckbox_states[FnLabel::kAdjustGameSpeed].activate = activate;
}

void Trainer::OnProcWatchTimer() {
    UpdateProcState();
    if (!attached_) {
        DeactivateAll();
    }
}

void Trainer::OnFuncScanTimer() {
    if (!attached_ || !IsGaming()) {
        return;
    }
    if (state_.ckbox_states[FnLabel::kUnlimitRadar].activate) {
        CHECK_REPORT(UnlimitRadar());
    }
    if (state_.ckbox_states[FnLabel::kUnlimitSuperWeapon].activate) {
        CHECK_REPORT(UnlimitSuperWeapon());
    }
}

#undef FMT_AUTOASSEMBLE
#undef CHECK_MEMAPI_OR_REPORT
#undef CHECK_REPORT

void Trainer::UpdateProcState() {
    if (win32::GetProcessIDFromName(exe_name_.c_str(), &pid_)) {
        if (mem_api_ == nullptr) {
            DLOG(INFO, "GetProcessIDFromName found exe={} pid={:#x}", exe_name_,
                 pid_);
            mem_api_.reset(new win32::MemoryAPI(pid_));
            if (mem_api_->CheckHandle()) {
                attached_ = true;
                auto state = (const char *)yrtr::GetFnStr(
                    yrtr::FnLabel::kStateOk, config::GetGlobalConfig().lang);
                state_.game_state = std::string(state);
                DLOG(INFO, "MemoryAPI initialized");
            }
        }
    } else {
        // Not found target process
        DLOG(INFO, "GetProcessIDFromName searching exe={}", exe_name_);
        mem_api_.reset();
        attached_ = false;
        auto state = (const char *)yrtr::GetFnStr(
            yrtr::FnLabel::kStateIdle, config::GetGlobalConfig().lang);
        state_.game_state = std::string(state);
    }
}

void Trainer::DeactivateAll() {
    bool any = false;
    for (CheckboxState& state : std::views::values(state_.ckbox_states)) {
        if (state.activate) {
            any = true;
            break;
        }
    }
    if (!any) {
        return;
    }

    OnCkboxGod(false);
    OnCkboxInstBuild(false);
    OnCkboxUnlimitSuperWeapon(false);
    OnCkboxUnlimitRadar(false);
    OnCkboxInstFire(false);
    OnCkboxInstTurn(false);
    OnCkboxRangeToYourBase(false);
    OnCkboxFireToYourBase(false);
    OnCkboxFreezeGapGenerator(false);
    OnCkboxFreezeUnit(false);
    OnCkboxSellTheWorld(false);
    OnCkboxUnlimitPower(false);
    OnCkboxBuildEveryWhere(false);
    OnCkboxAutoRepair(false);
    OnCkboxEnermyRevertRepair(false);
    OnCkboxSocialismTheBest(false);
    OnCkboxMakeAttackedMine(false);
    OnCkboxMakeCapturedMine(false);
    OnCkboxMakeGarrisonedMine(false);
    OnCkboxInvadeMode(false);
    OnCkboxUnlimitTech(false);
    OnCkboxFastReload(false);
    OnCkboxUnlimitFirePower(false);
    OnCkboxInstChrono(false);
    OnCkboxSpySpy(false);
    OnCkboxInfantrySlip(false);
    OnCkboxUnitLeveledUp(false);
    OnCkboxAdjustGameSpeed(false);

    ResetStates(state_);
}

#define CHECK_MEMAPI_OR_RETURN_FALSE()                                        \
    if (mem_api_ == nullptr) {                                                \
        LOG(WARN, "MemoryAPI not initialized. Ensure the game has started."); \
        return false;                                                         \
    }

#define CHECK_RETF(exp) \
    if (!(exp)) {       \
        return false;   \
    }

bool Trainer::IsGaming() const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    uint32_t p_player;
    CHECK_RETF(mem_api_->ReadMemory(0x00A83D4C, &p_player));
    return p_player != 0;
}

bool Trainer::WriteSpeed(uint32_t speed) const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    return mem_api_->WriteMemory(0x00A8EB60, speed);
}

bool Trainer::WriteCredit(uint32_t credit) const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    uint32_t addr;
    CHECK_RETF(mem_api_->ReadAddress(0x00A83D4C, {0x30C}, &addr));
    return mem_api_->WriteMemory(addr, credit);
}

bool Trainer::WriteMaxBuildingCount(uint32_t val) const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    uint32_t addr;
    CHECK_RETF(mem_api_->ReadAddress(0x008871E0, {0xF0}, &addr));
    return mem_api_->WriteMemory(addr, val - 1);
}

bool Trainer::TobeGhost() const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    uint32_t select_num;
    // Check if selecting a building
    CHECK_RETF(mem_api_->ReadMemory(0x00A8ECC8, &select_num));
    if (select_num != 1) {
        return true;
    }
    uint32_t select_vft;
    CHECK_RETF(mem_api_->ReadMemory(0x00A8ECBC, {0, 0}, &select_vft));
    if (select_vft != 0x007E3EBC) {
        return true;
    }
    // Check if the building belongs to the player
    uint32_t select_owner;
    CHECK_RETF(mem_api_->ReadMemory(0x00A8ECBC, {0, 0x21C}, &select_owner));
    uint32_t p_player;
    CHECK_RETF(mem_api_->ReadMemory(0x00A83D4C, &p_player));
    if (select_owner != p_player) {
        return true;
    }
    // Cheat
    uint32_t player_id;
    CHECK_RETF(mem_api_->ReadMemory(0x00A83D4C, {0x30}, &player_id));
    // The human player id is always 0 in single player game
    if (player_id == 0) {
        player_id++;
    } else {
        player_id--;
    }
    uint32_t to_player_id;
    CHECK_RETF(mem_api_->ReadMemory(0x00A8022C, {4*player_id}, &to_player_id));
    uint32_t p_select_owner;
    CHECK_RETF(mem_api_->ReadAddress(0x00A8ECBC, {0, 0x21C}, &p_select_owner));
    CHECK_RETF(mem_api_->WriteMemory(p_select_owner, to_player_id));
    // Coarsely delete the selecting building, trigger the ghost bug
    CHECK_RETF(mem_api_->CreateRemoteThread(AsmDeleteUnit));
    return true;
}

// Prevent building list going to grey
bool Trainer::AntiChronoDisable(bool activate) const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    if (activate) {
        CHECK_RETF(mem_api_->WriteMemory(
            0x006A97B2, std::span<const uint8_t>({0x90, 0x90})));
    } else {
        CHECK_RETF(mem_api_->WriteMemory(
            0x006A97B2, std::span<const uint8_t>({0x74, 0x47})));
    }
    return true;
}

bool Trainer::InvadeMode(bool activate) const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    if (activate) {
        CHECK_RETF(mem_api_->WriteLongJump(0x006F85DD, 0x006F8604));
    } else {
        CHECK_RETF(mem_api_->WriteMemory(
            0x006F85DD,
            std::span<const uint8_t>({0x83, 0x38, 0x00, 0x74, 0x0E})));
    }
    return true;
}

// Allow sell things not belong to player
bool Trainer::EnableSellAllBelong(bool activate) const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    if (activate) {
        CHECK_RETF(mem_api_->WriteLongJump(0x004C6F48, 0x004C6F9C));
        CHECK_RETF(mem_api_->WriteMemory(0x004C6F48 + 5,
                                         std::span<const uint8_t>({0x90})));
    }
    else {
        CHECK_RETF(mem_api_->WriteMemory(
            0x004C6F48,
            std::span<const uint8_t>({0x0F, 0x85, 0xBB, 0x11, 0x00, 0x00})));
    }
    return true;
}

// Allow sell things not built by player
bool Trainer::EnableSellAllBuilder(bool activate) const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    if (activate) {
        CHECK_RETF(mem_api_->WriteMemory(
            0x0044711B,
            std::span<const uint8_t>({0x90, 0x90, 0x90, 0x90, 0x90, 0x90})));
    }
    else {
        CHECK_RETF(mem_api_->WriteMemory(
            0x0044711B,
            std::span<const uint8_t>({0x0F, 0x84, 0xA4, 0x00, 0x00, 0x00})));
    }
    return true;
}

bool Trainer::UnlimitRadar() const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    CHECK_RETF(IsGaming());
    uint32_t addr;
    CHECK_RETF(mem_api_->ReadAddress(0x00A8B230, {0x34A4}, &addr));
    return mem_api_->WriteMemory(addr, static_cast<uint8_t>(1));
}

bool Trainer::UnlimitSuperWeapon() const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    CHECK_RETF(IsGaming());
    // 00 NuclearMissile
    // 01 IronCurtain
    // 02 ForceShield
    // 03 LightningStorm
    // 04 PsychicDominator
    // 05 Chronosphere
    // 06 Reinforcements
    // 07 SpyPlane
    // 08 GeneticMutator
    // 09 PsychicReveal
    // Seems 12 in total...
    uint32_t addr1;
    CHECK_RETF(mem_api_->ReadMemory(0x00A83D4C, {0x258}, &addr1));
    for (uint32_t i = 0; i < 12; i++) {
        uint32_t addr2;
        CHECK_RETF(mem_api_->ReadMemory(addr1 + i * 4, &addr2));
        int32_t dis;
        CHECK_RETF(mem_api_->ReadMemory(addr2 + 0x78, &dis));
        if (dis != -1) {
            // Has the weapon
            CHECK_RETF(mem_api_->WriteMemory(addr2 + 0x6F,
                                             static_cast<uint8_t>(1)));
        }
    }
    return true;
}

#undef CHECK_MEMAPI_OR_RETURN_FALSE
#undef CHECK_RETF

// Disable the compiler attaching runtime check functions which is whithin the
// trainer. Asmxxx functions are executed in the game process.
#pragma runtime_checks("", off)
DWORD Trainer::AsmClearShroud(LPVOID) {
    _asm {
        pushad
        mov eax,0x00A83D4C
        mov edx,[eax]
        mov ecx,0x0087F7E8
        push edx
        mov eax,0x00577D90
        call eax
        popad                      
    }
    return 0;
}

DWORD Trainer::AsmNuclearBomb(LPVOID) {
    int eax_temp;
    __asm {
        pushad
        mov ecx,0x00A83D4C    // check if there's a launch tower
        mov ecx,[ecx]
        add ecx,0x258
        mov ecx,[ecx]
        mov ecx,[ecx]
        add ecx,0x78
        mov ecx,[ecx]
        cmp ecx,0xFFFFFFFF    // pannel = -1 => no 0~54 => yes
        jne exit1

        mov ecx,0x14
        mov eax_temp,0x006CEEB0
        call eax_temp
        mov ebx,eax
        // adjust cd
        add ebx,0x98
        mov eax,[ebx]   // weapon id
        mov edx,0x00A83D4C
        mov edx,[edx]
        add edx,0x258
        mov ecx,[edx]
        push 0          // initial state 1=idle
        push 0          // corresponding building
        mov ecx,[ecx+eax*4]
        push 1          //1=one shot, nuclear bomb has to be 1, others could be 0
        mov eax_temp,0x006CB560
        call eax_temp
        // add nuclear bomb pannel option
        add ebx,0x98
        mov eax,[ebx]   // weapon id
        mov ecx,0x0087F7E8
        push eax
        push 0x1F
        mov eax_temp,0x006A6300
        call eax_temp

        exit1:
        popad             
    }
    return 0;
}

DWORD Trainer::AsmUnitLevelUp(LPVOID) {
    __asm {
        pushad
        mov eax,0x00A8ECBC
        mov ebx,[eax]    // the first selecting unit address
        mov eax,0

        process:
        mov ecx,0x00A8ECC8
        cmp eax,[ecx]  // check if selecting anything
        jge brek
        mov ecx,[ebx+eax*4]  // unit address
        mov edx,0x40000000        // float 2.0
        add ecx,0x150
        mov [ecx],edx  // unit level
        add eax,1
        jmp process

        brek:
        popad
    }
    return 0;
}

DWORD Trainer::AsmUnitSpeedUp(LPVOID) {
    __asm {
        pushad
        mov eax,0x00A8ECBC
        mov ebx,[eax]    // the first selecting unit address
        mov eax,0

        process:
        mov ecx,0x00A8ECC8
        cmp eax,[ecx]  // check if selecting anything
        jge brek
        mov ecx,[ebx+eax*4]  // unit address
        mov edx,0x007E3EBC
        cmp [ecx],edx    // check if type is building
        je brek            // skip buildings
        mov edx,0x007EB058
        cmp [ecx],edx    // infantry max 2.0
        jne next1
        mov edx,0x40000000
        add ecx,0x584
        mov [ecx],edx
        je loopcontinue
        next1:
        mov edx,0x007F5C70
        cmp [ecx],edx    // vehicles, ships
        jne next2
        mov edx,0x40A00000
        add ecx,0x584
        mov [ecx],edx    // 5.0
        je loopcontinue
        next2:
        // not working for aircraftss
        // mov [ecx+584],3FF00000 //1.875
        loopcontinue:
        add eax,1
        jmp process

        brek:
        popad
    }
    return 0;
}

DWORD Trainer::AsmThisIsMine(LPVOID) {
    __asm {
        pushad
        mov eax,0x00A8ECC8
        mov eax,[eax]
        cmp eax,0        // check if selecting anything
        je exit1
        push 0
        mov ebx,0x00A83D4C
        mov eax,[ebx]
        push eax
        mov ebx,0x00A8ECBC
        mov eax,[ebx]
        mov ecx,[eax]
        mov ebx,[ecx]
        add ebx,0x3D4
        mov ebx,[ebx]
        call ebx
        exit1:
        popad
    }
    return 0;
}

DWORD Trainer::AsmDeleteUnit(LPVOID) {
    __asm {
        pushad
        mov eax,0x00A8ECC8
        mov eax,[eax]
        cmp eax,0        // check if selecting anything
        je exit1

        mov eax,0x00A8ECBC
        mov eax,[eax]
        mov ecx,[eax]
        mov eax,[ecx]
        add eax,0xF8
        mov ebx,[eax]
        call ebx

        exit1:
        popad
    }
    return 0;
}
#pragma runtime_checks("", restore)

}  // namespace yrtr
