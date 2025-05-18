#pragma once
#include <atomic>
#include <functional>

#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/container/inlined_vector.h"
#include "absl/synchronization/mutex.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "base/thread.h"

namespace yrtr {

using Task = std::function<void()>;

// Perform cross thread event passing.
class TaskQueue {
 public:
  TaskQueue();
  // Tasks are not invoked until the thread id is set. There has the potential
  // to add tasks before setup thread id, therefore this is not a constructor.
  void SetThreadId(ThreadId tid);
  // Execute task if this function is called inside the setting thread,
  // otherwise pending until a call to ExecuteTasks(). This function can be
  // called from any thread.
  void ExecuteOrScheduleTask(Task&& task);
  // Execute pending tasks pushed from other threads. This function has to be
  // called inside the setting thread.
  void ExecuteTasks();

 private:
  using TaskBuffer = absl::InlinedVector<Task, 10>;
  std::atomic<ThreadId> tid_;
  absl::Mutex pending_tasks_mu_;
  TaskBuffer pending_tasks_ ABSL_GUARDED_BY(pending_tasks_mu_);

  DISALLOW_COPY_AND_ASSIGN(TaskQueue);
};

}  // namespace yrtr
