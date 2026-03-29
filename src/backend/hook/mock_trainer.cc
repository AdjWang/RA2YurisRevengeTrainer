#include "backend/hook/mock_trainer.h"

#include "base/worker.h"  // include before WinSock.h
#include "backend/record.h"
#include "base/logging.h"
#include "base/thread.h"

namespace yrtr {
namespace backend {
namespace hook {
static Worker g_fs_worker("Filesystem");
SideMap MockTrainer::protected_houses_;

MockTrainer::MockTrainer(Config* cfg)
    : cfg_(cfg),
      state_dirty_(true),
      pending_record_(false) {
  InitStates(state_);
  if (cfg_->auto_record()) {
    // Read checkbox state from record file.
    bool ok = ReadCheckboxStateFromToml(cfg_->record_path(),
                                        /*out*/ state_.ckbox_states);
    if (ok) {
      // Apply recorded checkbox state.
      bool temp = GetBeepEnable();
      SetBeepEnable(false);
      for (auto [label, activate] : state_.ckbox_states) {
        OnCheckboxEvent(label, activate.activate);
      }
      SetBeepEnable(temp);
    }
    // Setup record worker thread.
    g_fs_worker.PostTask([this]() { SetupFilesystemThreadOnce(); });
  }
}

MockTrainer::~MockTrainer() {}

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
  // Do record.
  if (pending_record_) [[unlikely]] {
    DCHECK(cfg_->auto_record());
    pending_record_ = false;
    g_fs_worker.PostTask([this, ckbox_states_copy = state_.ckbox_states]() {
      DCHECK(IsWithinFilesystemThread());
      WriteCheckboxStateToToml(ckbox_states_copy, cfg_->record_path());
    });
  }
}

void MockTrainer::OnInputEvent(FnLabel label, uint32_t val) {
  CHECK(IsWithinGameLoopThread());
  // There's only one input event for now.
  CHECK_EQ(static_cast<int>(label), static_cast<int>(FnLabel::kApply));
  LOG_F(INFO, "OnInputEvent label={} val={}", StrFnLabel(label), val);
  PlayBeepActivate();
  PropagateStateIfDirty();
}

void MockTrainer::OnButtonEvent(FnLabel label) {
  CHECK(IsWithinGameLoopThread());
  LOG_F(INFO, "OnButtonEvent label={}", StrFnLabel(label));
  PlayBeepActivate();
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
  if (on_state_updated_ != nullptr) {
    on_state_updated_(state_);
  }
  if (cfg_->auto_record()) {
    // Trigger record states.
    auto now = std::chrono::system_clock::now();
    if (now - last_record_ts_ > std::chrono::milliseconds(kRecordDurationMs)) {
      last_record_ts_ = now;
      pending_record_ = true;
    }
  }
}

void MockTrainer::UpdateCheckboxState(FnLabel label, bool activate) {
  if (activate) {
    PlayBeepActivate();
  } else {
    PlayBeepDeactivate();
  }
  if (state_.ckbox_states[label].activate != activate) {
    state_.ckbox_states[label].activate = activate;
    state_dirty_ = true;
  }
}

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
