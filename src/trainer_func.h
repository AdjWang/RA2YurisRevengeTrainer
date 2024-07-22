#pragma once
#include "macro.h"
#include "vendor.h"

namespace yrtr {

class GuiContext;

namespace win32{
class MemoryAPI;
}

class TrainerFunc {
public:
    static TrainerFunc* instance() { return trainer_target_.get(); }
    static void Init(std::string_view exe_name, GuiContext& gui_ctx);

    bool attached() const { return attached_; }
    win32::MemoryAPI* mem_api() { return mem_api_.get(); }
    void UpdateProcState();

    bool OnBtnApply(GuiContext* gui_ctx, uint32_t val);
    bool OnBtnFastBuild(GuiContext* gui_ctx);
    bool OnBtnDeleteUnit(GuiContext* gui_ctx);
    bool OnBtnClearShroud(GuiContext* gui_ctx);
    bool OnBtnGiveMeABomb(GuiContext* gui_ctx);
    bool OnBtnUnitLevelUp(GuiContext* gui_ctx);
    bool OnBtnUnitSpeedUp(GuiContext* gui_ctx);
    bool OnBtnIAMWinner(GuiContext* gui_ctx);
    bool OnBtnThisIsMine(GuiContext* gui_ctx);
    bool OnBtnIAMGhost(GuiContext* gui_ctx);

    bool OnCkboxGod(GuiContext* gui_ctx, bool activate);
    bool OnCkboxInstBuild(GuiContext* gui_ctx, bool activate);
    bool OnCkboxUnlimitSuperWeapon(GuiContext* gui_ctx, bool activate);
    bool OnCkboxUnlimitRadar(GuiContext* gui_ctx, bool activate);
    bool OnCkboxInstFire(GuiContext* gui_ctx, bool activate);
    bool OnCkboxInstTurn(GuiContext* gui_ctx, bool activate);
    bool OnCkboxRangeToYourBase(GuiContext* gui_ctx, bool activate);
    bool OnCkboxFireToYourBase(GuiContext* gui_ctx, bool activate);
    bool OnCkboxFreezeGapGenerator(GuiContext* gui_ctx, bool activate);
    bool OnCkboxFreezeUnit(GuiContext* gui_ctx, bool activate);
    bool OnCkboxSellTheWorld(GuiContext* gui_ctx, bool activate);
    bool OnCkboxUnlimitPower(GuiContext* gui_ctx, bool activate);
    bool OnCkboxBuildEveryWhere(GuiContext* gui_ctx, bool activate);
    bool OnCkboxAutoRepair(GuiContext* gui_ctx, bool activate);
    bool OnCkboxEnermyRevertRepair(GuiContext* gui_ctx, bool activate);
    bool OnCkboxSocialismTheBest(GuiContext* gui_ctx, bool activate);
    bool OnCkboxMakeAttackedMine(GuiContext* gui_ctx, bool activate);
    bool OnCkboxMakeCapturedMine(GuiContext* gui_ctx, bool activate);
    bool OnCkboxMakeGarrisonedMine(GuiContext* gui_ctx, bool activate);
    bool OnCkboxInvadeMode(GuiContext* gui_ctx, bool activate);
    bool OnCkboxUnlimitTech(GuiContext* gui_ctx, bool activate);
    bool OnCkboxFastReload(GuiContext* gui_ctx, bool activate);
    bool OnCkboxUnlimitFirePower(GuiContext* gui_ctx, bool activate);
    bool OnCkboxInstChrono(GuiContext* gui_ctx, bool activate);
    bool OnCkboxSpySpy(GuiContext* gui_ctx, bool activate);
    bool OnCkboxInfantrySlip(GuiContext* gui_ctx, bool activate);
    bool OnCkboxUnitLeveledUp(GuiContext* gui_ctx, bool activate);
    bool OnCkboxAdjustGameSpeed(GuiContext* gui_ctx, bool activate);

    bool UnlimitRadar() const;
    bool UnlimitSuperWeapon() const;

private:
    static std::unique_ptr<TrainerFunc> trainer_target_;

    std::string exe_name_;
    bool attached_;
    DWORD pid_;
    std::unique_ptr<win32::MemoryAPI> mem_api_;

    bool enable_unlimit_radar_;
    bool enable_unlimit_super_weapon_;

    TrainerFunc(std::string_view exe_name);
    TrainerFunc(TrainerFunc&&) = delete;
    TrainerFunc& operator=(TrainerFunc&&) = delete;

    bool IsGaming() const;
    bool WriteSpeed(uint32_t speed) const;
    bool WriteCredit(uint32_t credit) const;
    bool WriteMaxBuildingCount(uint32_t val) const;
    bool TobeGhost() const;
    bool AntiChronoDisable(bool activate) const;
    bool InvadeMode(bool activate) const;
    bool EnableSellAllBelong(bool activate) const;
    bool EnableSellAllBuilder(bool activate) const;

    static DWORD AsmClearShroud(LPVOID);
    static DWORD AsmNuclearBomb(LPVOID);
    static DWORD AsmUnitLevelUp(LPVOID);
    static DWORD AsmUnitSpeedUp(LPVOID);
    static DWORD AsmThisIsMine(LPVOID);
    static DWORD AsmDeleteUnit(LPVOID);

    DISALLOW_COPY_AND_ASSIGN(TrainerFunc);
};

}  // namespace yrtr
