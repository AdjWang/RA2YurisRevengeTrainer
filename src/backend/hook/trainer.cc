#include "backend/hook/trainer.h"

#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "AbstractTypeClass.h"
#include "BuildingTypeClass.h"
#include "FactoryClass.h"
#include "GeneralDefinitions.h"
#include "Helpers/Cast.h"
#include "HouseClass.h"
#include "InfantryClass.h"
#include "MapClass.h"
#include "ObjectClass.h"
#include "SidebarClass.h"
#include "SpawnManagerClass.h"
#include "SuperClass.h"
#include "TechnoClass.h"
#include "Unsorted.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "backend/game/beacon.h"
#include "backend/hook/hook_point.h"
#include "backend/record.h"
#include "backend/tech.h"
#include "base/logging.h"

namespace yrtr {
namespace backend {
namespace hook {
SideMap Trainer::protected_houses_;
bool Trainer::activate_disable_gagap_ = false;

void BeepEnable() {
  std::thread t([]() {
    Beep(600, 100);
    Beep(1000, 100);
  });
  t.detach();
}

void BeepDisable() {
  std::thread t([]() {
    Beep(1000, 100);
    Beep(600, 100);
  });
  t.detach();
}

void InitStates(State& state) {
  state.ckbox_states.emplace(FnLabel::kGod,                CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kInstBuild,          CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kUnlimitSuperWeapon, CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kInstFire,           CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kInstTurn,           CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kRangeToYourBase,    CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kFireToYourBase,     CheckboxState{.enable=false, .activate=false});
  state.ckbox_states.emplace(FnLabel::kFreezeGapGenerator, CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kSellTheWorld,       CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kBuildEveryWhere,    CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kAutoRepair,         CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kSocialismMajesty,   CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kMakeGarrisonedMine, CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kInvadeMode,         CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kUnlimitTech,        CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kUnlimitFirePower,   CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kInstChrono,         CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kSpySpy,             CheckboxState{.enable=true, .activate=false});
  state.ckbox_states.emplace(FnLabel::kAdjustGameSpeed,    CheckboxState{.enable=true, .activate=false});
}

namespace {
static yrpp::HouseClass* FindHouseById(UniqId id) {
  for (int i = 0; i < yrpp::HouseClass::Array->Count; i++) {
    yrpp::HouseClass* obj = yrpp::HouseClass::Array->GetItem(i);
    if (obj->UniqueID == id) {
      return obj;
    }
  }
  return nullptr;
}

template <class T>
auto DynamicCast(yrpp::ObjectClass* obj) {
  return yrpp::abstract_cast<T>(obj);
}

static void __declspec(naked) __cdecl InjectSellTheWorldCursor() {
  static const uint32_t jmp_original = GetJumpBack(kHpSellTheWorldCursor);
  static const uint32_t jmp_allow_sell = 0x00692A20;
  static yrpp::TechnoClass* techno;
  __asm {
    mov [techno], esi
    pushad
  }
  if (techno->WhatAmI() == yrpp::AbstractType::Terrain) {
    __asm {
      popad
      mov eax, [esi]
      mov ecx, esi
      call dword ptr [eax + 0x3C]
      jmp [jmp_original]
    }
  } else {
    __asm {
      popad
      mov eax, 0x0C // yrpp::AbstractType::Factory
      jmp [jmp_allow_sell]
    }
  }
}

static void __declspec(naked) __cdecl InjectGodPlayer() {
  static const uint32_t jmp_original = GetJumpBack(kHpGodPlayer);
  static yrpp::ObjectClass* obj;
  static int damage = 0;
  __asm {
    mov [obj], esi
    mov [damage], edx
    pushad
  }
  if (Trainer::ShouldProtect(obj)) {
    damage = 0;
  }
  __asm {
    popad
    mov edx, [damage]
    sub eax, edx
    test eax, eax
    mov [esi + 0x6C], eax
    jmp [jmp_original]
  }
}

static void __declspec(naked) __cdecl InjectCanWrapTarget() {
  static const uint32_t jmp_original = GetJumpBack(kHpCanWrapTarget);
  static const uint32_t jmp_cannot_wrap = 0x0071AE5D;
  static yrpp::TechnoClass* target;
  __asm {
    mov [target], esi
    pushad
  }
  if (Trainer::ShouldProtect(target)) {
    __asm {
      popad
      jmp [jmp_cannot_wrap]
    }
  } else {
    __asm {
      popad
      mov eax, [esi]
      mov ecx, esi
      call dword ptr [eax + 0x2C]
      jmp [jmp_original]
    }
  }
}

static void __declspec(naked) __cdecl InjectInstFire() {
  static const uint32_t jmp_original = GetJumpBack(kHpInstFire);
  static yrpp::TechnoClass* target;
  __asm {
    mov [target], esi
    mov eax, [esi + 0x2F4]
    pushad
  }
  if (Trainer::ShouldProtect(target)) {
    __asm {
      popad
      mov eax, 0
      jmp [jmp_original]
    }
  } else {
    __asm {
      popad
      jmp [jmp_original]
    }
  }
}

static void __declspec(naked) __cdecl InjectRangeToYourBase() {
  static const uint32_t jmp_original = GetJumpBack(kHpRangeToYourBase);
  static yrpp::TechnoClass* target;
  __asm {
    mov [target], esi
    mov edi, [ebx + 0xB4]
    pushad
  }
  if (Trainer::ShouldProtect(target)) {
    __asm {
      popad
      mov edi, 0xF900
      jmp [jmp_original]
    }
  } else {
    __asm {
      popad
      jmp [jmp_original]
    }
  }
}

static void __declspec(naked) __cdecl InjectFireToYourBase() {
  static const uint32_t jmp_return = 0x00701399;
  static yrpp::TechnoClass* target;
  __asm {
    mov [target], esi
    pushad
  }
  if (Trainer::ShouldProtect(target)) {
    __asm {
      popad
      mov eax, 0xF900
    }
  } else {
    __asm {
      popad
      mov eax, ebx
    }
  }
  __asm {
    pop esi
    jmp [jmp_return]
  }
}

static bool IsGAGAP(yrpp::TechnoClass* target) {
  yrpp::BuildingClass* building = DynamicCast<yrpp::BuildingClass*>(target);
  if (building == nullptr) {
    return false;
  }
  return building->Type->GapGenerator;
}

static void __declspec(naked) __cdecl InjectFreezeGapGenerator() {
  static const uint32_t jmp_original = GetJumpBack(kHpFreezeGapGenerator);
  static yrpp::TechnoClass* target;
  __asm {
    mov [target], esi
    pushad
  }
  if (Trainer::is_active_disable_gagap() && IsGAGAP(target) &&
      !Trainer::ShouldProtect(target)) {
    __asm {
      popad
      mov eax, 2
      jmp [jmp_original]
    }
  } else {
    // Original code.
    __asm {
      popad
      mov eax, [esi + 0x504]  // EMPLockRemaining
      jmp [jmp_original]
    }
  }
}

static void __declspec(naked) __cdecl InjectBuildEveryWhereGround() {
  static const uint32_t jmp_return = 0x004A9063;
  __asm {
    mov eax, 1
    jmp [jmp_return]
  }
}

static void __declspec(naked) __cdecl InjectBuildEveryWhereWater() {
  static const uint32_t jmp_back = GetJumpBack(kHpBuildEveryWhereWater);
  static yrpp::HouseClass* house;
  __asm {
    mov ecx, [esp + 0x24]
    mov [house], ecx
    pushad
  }
  // CellClass::CanThisExistHere_47C620, sometimes checking objects does not
  // have a house, e.g., terrains.
  if (house != nullptr && Trainer::ShouldProtect(house)) {
    __asm {
      popad
      mov ecx, 3
    }
  } else {
    __asm {
      popad
      mov ecx, [esp + 0x1C]
    }
  }
  __asm {
    cmp ecx, -1
    jmp [jmp_back]
  }
}

static void __declspec(naked) __cdecl InjectAutoRepairNeutral() {
  static const uint32_t jmp_back = GetJumpBack(kHpAutoRepairNeutral);
  static yrpp::BuildingClass* building;
  __asm {
    mov [building], esi
    pushad
  }
  if (Trainer::ShouldProtect(building)) {
    __asm {
      popad
      mov cl, 1
    }
  } else {
    __asm {
      popad
      mov cl, [eax + 0x157A]
    }
  }
  __asm {
    jmp [jmp_back]
  }
}

static void __declspec(naked) __cdecl InjectCapturedMine() {
  static const uint32_t jmp_back = GetJumpBack(kHpCapturedMine);
  static yrpp::BuildingClass* building;
  static yrpp::HouseClass* player;
  __asm {
    mov [building], edi
    pushad
  }
  if (Trainer::ShouldProtect(building)) {
    player = yrpp::HouseClass::CurrentPlayer;
    __asm {
      popad
      mov eax, [player]
    }
  } else {
    __asm {
      popad
      mov eax, [esi + 0x21C]
    }
  }
  __asm {
    jmp [jmp_back]
  }
}

static void __declspec(naked) __cdecl InjectSocialismMajestyCome() {
  static const uint32_t jmp_back = GetJumpBack(kHpSocialismMajestyCome);
  static const uint32_t jmp_return = 0x004692D5;
  static yrpp::TechnoClass* attacker;
  static yrpp::TechnoClass* target;
  __asm {
    mov eax, [esi + 0xB0]
    mov [attacker], eax
    mov eax, [esi + 0x10C]
    mov [target], eax
    pushad
  }
  if (Trainer::ShouldProtect(target)) {
    attacker->SetOwningHouse(yrpp::HouseClass::CurrentPlayer);
    __asm {
      popad
      mov al, 1
      jmp [jmp_return]
    }
  } else {
    // Original code.
    __asm {
      popad
      mov ecx, [esi + 0xB0]
      jmp [jmp_back]
    }
  }
}

static void __declspec(naked) __cdecl InjectSocialismMajestyBack() {
  static const uint32_t jmp_back = GetJumpBack(kHpSocialismMajestyBack);
  static const uint32_t jmp_return = 0x00472000;
  static yrpp::TechnoClass* attacker;
  __asm {
    mov eax, [ecx + 0xB0]
    mov [attacker], eax
    sub esp, 0x18
    push esi
    mov esi, [esp + 0x20]
    pushad
  }
  if (attacker != nullptr && Trainer::ShouldProtect(attacker)) {
    __asm {
      popad
      jmp [jmp_return]
    }
  } else {
    // Original code.
    __asm {
      popad
      jmp [jmp_back]
    }
  }
}

static void __declspec(naked) __cdecl InjectGarrisonedMine() {
  static const uint32_t jmp_back = GetJumpBack(kHpGarrisonedMine);
  static yrpp::HouseClass* player;
  __asm {
    pushad
  }
  player = yrpp::HouseClass::CurrentPlayer;
  __asm {
    popad
    mov eax, [player]
    jmp [jmp_back]
  }
}

static void __declspec(naked) __cdecl InjectUnlimitTech() {
  static yrpp::BuildingTypeClass* tech;
  static int should_enable;
  __asm {
    mov eax, [esp + 0x4]
    mov [tech], eax
    pushad
  }
  should_enable = static_cast<int>(Trainer::ShouldEnableTech(tech));
  __asm {
    popad
    mov eax, [should_enable]
    ret 0x0C
  }
}

static void __declspec(naked) __cdecl InjectFastReload() {
  static const uint32_t jmp_back = GetJumpBack(kHpFastReload);
  static yrpp::SpawnManagerClass* spawn_manager;
  __asm {
    mov [spawn_manager], esi
    pushad
  }
  if(Trainer::ShouldProtect(spawn_manager)) {
    __asm {
      popad
      mov edx, 2
    }
  } else {
    __asm {
      popad
      mov edx, [esi + 0x30]
    }
  }
  __asm {
    mov esi, [esi + 0x3C]
    jmp [jmp_back]
  }
}

static void __declspec(naked) __cdecl InjectMoreAmmunition() {
  static const uint32_t jmp_back = GetJumpBack(kHpMoreAmmunition);
  static yrpp::SpawnManagerClass* spawn_manager;
  __asm {
    mov [spawn_manager], esi
    pushad
  }
  if(Trainer::ShouldProtect(spawn_manager)) {
    __asm {
      popad
      mov [esi + 0x2C], 0xF
    }
  } else {
    __asm {
      popad
      mov [esi + 0x2C], eax
    }
  }
  __asm {
    mov [esi + 0x30], ecx
    jmp [jmp_back]
  }
}

static void __declspec(naked) __cdecl InjectInstChronoMove() {
  static const uint32_t jmp_back = GetJumpBack(kHpInstChronoMove);
  static yrpp::FootClass* unit;
  __asm {
    mov [unit], ecx
    pushad
  }
  if(Trainer::ShouldProtect(unit)) {
    unit->WarpingOut = false;
  } else {
    unit->WarpingOut = true;
  }
  __asm {
    popad
    jmp [jmp_back]
  }
}

static void __declspec(naked) __cdecl InjectInstChronoAttack() {
  static const uint32_t jmp_back = GetJumpBack(kHpInstChronoAttack);
  static yrpp::TemporalClass* temporal;
  static int wrap_remaining;
  __asm {
    mov [temporal], esi
    mov [wrap_remaining], eax
    pushad
  }
  if(Trainer::ShouldProtect(temporal)) {
    // Don't worry, chrono legionnaire attacking is suppressed in god mode.
    temporal->WarpRemaining = wrap_remaining;
  } else {
    temporal->WarpRemaining = 0;
  }
  __asm {
    popad
    test ecx, ecx
    jmp [jmp_back]
  }
}

static void __declspec(naked) __cdecl InjectSpySpy() {
  static const uint32_t jmp_back = GetJumpBack(kHpSpySpy);
  static yrpp::BuildingClass* invaded;
  static yrpp::HouseClass* player;
  __asm {
    mov [invaded], edi
    pushad
  }
  if(Trainer::ShouldProtect(invaded)) {
    player = yrpp::HouseClass::CurrentPlayer;
    __asm {
      popad
      mov eax, [player]
      jmp [jmp_back]
    }
  } else {
    __asm {
      popad
      mov eax, [esi + 0x21C]
      jmp [jmp_back]
    }
  }
}
}  // namespace

#define CHECK_MEMAPI_OR_REPORT()                                               \
  if (mem_api_ == nullptr) {                                                   \
    LOG_F(WARNING, "MemoryAPI not initialized. Ensure the game has started."); \
    return;                                                                    \
  }

#define CHECK_REPORT(exp)                                    \
  if (!(exp)) {                                              \
    LOG_F(WARNING, __FUNCTION__ " Failed to execute " #exp); \
    return;                                                  \
  }

Trainer::Trainer(Config* cfg)
    : cfg_(cfg),
      activate_inst_building_(false),
      activate_inst_superweapon_(false),
      activate_inst_turn_turret_(false),
      activate_inst_turn_body_(false),
      activate_auto_clean_beacon_(false),
      on_state_updated_(nullptr),
      state_dirty_(true) {
  DCHECK_NOTNULL(cfg_);
  activate_auto_clean_beacon_ = cfg_->auto_clean_beacon();
  mem_api_ = std::make_unique<MemoryAPI>();
  InitStates(state_);
  if (cfg_->auto_record()) {
    ReadCheckboxStateFromToml(cfg_->record_path(), /*out*/ state_.ckbox_states);
  }
}

Trainer::~Trainer() {
  if (cfg_->auto_record()) {
    WriteCheckboxStateToToml(state_.ckbox_states, cfg_->record_path());
  }
}

bool Trainer::ShouldProtect(yrpp::AbstractClass* obj) {
  DCHECK(IsWithinGameLoopThread());
  CHECK_NOTNULL(obj);
  if (obj->AbstractFlags == yrpp::AbstractFlags::None) {
    return false;
  }
  bool result = false;
  ForeachProtectedHouse([&](yrpp::HouseClass* house) {
    result |= house == obj->GetOwningHouse();
  });
  return result;
}

bool Trainer::ShouldProtect(yrpp::HouseClass* house) {
  DCHECK(IsWithinGameLoopThread());
  CHECK_NOTNULL(house);
  bool result = false;
  ForeachProtectedHouse([&](yrpp::HouseClass* protected_house) {
    result |= house == protected_house;
  });
  return result;
}

bool Trainer::ShouldEnableTech(yrpp::BuildingTypeClass* tech) {
  std::string_view key(tech->ID);
  Tech tech_key = GetTech(key);
  if (tech_key == Tech::kUnknown) {
    return false;
  }
  DCHECK_NOTNULL(Config::instance());
  const TechList& tech_list = Config::instance()->tech_list();
  DCHECK_LT_F(static_cast<uint32_t>(tech_key), tech_list.size(), "key={}", key);
  return tech_list[static_cast<uint32_t>(tech_key)];
}

void Trainer::Update(double /*delta*/) {
  DCHECK(IsWithinGameLoopThread());
  // Perform activated operations.
  if (activate_inst_building_) {
    FinishBuilding();
  }
  if (activate_inst_superweapon_) {
    FinishSuperweapon();
  }
  if (activate_inst_turn_turret_ || activate_inst_turn_body_) {
    RotateUnit();
  }
  if (activate_auto_clean_beacon_) {
    game::Beacon::Instance->AutoClean(
        yrpp::HouseClass::CurrentPlayer->ArrayIndex);
  }
  // Update selecting houses to view.
  SideMap selecting_houses;
  ForeachSelectingObject([&](yrpp::ObjectClass* obj) {
    yrpp::HouseClass* obj_house = obj->GetOwningHouse();
    CHECK_NOTNULL(obj_house);
    size_t namelen = strnlen_s(obj_house->PlainName, 21);
    selecting_houses.emplace(
        obj_house->UniqueID,
        SideDesc{
            .uniq_id = obj_house->UniqueID,
            .name = std::string(obj_house->PlainName, namelen),
        });
  });
  if (!AreEqual(state_.selecting_houses, selecting_houses)) {
    state_.selecting_houses = std::move(selecting_houses);
    state_dirty_ = true;
  }
  protected_houses_ = state_.protected_houses;
  PropagateStateIfDirty();
}

void Trainer::OnInputEvent(FnLabel label, uint32_t val) {
  DCHECK(IsWithinGameLoopThread());
#ifdef YRTR_DEBUG
  // There's only one input event for now.
  DCHECK_EQ(static_cast<int>(label), static_cast<int>(FnLabel::kApply));
#else
  UNREFERENCED_PARAMETER(label);
#endif
  OnInputCredit(val);
  PropagateStateIfDirty();
}

void Trainer::OnButtonEvent(FnLabel label) {
  DCHECK(IsWithinGameLoopThread());
  switch (label) {
    case FnLabel::kFastBuild:   OnBtnFastBuild();   break;
    case FnLabel::kDeleteUnit:  OnBtnDeleteUnit();  break;
    case FnLabel::kClearShroud: OnBtnClearShroud(); break;
    case FnLabel::kGiveMeABomb: OnBtnGiveMeABomb(); break;
    case FnLabel::kUnitLevelUp: OnBtnUnitLevelUp(); break;
    case FnLabel::kUnitSpeedUp: OnBtnUnitSpeedUp(); break;
    case FnLabel::kIAMWinner:   OnBtnIAMWinner();   break;
    case FnLabel::kThisIsMine:  OnBtnThisIsMine();  break;
    default:
      LOG_F(ERROR, "Unknown button event={}", StrFnLabel(label));
  }
  PropagateStateIfDirty();
}

void Trainer::OnCheckboxEvent(FnLabel label, bool activate) {
  DCHECK(IsWithinGameLoopThread());
  switch (label) {
    case FnLabel::kGod:                 OnCkboxGod(activate);                 break;
    case FnLabel::kInstBuild:           OnCkboxInstBuild(activate);           break;
    case FnLabel::kUnlimitSuperWeapon:  OnCkboxUnlimitSuperWeapon(activate);  break;
    case FnLabel::kInstFire:            OnCkboxInstFire(activate);            break;
    case FnLabel::kInstTurn:            OnCkboxInstTurn(activate);            break;
    case FnLabel::kRangeToYourBase:     OnCkboxRangeToYourBase(activate);     break;
    case FnLabel::kFireToYourBase:      OnCkboxFireToYourBase(activate);      break;
    case FnLabel::kFreezeGapGenerator:  OnCkboxFreezeGapGenerator(activate);  break;
    case FnLabel::kSellTheWorld:        OnCkboxSellTheWorld(activate);        break;
    case FnLabel::kBuildEveryWhere:     OnCkboxBuildEveryWhere(activate);     break;
    case FnLabel::kAutoRepair:          OnCkboxAutoRepair(activate);          break;
    case FnLabel::kSocialismMajesty:    OnCkboxSocialismMajesty(activate);    break;
    case FnLabel::kMakeCapturedMine:    OnCkboxMakeCapturedMine(activate);    break;
    case FnLabel::kMakeGarrisonedMine:  OnCkboxMakeGarrisonedMine(activate);  break;
    case FnLabel::kInvadeMode:          OnCkboxInvadeMode(activate);          break;
    case FnLabel::kUnlimitTech:         OnCkboxUnlimitTech(activate);         break;
    case FnLabel::kUnlimitFirePower:    OnCkboxUnlimitFirePower(activate);    break;
    case FnLabel::kInstChrono:          OnCkboxInstChrono(activate);          break;
    case FnLabel::kSpySpy:              OnCkboxSpySpy(activate);              break;
    case FnLabel::kAdjustGameSpeed:     OnCkboxAdjustGameSpeed(activate);     break;
    default:
      LOG_F(ERROR, "Unknown checkbox event={}", StrFnLabel(label));
  }
  PropagateStateIfDirty();
}

void Trainer::OnProtectedListEvent(SideMap&& side_map) {
  DCHECK(IsWithinGameLoopThread());
  if (!AreEqual(state_.protected_houses, side_map)) {
    state_.protected_houses = std::move(side_map);
    state_dirty_ = true;
  }
  PropagateStateIfDirty();
}

void Trainer::PropagateStateIfDirty() {
  DCHECK(IsWithinGameLoopThread());
  if (!state_dirty_) {
    return;
  }
  state_dirty_ = false;
  if (on_state_updated_ == nullptr) {
    return;
  }
  on_state_updated_(state_);
}

void Trainer::OnInputCredit(uint32_t val) {
  DLOG_F(INFO, "Trigger {} val={}", __FUNCTION__, val);
  DCHECK(IsWithinGameLoopThread());
  CHECK_REPORT(IsGaming());
  CHECK_REPORT(WriteCredit(val));
  BeepEnable();
}

void Trainer::OnBtnFastBuild() {
  DLOG_F(INFO, "Trigger {}", __FUNCTION__);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  CHECK_REPORT(IsGaming());
  ForeachProtectedHouse([](yrpp::HouseClass* house) {
    house->NumAirpads = 15;
    house->NumBarracks = 15;
    house->NumWarFactories = 15;
    house->NumConYards = 15;
    house->NumShipyards = 15;
  });
  BeepEnable();
}

void Trainer::OnBtnDeleteUnit() {
  DLOG_F(INFO, "Trigger {}", __FUNCTION__);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  CHECK_REPORT(IsGaming());
  ForeachSelectingObject([](yrpp::ObjectClass* obj) { obj->UnInit(); });
  BeepEnable();
}

void Trainer::OnBtnClearShroud() {
  DLOG_F(INFO, "Trigger {}", __FUNCTION__);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  CHECK_REPORT(IsGaming());
  if (yrpp::HouseClass::CurrentPlayer != nullptr) {
    yrpp::MapClass::Instance->Reveal(yrpp::HouseClass::CurrentPlayer);
  }
  BeepEnable();
}

void Trainer::OnBtnGiveMeABomb() {
  DLOG_F(INFO, "Trigger {}", __FUNCTION__);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  CHECK_REPORT(IsGaming());
  yrpp::HouseClass* house = yrpp::HouseClass::CurrentPlayer;
  CHECK_NOTNULL(house);
  if (house->Supers.Count == 0) {
    return;
  }
  yrpp::SuperClass* super0 = house->Supers.GetItem(0);
  CHECK_NOTNULL(super0);
  if (super0->CameoChargeState != -1) {
    // Break if the game enables super weapons, which conflicts with nuke from
    // crate.
    return;
  }
  // Find super weapon.
  int index = -1;
  yrpp::SuperClass* nuke_inst = nullptr;
  for (int i = 0; i < house->Supers.Count; ++i) {
    if (house->Supers.Items[i]->Type->Type == yrpp::SuperWeaponType::Nuke) {
      index = i;
      nuke_inst = house->Supers.Items[i];
      break;
    }
  }
  if (index == -1) {
    // Not found target type.
    return;
  }
  CHECK_NOTNULL(nuke_inst);
  nuke_inst->Grant(/*oneTime*/ true, /*announce*/ false, /*onHold*/ false);
  yrpp::SidebarClass::Instance->AddCameo(yrpp::AbstractType::Special, index);
  BeepEnable();
}

void Trainer::OnBtnUnitLevelUp() {
  DLOG_F(INFO, "Trigger {}", __FUNCTION__);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  CHECK_REPORT(IsGaming());
  ForeachSelectingObject([](yrpp::ObjectClass* unit) {
    yrpp::TechnoClass* techno = yrpp::generic_cast<yrpp::TechnoClass*>(unit);
    if (techno != nullptr) {
      techno->Veterancy.SetElite(true);
    }
  });
  BeepEnable();
}

void Trainer::OnBtnUnitSpeedUp() {
  DLOG_F(INFO, "Trigger {}", __FUNCTION__);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  CHECK_REPORT(IsGaming());
  ForeachSelectingObject([](yrpp::ObjectClass* obj) {
    yrpp::UnitClass* unit = DynamicCast<yrpp::UnitClass*>(obj);
    if (unit != nullptr) {
      unit->SpeedMultiplier = 5.0;
    }
    yrpp::InfantryClass* infantry = DynamicCast<yrpp::InfantryClass*>(obj);
    if (infantry != nullptr) {
      // Higher value may cause the unit getting into some kind of "glitch"
      // state.
      infantry->SpeedMultiplier = 2.0;
    }
  });
  BeepEnable();
  // FUTURE: how to speed up aircrafts?
}

void Trainer::OnBtnIAMWinner() {
  DLOG_F(INFO, "Trigger {}", __FUNCTION__);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  CHECK_REPORT(IsGaming());
  CHECK_REPORT(mem_api_->WriteMemory(0x00A83D49, static_cast<uint8_t>(1)));
  BeepEnable();
}

void Trainer::OnBtnThisIsMine() {
  DLOG_F(INFO, "Trigger {}", __FUNCTION__);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  CHECK_REPORT(IsGaming());
  ForeachSelectingObject([](yrpp::ObjectClass* obj) {
    yrpp::TechnoClass* techno = DynamicCast<yrpp::TechnoClass*>(obj);
    DLOG_F(INFO, "obj={:p} to techno={:p}", (void*)obj, (void*)techno);
    if (techno != nullptr) {
      techno->SetOwningHouse(yrpp::HouseClass::CurrentPlayer);
    }
  });
  BeepEnable();
}

void Trainer::OnCkboxGod(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    // Suppress damage.
    CHECK_REPORT(mem_api_->HookJump(kHpGodPlayer, InjectGodPlayer));
    // Suppress chrono legionnaire attacking.
    CHECK_REPORT(mem_api_->HookJump(kHpCanWrapTarget, InjectCanWrapTarget));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpGodPlayer));
    CHECK_REPORT(mem_api_->RestoreHook(kHpCanWrapTarget));
  }
  UpdateCheckboxState(FnLabel::kGod, activate);
}

void Trainer::OnCkboxInstBuild(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  activate_inst_building_ = activate;
  UpdateCheckboxState(FnLabel::kInstBuild, activate);
}

void Trainer::OnCkboxUnlimitSuperWeapon(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  activate_inst_superweapon_ = activate;
  UpdateCheckboxState(FnLabel::kUnlimitSuperWeapon, activate);
}

void Trainer::OnCkboxInstFire(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  activate_inst_turn_turret_ = activate;
  if (activate) {
    // Skip reload timer.
    CHECK_REPORT(mem_api_->HookJump(kHpInstFire, InjectInstFire));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpInstFire));
  }
  UpdateCheckboxState(FnLabel::kInstFire, activate);
}

void Trainer::OnCkboxInstTurn(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  activate_inst_turn_body_ = activate;
  UpdateCheckboxState(FnLabel::kInstTurn, activate);
}

void Trainer::OnCkboxRangeToYourBase(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    // Skip reload timer.
    CHECK_REPORT(mem_api_->HookJump(kHpRangeToYourBase, InjectRangeToYourBase));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpRangeToYourBase));
  }
  UpdateCheckboxState(FnLabel::kRangeToYourBase, activate);
  bool enable_child = activate;
  bool should_deactivate =
      SetEnableCheckbox(FnLabel::kFireToYourBase, enable_child);
  if (should_deactivate) {
    OnCkboxFireToYourBase(false);
  }
}

void Trainer::OnCkboxFireToYourBase(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    CHECK_REPORT(mem_api_->HookJump(kHpFireToYourBase, InjectFireToYourBase));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpFireToYourBase));
  }
  UpdateCheckboxState(FnLabel::kFireToYourBase, activate);
}

void Trainer::OnCkboxFreezeGapGenerator(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    CHECK_REPORT(
        mem_api_->HookJump(kHpFreezeGapGenerator, InjectFreezeGapGenerator));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpFreezeGapGenerator));
  }
  UpdateCheckboxState(FnLabel::kFreezeGapGenerator, activate);
}

void Trainer::OnCkboxSellTheWorld(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    // Enable sell cursor.
    CHECK_REPORT(
        mem_api_->HookJump(kHpSellTheWorldCursor, InjectSellTheWorldCursor));
    // Allow sell things not belong to player.
    CHECK_REPORT(mem_api_->HookJump(kHpSellTheWorldBlong,
                                    reinterpret_cast<void*>(0x004C6F9C)));
    // Allow sell things not built by player.
    CHECK_REPORT(mem_api_->HookNop(kHpSellTheWorldBuilder));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpSellTheWorldCursor));
    CHECK_REPORT(mem_api_->RestoreHook(kHpSellTheWorldBlong));
    CHECK_REPORT(mem_api_->RestoreHook(kHpSellTheWorldBuilder));
  }
  UpdateCheckboxState(FnLabel::kSellTheWorld, activate);
}

void Trainer::OnCkboxBuildEveryWhere(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    CHECK_REPORT(mem_api_->HookJump(kHpBuildEveryWhereGround,
                                    InjectBuildEveryWhereGround));
    CHECK_REPORT(mem_api_->HookJump(kHpBuildEveryWhereWater,
                                    InjectBuildEveryWhereWater));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpBuildEveryWhereGround));
    CHECK_REPORT(mem_api_->RestoreHook(kHpBuildEveryWhereWater));
  }
  UpdateCheckboxState(FnLabel::kBuildEveryWhere, activate);
}

void Trainer::OnCkboxAutoRepair(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    ForeachProtectedHouse([this](yrpp::HouseClass* house) {
      // From VA:00450645, controls auto repair.
      if (house->IQLevel2 < yrpp::RulesClass::Instance->RepairSell) {
        // Store states so we can restore back.
        iq_levels_[house->UniqueID] = house->IQLevel2;
        // This is a smart ass, who knows to fix injured building.
        house->IQLevel2 = 5;
      }
    });
    CHECK_REPORT(
        mem_api_->HookJump(kHpAutoRepairNeutral, InjectAutoRepairNeutral));
  } else {
    ForeachProtectedHouse([this](yrpp::HouseClass* house){
      auto it = iq_levels_.find(house->UniqueID);
      if (it != iq_levels_.end()) {
        house->IQLevel2 = it->second;
        iq_levels_.erase(it);
      }
    });
    CHECK_REPORT(mem_api_->RestoreHook(kHpAutoRepairNeutral));
  }
  UpdateCheckboxState(FnLabel::kAutoRepair, activate);
}

void Trainer::OnCkboxSocialismMajesty(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    CHECK_REPORT(mem_api_->HookJump(kHpSocialismMajestyCome,
                                    InjectSocialismMajestyCome));
    CHECK_REPORT(mem_api_->HookJump(kHpSocialismMajestyBack,
                                    InjectSocialismMajestyBack));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpSocialismMajestyCome));
    CHECK_REPORT(mem_api_->RestoreHook(kHpSocialismMajestyBack));
  }
  UpdateCheckboxState(FnLabel::kSocialismMajesty, activate);
}

void Trainer::OnCkboxMakeCapturedMine(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    // Suppress building captured by engineers.
    CHECK_REPORT(mem_api_->HookJump(kHpCapturedMine, InjectCapturedMine));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpCapturedMine));
  }
  UpdateCheckboxState(FnLabel::kMakeCapturedMine, activate);
}

void Trainer::OnCkboxMakeGarrisonedMine(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    CHECK_REPORT(mem_api_->HookJump(kHpGarrisonedMine, InjectGarrisonedMine));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpGarrisonedMine));
  }
  UpdateCheckboxState(FnLabel::kMakeGarrisonedMine, activate);
}

void Trainer::OnCkboxInvadeMode(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    CHECK_REPORT(
        mem_api_->HookJump(kHpInvadeMode, reinterpret_cast<void*>(0x006F8604)));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpInvadeMode));
  }
  UpdateCheckboxState(FnLabel::kInvadeMode, activate);
}

void Trainer::OnCkboxUnlimitTech(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    CHECK_REPORT(mem_api_->HookJump(kHpUnlimitTech, InjectUnlimitTech));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpUnlimitTech));
  }
  UpdateCheckboxState(FnLabel::kUnlimitTech, activate);
}

void Trainer::OnCkboxUnlimitFirePower(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    CHECK_REPORT(mem_api_->HookJump(kHpFastReload, InjectFastReload));
    CHECK_REPORT(mem_api_->HookJump(kHpMoreAmmunition, InjectMoreAmmunition));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpFastReload));
    CHECK_REPORT(mem_api_->RestoreHook(kHpMoreAmmunition));
  }
  UpdateCheckboxState(FnLabel::kUnlimitFirePower, activate);
}

void Trainer::OnCkboxInstChrono(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    CHECK_REPORT(mem_api_->HookJump(kHpInstChronoMove, InjectInstChronoMove));
    CHECK_REPORT(
        mem_api_->HookJump(kHpInstChronoAttack, InjectInstChronoAttack));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpInstChronoMove));
    CHECK_REPORT(mem_api_->RestoreHook(kHpInstChronoAttack));
  }
  UpdateCheckboxState(FnLabel::kInstChrono, activate);
}

void Trainer::OnCkboxSpySpy(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    CHECK_REPORT(mem_api_->HookJump(kHpSpySpy, InjectSpySpy));
  } else {
    CHECK_REPORT(mem_api_->RestoreHook(kHpSpySpy));
  }
  UpdateCheckboxState(FnLabel::kSpySpy, activate);
}

void Trainer::OnCkboxAdjustGameSpeed(bool activate) {
  DLOG_F(INFO, "Trigger {} activate={}", __FUNCTION__, activate);
  DCHECK(IsWithinGameLoopThread());
  CHECK_MEMAPI_OR_REPORT();
  if (activate) {
    CHECK_REPORT(mem_api_->WriteMemory(0x00A8EDDC, static_cast<uint8_t>(1)));
  } else {
    CHECK_REPORT(mem_api_->WriteMemory(0x00A8EDDC, static_cast<uint8_t>(0)));
  }
  UpdateCheckboxState(FnLabel::kAdjustGameSpeed, activate);
}

#define CHECK_MEMAPI_OR_RETURN_FALSE()                                         \
  if (mem_api_ == nullptr) {                                                   \
    LOG_F(WARNING, "MemoryAPI not initialized. Ensure the game has started."); \
    return false;                                                              \
  }

#define CHECK_RETF(exp) \
  if (!(exp)) {         \
    return false;       \
  }

void Trainer::ForeachSelectingObject(
    std::function<void(yrpp::ObjectClass*)> cb) {
  for (int i = 0; i < yrpp::ObjectClass::CurrentObjects->Count; i++) {
    yrpp::ObjectClass* obj = yrpp::ObjectClass::CurrentObjects->GetItem(i);
    CHECK_NOTNULL(obj);
    cb(obj);
  }
}

void Trainer::ForeachProtectedHouse(std::function<void(yrpp::HouseClass*)> cb) {
  for (const SideDesc& desc : std::views::values(protected_houses_)) {
    yrpp::HouseClass* house = FindHouseById(desc.uniq_id);
    if (house != nullptr) {
      cb(house);
    } else {
      LOG_F(ERROR, "Not found house={} by id={}", desc.item_name(),
            desc.uniq_id);
    }
  }
}

bool Trainer::SetEnableCheckbox(FnLabel label, bool enable) {
  DCHECK(IsWithinGameLoopThread());
  if (state_.ckbox_states[label].enable != enable) {
    state_.ckbox_states[label].enable = enable;
    state_dirty_ = true;
  }
  return state_.ckbox_states[label].activate;
}

void Trainer::UpdateCheckboxState(FnLabel label, bool activate) {
  DCHECK(IsWithinGameLoopThread());
  if (activate) {
    BeepEnable();
  } else {
    BeepDisable();
  }
  if (state_.ckbox_states[label].activate != activate) {
    state_.ckbox_states[label].activate = activate;
    state_dirty_ = true;
  }
}

namespace {
static void FinishFactoryProduction(yrpp::FactoryClass* factory) {
  if (factory == nullptr) {
    return;
  }
  factory->Production.Value = std::max(factory->Production.Value, 53);
  factory->Production.HasChanged = true;
  factory->Production.Timer.TimeLeft = 0;
}
}  // namespace

void Trainer::FinishBuilding() const {
  ForeachProtectedHouse([](yrpp::HouseClass* house) {
    FinishFactoryProduction(house->Primary_ForAircraft);
    FinishFactoryProduction(house->Primary_ForInfantry);
    FinishFactoryProduction(house->Primary_ForVehicles);
    FinishFactoryProduction(house->Primary_ForShips);
    FinishFactoryProduction(house->Primary_ForBuildings);
    FinishFactoryProduction(house->Primary_ForDefenses);
  });
}

void Trainer::FinishSuperweapon() const {
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
  ForeachProtectedHouse([](yrpp::HouseClass* house) {
    for (int i = 0; i < house->Supers.Count; i++) {
      yrpp::SuperClass* super = house->Supers.GetItem(i);
      CHECK_NOTNULL(super);
      if (super->CameoChargeState != -1) {
        super->IsCharged = true;
      }
    }
  });
}

void Trainer::RotateUnit() const {
  for (int i = 0; i < yrpp::TechnoClass::Array->Count; i++) {
    yrpp::TechnoClass* obj = yrpp::TechnoClass::Array->GetItem(i);
    if (activate_inst_turn_turret_ && obj->HasTurret()) {
      obj->SecondaryFacing.SetCurrent(obj->SecondaryFacing.DesiredFacing);
    }
    if (activate_inst_turn_body_) {
      obj->PrimaryFacing.SetCurrent(obj->PrimaryFacing.DesiredFacing);
    }
  }
}

bool Trainer::IsGaming() const {
  return yrpp::Game::IsActive;
}

bool Trainer::WriteCredit(uint32_t credit) const {
  ForeachProtectedHouse([credit](yrpp::HouseClass* house) {
    house->GiveMoney(static_cast<int>(credit));
  });
  return true;
}

bool Trainer::UnlimitRadar() const {
  CHECK_MEMAPI_OR_RETURN_FALSE();
  CHECK_RETF(IsGaming());
  yrpp::ScenarioClass::Instance->FreeRadar = true;
  return true;
}

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
