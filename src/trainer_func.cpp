#include "logging.h"
#include "gui.h"
#include "trainer_func.h"
#include "win32utils.h"
#include "ce_scripts.h"

namespace yrtr {

std::unique_ptr<TrainerFunc> TrainerFunc::trainer_target_(nullptr);

namespace {
#define BIND_FN(fn) std::bind_front(&TrainerFunc::##fn, TrainerFunc::instance())
static void InitButtons(ImGuiContext& ctx) {
    ctx.AddButtonListener(FnLabel::kApply, BIND_FN(OnBtnApply));
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

static void InitCheckboxes(ImGuiContext& ctx) {
    ctx.AddCheckboxListener(FnLabel::kGod, BIND_FN(OnCkboxGod));
    ctx.AddCheckboxListener(FnLabel::kInstBuild, BIND_FN(OnCkboxInstBuild));
    ctx.AddCheckboxListener(FnLabel::kUnlimitSuperWeapon, BIND_FN(OnCkboxUnlimitSuperWeapon));
    ctx.AddCheckboxListener(FnLabel::kUnlimitRadar, BIND_FN(OnCkboxUnlimitRadar));
    ctx.AddCheckboxListener(FnLabel::kInstShot, BIND_FN(OnCkboxInstShot));
    ctx.AddCheckboxListener(FnLabel::kInstTurn, BIND_FN(OnCkboxInstTurn));
    ctx.AddCheckboxListener(FnLabel::kRangeToYourBase, BIND_FN(OnCkboxRangeToYourBase));
    ctx.AddCheckboxListener(FnLabel::kFireToYourBase, BIND_FN(OnCkboxFireToYourBase));
    ctx.AddCheckboxListener(FnLabel::kFreezeGapGenerator, BIND_FN(OnCkboxFreezeGapGenerator));
    ctx.AddCheckboxListener(FnLabel::kFreezeUnit, BIND_FN(OnCkboxFreezeUnit));
    ctx.AddCheckboxListener(FnLabel::kSellTheWorld, BIND_FN(OnCkboxSellTheWorld));
    ctx.AddCheckboxListener(FnLabel::kUnlimitPower, BIND_FN(OnCkboxUnlimitPower));
    ctx.AddCheckboxListener(FnLabel::kBuildEveryWhere, BIND_FN(OnCkboxBuildEveryWhere));
    ctx.AddCheckboxListener(FnLabel::kAutoRepaire, BIND_FN(OnCkboxAutoRepaire));
    ctx.AddCheckboxListener(FnLabel::kEnermyRepaireDown, BIND_FN(OnCkboxEnermyRepaireDown));
    ctx.AddCheckboxListener(FnLabel::kSocialismTheBest, BIND_FN(OnCkboxSocialismTheBest));
    ctx.AddCheckboxListener(FnLabel::kMakeAttackedMine, BIND_FN(OnCkboxMakeAttackedMine));
    ctx.AddCheckboxListener(FnLabel::kMakeOccupiedMine, BIND_FN(OnCkboxMakeOccupiedMine));
    ctx.AddCheckboxListener(FnLabel::kMakeGarrisonedMine, BIND_FN(OnCkboxMakeGarrisonedMine));
    ctx.AddCheckboxListener(FnLabel::kInvadeMode, BIND_FN(OnCkboxInvadeMode));
    ctx.AddCheckboxListener(FnLabel::kUnlimitTech, BIND_FN(OnCkboxUnlimitTech));
    ctx.AddCheckboxListener(FnLabel::kInstLoad, BIND_FN(OnCkboxInstLoad));
    ctx.AddCheckboxListener(FnLabel::kUnlimitFirePower, BIND_FN(OnCkboxUnlimitFirePower));
    ctx.AddCheckboxListener(FnLabel::kInstChrono, BIND_FN(OnCkboxInstChrono));
    ctx.AddCheckboxListener(FnLabel::kSpySpy, BIND_FN(OnCkboxSpySpy));
    ctx.AddCheckboxListener(FnLabel::kInfantrySlip, BIND_FN(OnCkboxInfantrySlip));
    ctx.AddCheckboxListener(FnLabel::kUnitLeveledUp, BIND_FN(OnCkboxUnitLeveledUp));
    ctx.AddCheckboxListener(FnLabel::kAdjustGameSpeed, BIND_FN(OnCkboxAdjustGameSpeed));
}
#undef BIND_FN
}  // namespace

void TrainerFunc::Init(std::string_view exe_name, ImGuiContext& gui_ctx) {
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
            mem_api_.reset(new win32::MemoryAPI(pid_));
            if (mem_api_->CheckHandle()) {
                attached_ = true;
            }
        }
    } else {
        // Not found target process
        DLOG(INFO, "GetProcessIDFromName failed to get pid of exe={}",
             exe_name_);
        mem_api_.reset();
        attached_ = false;
    }
}

#define CHECK_MEMAPI_OR_REPORT()                \
    if (mem_api_ == nullptr) {                  \
        LOG(WARN, "MemoryAPI not initialized"); \
        return;                                 \
    }

#define CHECK_REPORT(exp)                     \
    if (!(exp)) {                             \
        LOG(WARN, "Failed to execute " #exp); \
        return;                               \
    }

void TrainerFunc::OnBtnApply() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

void TrainerFunc::OnBtnFastBuild() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    uint32_t data;
    CHECK_REPORT(mem_api_->ReadMemory(0x00A83D4C, &data));
    for (uint32_t i = 1, j = 0; i <= 5; j += 4, i++) {
        CHECK_REPORT(mem_api_->WriteMemory(data + j + 0x5378, 15));
    }
}

void TrainerFunc::OnBtnDeleteUnit() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

void TrainerFunc::OnBtnClearShroud() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

void TrainerFunc::OnBtnGiveMeABomb() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

void TrainerFunc::OnBtnUnitLevelUp() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

void TrainerFunc::OnBtnUnitSpeedUp() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

void TrainerFunc::OnBtnIAMWinner() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);
    CHECK_MEMAPI_OR_REPORT();
    CHECK_REPORT(mem_api_->WriteMemory(0x00A83D49, static_cast<uint8_t>(1)));
}

void TrainerFunc::OnBtnThisIsMine() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

void TrainerFunc::OnBtnIAMGhost() {
    DLOG(INFO, "Trigger {}", __FUNCTION__);

}

void TrainerFunc::OnCkboxGod(bool activate) {
    DLOG(INFO, "Trigger {}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxInstBuild(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxUnlimitSuperWeapon(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxUnlimitRadar(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxInstShot(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxInstTurn(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxRangeToYourBase(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxFireToYourBase(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxFreezeGapGenerator(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxFreezeUnit(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxSellTheWorld(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxUnlimitPower(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxBuildEveryWhere(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxAutoRepaire(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxEnermyRepaireDown(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxSocialismTheBest(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxMakeAttackedMine(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxMakeOccupiedMine(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxMakeGarrisonedMine(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxInvadeMode(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxUnlimitTech(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxInstLoad(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxUnlimitFirePower(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxInstChrono(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxSpySpy(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxInfantrySlip(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxUnitLeveledUp(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

void TrainerFunc::OnCkboxAdjustGameSpeed(bool activate) {
    DLOG(INFO, "Trigger {} activate={}", __FUNCTION__, activate);

}

#undef CHECK_MEMAPI_OR_REPORT
#undef CHECK_REPORT

#define CHECK_MEMAPI_OR_RETURN_FALSE() \
    if (mem_api_ == nullptr) {         \
        return false;                  \
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

#undef CHECK_MEMAPI_OR_RETURN_FALSE
#undef CHECK_RETF

void TrainerFunc::AsmClearShroud() const {
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
}

void TrainerFunc::AsmNuclearBomb() const {
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
}

void TrainerFunc::AsmUnitLevelUp() const {
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
}

void TrainerFunc::AsmUnitSpeedUp() const {
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
        // not working for planes
        // mov [ecx+584],3FF00000 //1.875
        loopcontinue:
        add eax,1
        jmp process

        brek:
        popad
    }
}

void TrainerFunc::AsmThisIsMine() const {
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
}

void TrainerFunc::AsmDeleteUnit() const {
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
}

}  // namespace yrtr
