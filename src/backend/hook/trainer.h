#pragma once
#include <cstdint>

#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/container/inlined_vector.h"
#include "absl/container/flat_hash_map.h"
#include "absl/synchronization/mutex.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "backend/hook/memory_api.h"
#include "protocol/model.h"

namespace yrpp {
class AbstractClass;
class HouseClass;
class ObjectClass;
}  // namespace yrpp

namespace yrtr {
namespace backend {
namespace hook {
// MVC -- controller.

class Trainer {
 public:
  static bool is_active_disable_gagap() { return activate_disable_gagap_; }
  static bool ShouldProtect(yrpp::AbstractClass* obj);
  static bool ShouldProtect(yrpp::HouseClass* house);

  Trainer();
  Trainer(Trainer&&) = delete;
  Trainer& operator=(Trainer&&) = delete;

  State state() const {
    absl::MutexLock lk(&state_mu_);
    return state_;
  }

  void Update(double delta);

  void OnInputCredit(uint32_t val);
  void OnBtnFastBuild();
  void OnBtnDeleteUnit();
  void OnBtnClearShroud();
  void OnBtnGiveMeABomb();
  void OnBtnUnitLevelUp();
  void OnBtnUnitSpeedUp();
  void OnBtnIAMWinner();
  void OnBtnThisIsMine();
  // void OnBtnIAMGhost();

  void OnCkboxGod(bool activate);
  void OnCkboxInstBuild(bool activate);
  void OnCkboxUnlimitSuperWeapon(bool activate);
  // void OnCkboxUnlimitRadar(bool activate);
  void OnCkboxInstFire(bool activate);
  void OnCkboxInstTurn(bool activate);
  void OnCkboxRangeToYourBase(bool activate);
  void OnCkboxFireToYourBase(bool activate);
  void OnCkboxFreezeGapGenerator(bool activate);
  // void OnCkboxFreezeUnit(bool activate);
  void OnCkboxSellTheWorld(bool activate);
  // void OnCkboxUnlimitPower(bool activate);
  void OnCkboxBuildEveryWhere(bool activate);
  void OnCkboxAutoRepair(bool activate);
  // void OnCkboxEnermyRevertRepair(bool activate);
  // void OnCkboxSocialismTheBest(bool activate);
  // void OnCkboxMakeAttackedMine(bool activate);
  // void OnCkboxMakeCapturedMine(bool activate);
  void OnCkboxMakeGarrisonedMine(bool activate);
  void OnCkboxInvadeMode(bool activate);
  void OnCkboxUnlimitTech(bool activate);
  // void OnCkboxFastReload(bool activate);
  void OnCkboxUnlimitFirePower(bool activate);
  void OnCkboxInstChrono(bool activate);
  void OnCkboxSpySpy(bool activate);
  // void OnCkboxInfantrySlip(bool activate);
  // void OnCkboxUnitLeveledUp(bool activate);
  void OnCkboxAdjustGameSpeed(bool activate);

 private:
  // Update from state before use.
  static SideMap protected_houses_;
  static bool activate_disable_gagap_;

  mutable absl::Mutex state_mu_;
  State state_ ABSL_GUARDED_BY(state_mu_);

  std::unique_ptr<MemoryAPI> mem_api_;

  bool activate_inst_building_;
  bool activate_inst_superweapon_;
  bool activate_inst_turn_turret_;
  bool activate_inst_turn_body_;
  // From VA:00450645, controls auto repair.
  absl::flat_hash_map<UniqId /*house_id*/, int /*iq_level*/> iq_levels_;

  static void ForeachSelectingObject(
      std::function<void(yrpp::ObjectClass*)> cb);
  static void ForeachProtectedHouse(std::function<void(yrpp::HouseClass*)> cb);

  void UpdateCheckboxState(FnLabel label, bool activate);
  // Return the activate state before set enable.
  bool SetEnableCheckbox(FnLabel label, bool enable);
  void FinishBuilding() const;
  void FinishSuperweapon() const;
  void TurnUnit() const;
  bool IsGaming() const;
  bool WriteCredit(uint32_t credit) const;
  bool UnlimitRadar() const;

  DISALLOW_COPY_AND_ASSIGN(Trainer);
};

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
