#pragma once
#include <cstdint>

#include "backend/hook/trainer.h"
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/synchronization/mutex.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "protocol/model.h"

namespace yrtr {
namespace backend {
namespace hook {

class MockTrainer : public ITrainer {
 public:
  MockTrainer();
  MockTrainer(MockTrainer&&) = delete;
  MockTrainer& operator=(MockTrainer&&) = delete;

  State state() const final {
    absl::MutexLock lk(&state_mu_);
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

  mutable absl::Mutex state_mu_;
  State state_ ABSL_GUARDED_BY(state_mu_);
  std::function<void(State)> on_state_updated_;

  void UpdateCheckboxState(FnLabel label, bool activate);

  DISALLOW_COPY_AND_ASSIGN(MockTrainer);
};

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
