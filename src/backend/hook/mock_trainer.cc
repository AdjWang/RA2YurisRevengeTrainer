#include "backend/hook/mock_trainer.h"

#include "backend/record.h"
#include "base/logging.h"
#include "base/thread.h"

namespace yrtr {
namespace backend {
namespace hook {
SideMap MockTrainer::protected_houses_;

MockTrainer::MockTrainer(Config* cfg)
    : cfg_(cfg),
      state_dirty_(true) {
  InitStates(state_);
  if (cfg_->auto_record()) {
    ReadCheckboxStateFromToml(cfg_->record_path(), /*out*/ state_.ckbox_states);
  }
}

MockTrainer::~MockTrainer() {
  if (cfg_->auto_record()) {
    WriteCheckboxStateToToml(state_.ckbox_states, cfg_->record_path());
  }
}

void MockTrainer::Update(double /*delta*/) {
  CHECK(IsWithinGameLoopThread());
  // Update selecting houses to view.
  SideMap selecting_houses;
  selecting_houses.emplace(123, SideDesc{
                                    .uniq_id = 123,
                                    .name = "mock player 123",
                                });
  selecting_houses.emplace(456, SideDesc{
                                    .uniq_id = 456,
                                    .name = "mock player 456",
                                });
  if (!AreEqual(state_.selecting_houses, selecting_houses)) {
    state_.selecting_houses = std::move(selecting_houses);
    state_dirty_ = true;
  }
  protected_houses_ = state_.protected_houses;
  PropagateStateIfDirty();
}

void MockTrainer::OnInputEvent(FnLabel label, uint32_t val) {
  CHECK(IsWithinGameLoopThread());
  // There's only one input event for now.
  CHECK_EQ(static_cast<int>(label), static_cast<int>(FnLabel::kApply));
  LOG_F(INFO, "OnInputEvent label={} val={}", StrFnLabel(label), val);
  BeepEnable();
  PropagateStateIfDirty();
}

void MockTrainer::OnButtonEvent(FnLabel label) {
  CHECK(IsWithinGameLoopThread());
  LOG_F(INFO, "OnButtonEvent label={}", StrFnLabel(label));
  BeepEnable();
  PropagateStateIfDirty();
}

void MockTrainer::OnCheckboxEvent(FnLabel label, bool activate) {
  CHECK(IsWithinGameLoopThread());
  LOG_F(INFO, "OnCheckboxEvent label={} activate={}", StrFnLabel(label),
        activate);
  UpdateCheckboxState(label, activate);
  PropagateStateIfDirty();
}

void MockTrainer::OnProtectedListEvent(SideMap&& side_map) {
  CHECK(IsWithinGameLoopThread());
  if (!AreEqual(state_.protected_houses, side_map)) {
    state_.protected_houses = std::move(side_map);
    state_dirty_ = true;
  }
  PropagateStateIfDirty();
}

void MockTrainer::PropagateStateIfDirty() {
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

void MockTrainer::UpdateCheckboxState(FnLabel label, bool activate) {
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

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
