#include "logging.h"
#include "gui.h"
#include "trainer_func.h"
#include "win32utils.h"
#include "ce_scripts.h"

namespace yrtr {

std::unique_ptr<TrainerFunc> TrainerFunc::trainer_target_(nullptr);

namespace {
#define BIND_FN(fn) std::bind_front(&TrainerFunc::##fn, TrainerFunc::instance())
static void InitButtons(GuiContext& ctx) {
    ctx.AddInputListener(FnLabel::kApply, BIND_FN(OnBtnApply));
    ctx.AddButtonListener(FnLabel::kFastBuild, BIND_FN(OnBtnFastBuild));
    ctx.AddButtonListener(FnLabel::kDeleteUnit, BIND_FN(OnBtnDeleteUnit));
    ctx.AddButtonListener(FnLabel::kClearShroud, BIND_FN(OnBtnClearShroud));
    ctx.AddButtonListener(FnLabel::kGiveMeABomb, BIND_FN(OnBtnGiveMeABomb));
    ctx.AddButtonListener(FnLabel::kUnitLevelUp, BIND_FN(OnBtnUnitLevelUp));
    ctx.AddButtonListener(FnLabel::kUnitSpeedUp, BIND_FN(OnBtnUnitSpeedUp));
    ctx.AddButtonListener(FnLabel::kIAMWinner, BIND_FN(OnBtnIAMWinner));
    ctx.AddButtonListener(FnLabel::kThisIsMine, BIND_FN(OnBtnThisIsMine));
    ctx.AddButtonListener(FnLabel::kIAMGhost, BIND_FN(OnBtnIAMGhost));
}

static void InitCheckboxes(GuiContext& ctx) {
    ctx.AddCheckboxListener(FnLabel::kGod, BIND_FN(OnCkboxGod));
    ctx.AddCheckboxListener(FnLabel::kInstBuild, BIND_FN(OnCkboxInstBuild));
    ctx.AddCheckboxListener(FnLabel::kUnlimitSuperWeapon, BIND_FN(OnCkboxUnlimitSuperWeapon));
    ctx.AddCheckboxListener(FnLabel::kUnlimitRadar, BIND_FN(OnCkboxUnlimitRadar));
    ctx.AddCheckboxListener(FnLabel::kInstFire, BIND_FN(OnCkboxInstFire));
    ctx.AddCheckboxListener(FnLabel::kInstTurn, BIND_FN(OnCkboxInstTurn));
    ctx.AddCheckboxListener(FnLabel::kRangeToYourBase, BIND_FN(OnCkboxRangeToYourBase));
    ctx.AddCheckboxListener(FnLabel::kFireToYourBase, BIND_FN(OnCkboxFireToYourBase));
    ctx.DisableCheckbox(FnLabel::kFireToYourBase);
    ctx.AddCheckboxListener(FnLabel::kFreezeGapGenerator, BIND_FN(OnCkboxFreezeGapGenerator));
    ctx.AddCheckboxListener(FnLabel::kFreezeUnit, BIND_FN(OnCkboxFreezeUnit));
    ctx.AddCheckboxListener(FnLabel::kSellTheWorld, BIND_FN(OnCkboxSellTheWorld));
    ctx.AddCheckboxListener(FnLabel::kUnlimitPower, BIND_FN(OnCkboxUnlimitPower));
    ctx.AddCheckboxListener(FnLabel::kBuildEveryWhere, BIND_FN(OnCkboxBuildEveryWhere));
    ctx.AddCheckboxListener(FnLabel::kAutoRepair, BIND_FN(OnCkboxAutoRepair));
    ctx.AddCheckboxListener(FnLabel::kEnermyRevertRepair, BIND_FN(OnCkboxEnermyRevertRepair));
    ctx.AddCheckboxListener(FnLabel::kSocialismTheBest, BIND_FN(OnCkboxSocialismTheBest));
    ctx.AddCheckboxListener(FnLabel::kMakeAttackedMine, BIND_FN(OnCkboxMakeAttackedMine));
    ctx.AddCheckboxListener(FnLabel::kMakeCapturedMine, BIND_FN(OnCkboxMakeCapturedMine));
    ctx.AddCheckboxListener(FnLabel::kMakeGarrisonedMine, BIND_FN(OnCkboxMakeGarrisonedMine));
    ctx.AddCheckboxListener(FnLabel::kInvadeMode, BIND_FN(OnCkboxInvadeMode));
    ctx.AddCheckboxListener(FnLabel::kUnlimitTech, BIND_FN(OnCkboxUnlimitTech));
    ctx.AddCheckboxListener(FnLabel::kFastReload, BIND_FN(OnCkboxFastReload));
    ctx.AddCheckboxListener(FnLabel::kUnlimitFirePower, BIND_FN(OnCkboxUnlimitFirePower));
    ctx.AddCheckboxListener(FnLabel::kInstChrono, BIND_FN(OnCkboxInstChrono));
    ctx.AddCheckboxListener(FnLabel::kSpySpy, BIND_FN(OnCkboxSpySpy));
    ctx.AddCheckboxListener(FnLabel::kInfantrySlip, BIND_FN(OnCkboxInfantrySlip));
    ctx.AddCheckboxListener(FnLabel::kUnitLeveledUp, BIND_FN(OnCkboxUnitLeveledUp));
    ctx.AddCheckboxListener(FnLabel::kAdjustGameSpeed, BIND_FN(OnCkboxAdjustGameSpeed));
}
#undef BIND_FN
}  // namespace

void TrainerFunc::Init(std::string_view exe_name, GuiContext& gui_ctx) {
    trainer_target_.reset(new TrainerFunc(exe_name));
    InitButtons(gui_ctx);
    InitCheckboxes(gui_ctx);
}

TrainerFunc::TrainerFunc(std::string_view exe_name)
    : exe_name_(exe_name),
      attached_(false) {}

void TrainerFunc::Update() {
    if (win32::GetProcessIDFromName(exe_name_.c_str(), &pid_)) {
        if (mem_api_ == nullptr) {
            DLOG(INFO, "GetProcessIDFromName found exe={} pid={:#x}", exe_name_,
                 pid_);
            mem_api_.reset(new win32::MemoryAPI(pid_));
            if (mem_api_->CheckHandle()) {
                attached_ = true;
                DLOG(INFO, "MemoryAPI initialized");
            }
        }
    } else {
        // Not found target process
        DLOG(INFO, "GetProcessIDFromName searching exe={}", exe_name_);
        mem_api_.reset();
        attached_ = false;
    }
}

#define CHECK_MEMAPI_OR_REPORT()                \
    if (mem_api_ == nullptr) {                  \
        LOG(WARN, "MemoryAPI not initialized"); \
        return false;                           \
    }

#define CHECK_REPORT(exp)                                   \
    if (!(exp)) {                                           \
        LOG(WARN, __FUNCTION__ " Failed to execute " #exp); \
        return false;                                       \
    }

#pragma warning(push)
// Too simple to make mistakes, just ignore
// warning C4100: 'gui_ctx': unreferenced formal parameter
#pragma warning(disable:4100)
bool TrainerFunc::OnBtnApply(GuiContext* gui_ctx, uint32_t val) {
    DLOG(INFO, "Trigger {} val={}", __FUNCTION__, val);
    CHECK_REPORT(WriteCredit(val));
    return true;
}

bool TrainerFunc::OnBtnFastBuild(GuiContext* gui_ctx) {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    uint32_t data;
    CHECK_REPORT(mem_api_->ReadMemory(0x00A83D4C, &data));
    for (uint32_t i = 1, j = 0; i <= 5; j += 4, i++) {
        CHECK_REPORT(mem_api_->WriteMemory(data + j + 0x5378, 15));
    }
    return true;
}

bool TrainerFunc::OnBtnDeleteUnit(GuiContext* gui_ctx) {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(mem_api_->CreateRemoteThread(AsmDeleteUnit));
    return true;
}

bool TrainerFunc::OnBtnClearShroud(GuiContext* gui_ctx) {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(mem_api_->CreateRemoteThread(AsmClearShroud));
    return true;
}

bool TrainerFunc::OnBtnGiveMeABomb(GuiContext* gui_ctx) {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(mem_api_->CreateRemoteThread(AsmNuclearBomb));
    return true;
}

bool TrainerFunc::OnBtnUnitLevelUp(GuiContext* gui_ctx) {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(mem_api_->CreateRemoteThread(AsmUnitLevelUp));
    return true;
}

bool TrainerFunc::OnBtnUnitSpeedUp(GuiContext* gui_ctx) {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(mem_api_->CreateRemoteThread(AsmUnitSpeedUp));
    return true;
}

bool TrainerFunc::OnBtnIAMWinner(GuiContext* gui_ctx) {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(mem_api_->WriteMemory(0x00A83D49, static_cast<uint8_t>(1)));
    return true;
}

bool TrainerFunc::OnBtnThisIsMine(GuiContext* gui_ctx) {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(mem_api_->CreateRemoteThread(AsmThisIsMine));
    return true;
}

bool TrainerFunc::OnBtnIAMGhost(GuiContext* gui_ctx) {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(TobeGhost());
    return true;
}

// Use a macro instead of a function because fmt::format_string has to be a
// constexpr
#define FMT_AUTOASSEMBLE(script, activate)                                   \
    mem_api_->AutoAssemble(fmt::format(script, fmt::arg("proc", exe_name_)), \
                           activate)

bool TrainerFunc::OnCkboxGod(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kGodPlayer, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kAntiChrono, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kAntiChronoDisbuild, activate));
    CHECK_REPORT(AntiChronoDisable(activate));
    return true;
}

bool TrainerFunc::OnCkboxInstBuild(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInstBuild, activate));
    return true;
}

bool TrainerFunc::OnCkboxUnlimitSuperWeapon(GuiContext* gui_ctx,
                                            bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    // TODO
    return true;
}

bool TrainerFunc::OnCkboxUnlimitRadar(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    // TODO
    return true;
}

bool TrainerFunc::OnCkboxInstFire(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInstFire, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInstTurnBattery, activate));
    return true;
}

bool TrainerFunc::OnCkboxInstTurn(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInstTurnRound, activate));
    return true;
}

bool TrainerFunc::OnCkboxRangeToYourBase(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kRangeToYourBase, activate));
    if (activate) {
        gui_ctx->EnableCheckbox(FnLabel::kFireToYourBase);
    } else {
        gui_ctx->DisableCheckbox(FnLabel::kFireToYourBase);
    }
    return true;
}

bool TrainerFunc::OnCkboxFireToYourBase(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kFireToYourBase, activate));
    return true;
}

bool TrainerFunc::OnCkboxFreezeGapGenerator(GuiContext* gui_ctx,
                                            bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kFreezeGAGAP, activate));
    return true;
}

bool TrainerFunc::OnCkboxFreezeUnit(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kFreezeUnit, activate));
    return true;
}

bool TrainerFunc::OnCkboxSellTheWorld(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kSellTheWorldOnCursor, activate));
    CHECK_REPORT(EnableSellAllBelong(activate));
    CHECK_REPORT(EnableSellAllBuilder(activate));
    return true;
}

bool TrainerFunc::OnCkboxUnlimitPower(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kUnlimitPower, activate));
    return true;
}

bool TrainerFunc::OnCkboxBuildEveryWhere(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kBuildEveryWhereGround, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kBuildEveryWhereWater, activate));
    return true;
}

bool TrainerFunc::OnCkboxAutoRepair(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kAutoRepair, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kAutoRepairNeutral, activate));
    return true;
}

bool TrainerFunc::OnCkboxEnermyRevertRepair(GuiContext* gui_ctx,
                                            bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kRevertRepair, activate));
    return true;
}

bool TrainerFunc::OnCkboxSocialismTheBest(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kRevertYuri, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kPermanentYuri, activate));
    return true;
}

bool TrainerFunc::OnCkboxMakeAttackedMine(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kThisIsMineOnAttack, activate));
    return true;
}

bool TrainerFunc::OnCkboxMakeCapturedMine(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kThisIsMineOnCapture, activate));
    return true;
}

bool TrainerFunc::OnCkboxMakeGarrisonedMine(GuiContext* gui_ctx,
                                            bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kThisIsMineOnGarrison, activate));
    return true;
}

bool TrainerFunc::OnCkboxInvadeMode(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(InvadeMode(activate));
    return true;
}

bool TrainerFunc::OnCkboxUnlimitTech(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kUnlimitTech, activate));
    return true;
}

bool TrainerFunc::OnCkboxFastReload(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kFastReload, activate));
    return true;
}

bool TrainerFunc::OnCkboxUnlimitFirePower(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kUnlimitFirePower, activate));
    return true;
}

bool TrainerFunc::OnCkboxInstChrono(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInstChronoMove, activate));
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInstChronoAttack, activate));
    return true;
}

bool TrainerFunc::OnCkboxSpySpy(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kSpySpy, activate));
    return true;
}

bool TrainerFunc::OnCkboxInfantrySlip(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kInfantrySlip, activate));
    return true;
}

bool TrainerFunc::OnCkboxUnitLeveledUp(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(FMT_AUTOASSEMBLE(script::kUnitLeveledUp, activate));
    return true;
}

bool TrainerFunc::OnCkboxAdjustGameSpeed(GuiContext* gui_ctx, bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
    // TODO
    return true;
}
#pragma warning(pop)

#undef FMT_AUTOASSEMBLE
#undef CHECK_MEMAPI_OR_REPORT
#undef CHECK_REPORT

#define CHECK_MEMAPI_OR_RETURN_FALSE()          \
    if (mem_api_ == nullptr) {                  \
        LOG(WARN, "MemoryAPI not initialized"); \
        return false;                           \
    }

#define CHECK_RETF(exp) \
    if (!(exp)) {       \
        return false;   \
    }

bool TrainerFunc::WriteSpeed(uint32_t speed) const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    return mem_api_->WriteMemory(0x00A8EB60, speed);
}

bool TrainerFunc::WriteCredit(uint32_t credit) const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    uint32_t addr;
    CHECK_RETF(mem_api_->ReadAddress(0x00A83D4C, {0x30C}, &addr));
    return mem_api_->WriteMemory(addr, credit);
}

bool TrainerFunc::WriteMaxBuildingCount(uint32_t val) const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    uint32_t addr;
    CHECK_RETF(mem_api_->ReadAddress(0x008871E0, {0xF0}, &addr));
    return mem_api_->WriteMemory(addr, val - 1);
}

bool TrainerFunc::UnlimitRadar() const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
    uint32_t addr;
    CHECK_RETF(mem_api_->ReadAddress(0x00A8B230, {0x34A4}, &addr));
    return mem_api_->WriteMemory(addr, static_cast<uint8_t>(1));
}

bool TrainerFunc::UnlimitSuperWeapon() const {
    CHECK_MEMAPI_OR_RETURN_FALSE();
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

bool TrainerFunc::TobeGhost() const {
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
    if (select_owner != 0x007E3EBC) {
        return true;
    }
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
bool TrainerFunc::AntiChronoDisable(bool activate) const {
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

bool TrainerFunc::InvadeMode(bool activate) const {
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
bool TrainerFunc::EnableSellAllBelong(bool activate) const {
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
bool TrainerFunc::EnableSellAllBuilder(bool activate) const {
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

#undef CHECK_MEMAPI_OR_RETURN_FALSE
#undef CHECK_RETF

// Disable the compiler attaching runtime check functions which is whithin the
// trainer. Asmxxx functions are executed in the game process.
#pragma runtime_checks("", off)
DWORD TrainerFunc::AsmClearShroud(LPVOID) {
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

DWORD TrainerFunc::AsmNuclearBomb(LPVOID) {
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

DWORD TrainerFunc::AsmUnitLevelUp(LPVOID) {
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

DWORD TrainerFunc::AsmUnitSpeedUp(LPVOID) {
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

DWORD TrainerFunc::AsmThisIsMine(LPVOID) {
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

DWORD TrainerFunc::AsmDeleteUnit(LPVOID) {
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
