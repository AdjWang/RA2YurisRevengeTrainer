#pragma once
#include "macro.h"
#include "vendor.h"

namespace yrtr {

class ImGuiContext;

namespace win32{
class MemoryAPI;
}

class TrainerFunc {
public:
    static TrainerFunc* instance() { return trainer_target_.get(); }
    static void Init(std::string_view exe_name, ImGuiContext& gui_ctx);

    bool attached() const { return attached_; }
    win32::MemoryAPI* mem_api() { return mem_api_.get(); }
    void Update();

    void OnBtnApply();
    void OnBtnFastBuild();
    void OnBtnDeleteUnit();
    void OnBtnClearShroud();
    void OnBtnGiveMeABomb();
    void OnBtnUnitLevelUp();
    void OnBtnUnitSpeedUp();
    void OnBtnIAMWinner();
    void OnBtnThisIsMine();
    void OnBtnIAMGhost();

    void OnCkboxGod(bool activate);
    void OnCkboxInstBuild(bool activate);
    void OnCkboxUnlimitSuperWeapon(bool activate);
    void OnCkboxUnlimitRadar(bool activate);
    void OnCkboxInstShot(bool activate);
    void OnCkboxInstTurn(bool activate);
    void OnCkboxRangeToYourBase(bool activate);
    void OnCkboxFireToYourBase(bool activate);
    void OnCkboxFreezeGapGenerator(bool activate);
    void OnCkboxFreezeUnit(bool activate);
    void OnCkboxSellTheWorld(bool activate);
    void OnCkboxUnlimitPower(bool activate);
    void OnCkboxBuildEveryWhere(bool activate);
    void OnCkboxAutoRepaire(bool activate);
    void OnCkboxEnermyRepaireDown(bool activate);
    void OnCkboxSocialismTheBest(bool activate);
    void OnCkboxMakeAttackedMine(bool activate);
    void OnCkboxMakeOccupiedMine(bool activate);
    void OnCkboxMakeGarrisonedMine(bool activate);
    void OnCkboxInvadeMode(bool activate);
    void OnCkboxUnlimitTech(bool activate);
    void OnCkboxInstLoad(bool activate);
    void OnCkboxUnlimitFirePower(bool activate);
    void OnCkboxInstChrono(bool activate);
    void OnCkboxSpySpy(bool activate);
    void OnCkboxInfantrySlip(bool activate);
    void OnCkboxUnitLeveledUp(bool activate);
    void OnCkboxAdjustGameSpeed(bool activate);

   private:
    static std::unique_ptr<TrainerFunc> trainer_target_;

    std::string exe_name_;
    bool attached_;
    DWORD pid_;
    std::unique_ptr<win32::MemoryAPI> mem_api_;

    TrainerFunc(std::string_view exe_name);
    TrainerFunc(TrainerFunc&&) = delete;
    TrainerFunc& operator=(TrainerFunc&&) = delete;

    bool WriteSpeed(uint32_t speed) const;
    bool WriteCredit(uint32_t credit) const;
    bool WriteMaxBuildingCount(uint32_t val) const;
    bool UnlimitRadar() const;
    bool UnlimitSuperWeapon() const;

    void AsmClearShroud() const;
    void AsmNuclearBomb() const;
    void AsmUnitLevelUp() const;
    void AsmUnitSpeedUp() const;
    void AsmThisIsMine() const;
    void AsmDeleteUnit() const;

    DISALLOW_COPY_AND_ASSIGN(TrainerFunc);
};

}
