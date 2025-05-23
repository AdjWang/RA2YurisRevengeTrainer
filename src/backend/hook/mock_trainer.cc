#include "backend/hook/mock_trainer.h"

#include "base/logging.h"
#include "base/thread.h"

namespace yrtr {
namespace backend {
namespace hook {
SideMap MockTrainer::protected_houses_;

MockTrainer::MockTrainer() {
  absl::MutexLock lk(&state_mu_);
  InitStates(state_);
}

void MockTrainer::Update(double /*delta*/) {
  CHECK(IsWithinGameLoopThread());
  // Update selecting houses to view.
  SideMap selecting_houses;
  selecting_houses.emplace(123, SideDesc{
                                    .uniq_id = 123,
                                    .name = "mock player 123",
                                });
  {
    absl::MutexLock lk(&state_mu_);
    state_.selecting_houses.swap(selecting_houses);
    protected_houses_ = state_.protected_houses;
  }
}

void MockTrainer::OnInputEvent(FnLabel label, uint32_t val) {
  CHECK(IsWithinGameLoopThread());
  // There's only one input event for now.
  CHECK_EQ(static_cast<int>(label), static_cast<int>(FnLabel::kApply));
  LOG_F(INFO, "OnInputEvent label={} val={}", StrFnLabel(label), val);
  BeepEnable();
}

void MockTrainer::OnButtonEvent(FnLabel label) {
  CHECK(IsWithinGameLoopThread());
  LOG_F(INFO, "OnButtonEvent label={}", StrFnLabel(label));
  BeepEnable();
}

void MockTrainer::OnCheckboxEvent(FnLabel label, bool activate) {
  CHECK(IsWithinGameLoopThread());
  LOG_F(INFO, "OnCheckboxEvent label={} activate={}", StrFnLabel(label),
        activate);
  UpdateCheckboxState(label, activate);
}

void MockTrainer::OnProtectedListEvent(SideMap&& side_map) {
  CHECK(IsWithinGameLoopThread());
  absl::MutexLock lk(&state_mu_);
  state_.protected_houses = std::move(side_map);
}

void MockTrainer::UpdateCheckboxState(FnLabel label, bool activate) {
  if (activate) {
    BeepEnable();
  } else {
    BeepDisable();
  }
  absl::MutexLock lk(&state_mu_);
  state_.ckbox_states[label].activate = activate;
}

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
