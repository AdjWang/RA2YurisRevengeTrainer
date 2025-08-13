#pragma once
#include <cstdint>
#include <functional>

#include "backend/config.h"
#include "backend/hook/memory_api.h"
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/container/inlined_vector.h"
#include "absl/container/flat_hash_map.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "base/thread.h"
#include "protocol/model.h"

namespace yrpp {
class AbstractClass;
class BuildingTypeClass;
class HouseClass;
class ObjectClass;
}  // namespace yrpp

namespace yrtr {
namespace backend {
namespace hook {
// MVC -- controller.

// Export for testing.
void BeepEnable();
void BeepDisable();
void InitStates(State& state);

// For testing.
class ITrainer {
 public:
  virtual ~ITrainer() {}
  virtual State state() const = 0;
  virtual void set_on_state_updated(std::function<void(State)> cb) = 0;
  virtual void Update(double delta) = 0;
  virtual void OnInputEvent(FnLabel label, uint32_t val) = 0;
  virtual void OnButtonEvent(FnLabel label) = 0;
  virtual void OnCheckboxEvent(FnLabel label, bool activate) = 0;
  virtual void OnProtectedListEvent(SideMap&& side_map) = 0;
};

class Trainer : public ITrainer {
 public:
  static bool is_active_disable_gagap() { return activate_disable_gagap_; }
  static bool ShouldProtect(yrpp::AbstractClass* obj);
  static bool ShouldProtect(yrpp::HouseClass* house);
  static bool ShouldEnableTech(yrpp::BuildingTypeClass* tech);

  Trainer(Config* cfg);
  Trainer(Trainer&&) = delete;
  Trainer& operator=(Trainer&&) = delete;
  ~Trainer();

  State state() const final {
    DCHECK(IsWithinGameLoopThread());
    return state_;
  }

  void set_on_state_updated(std::function<void(State)> cb) final {
    on_state_updated_ = std::move(cb);
  }

  void Update(double delta) final;
  void OnInputEvent(FnLabel label, uint32_t val) final;
  void OnButtonEvent(FnLabel label) final;
  void OnCheckboxEvent(FnLabel label, bool activate) final;
  void OnProtectedListEvent(SideMap&& side_map) final;

 private:
  // Update from state before use.
  static SideMap protected_houses_;
  static bool activate_disable_gagap_;

  Config* cfg_;
  State state_;
  std::function<void(State)> on_state_updated_;
  bool state_dirty_;

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

  void PropagateStateIfDirty();
  void OnInputCredit(uint32_t val);
  void OnBtnFastBuild();
  void OnBtnDeleteUnit();
  void OnBtnClearShroud();
  void OnBtnGiveMeABomb();
  void OnBtnUnitLevelUp();
  void OnBtnUnitSpeedUp();
  void OnBtnIAMWinner();
  void OnBtnThisIsMine();

  void OnCkboxGod(bool activate);
  void OnCkboxInstBuild(bool activate);
  void OnCkboxUnlimitSuperWeapon(bool activate);
  void OnCkboxInstFire(bool activate);
  void OnCkboxInstTurn(bool activate);
  void OnCkboxRangeToYourBase(bool activate);
  void OnCkboxFireToYourBase(bool activate);
  void OnCkboxFreezeGapGenerator(bool activate);
  void OnCkboxSellTheWorld(bool activate);
  void OnCkboxBuildEveryWhere(bool activate);
  void OnCkboxAutoRepair(bool activate);
  void OnCkboxSocialismMajesty(bool activate);
  void OnCkboxMakeCapturedMine(bool activate);
  void OnCkboxMakeGarrisonedMine(bool activate);
  void OnCkboxInvadeMode(bool activate);
  void OnCkboxUnlimitTech(bool activate);
  void OnCkboxUnlimitFirePower(bool activate);
  void OnCkboxInstChrono(bool activate);
  void OnCkboxSpySpy(bool activate);
  void OnCkboxAdjustGameSpeed(bool activate);

  void UpdateCheckboxState(FnLabel label, bool activate);
  // Return the activate state before set enable.
  bool SetEnableCheckbox(FnLabel label, bool enable);
  void FinishBuilding() const;
  void FinishSuperweapon() const;
  void RotateUnit() const;
  bool IsGaming() const;
  bool WriteCredit(uint32_t credit) const;
  bool UnlimitRadar() const;

  DISALLOW_COPY_AND_ASSIGN(Trainer);
};

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
