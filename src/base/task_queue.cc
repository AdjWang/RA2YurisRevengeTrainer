#include "base/task_queue.h"

#include "base/logging.h"
#include "base/debug.h"

namespace yrtr {

TaskQueue::TaskQueue() {}

void TaskQueue::SetThreadId(ThreadId tid) {
  // Only allowed to set once.
  ThreadId old_tid = 0;
  tid_.compare_exchange_strong(old_tid, tid);
}

void TaskQueue::ExecuteOrScheduleTask(Task&& task) {
  // Not pending task if the loop is not start yet. Or remove this clause to
  // pending task before target thread starting.
  ThreadId tid = tid_.load();
  if (tid == 0) [[unlikely]] {
    LOG_F(WARNING, "Trying to add task before setting loop thread");
    return;
  }
  if (IsWithinThread(tid)) {
    task();
  } else {
    absl::MutexLock lk(&pending_tasks_mu_);
    pending_tasks_.emplace_back(std::move(task));
  }
}

void TaskQueue::ExecuteTasks() {
  ThreadId tid = tid_.load();
  if (tid == 0) [[unlikely]] {
    LOG_F(ERROR, "Trying to execute task before setting loop thread");
    return;
  }
  DCHECK(IsWithinThread(tid));
  TaskBuffer pending_tasks;
  {
    absl::MutexLock lk(&pending_tasks_mu_);
    pending_tasks_.swap(pending_tasks);
  }
  for (Task& task : pending_tasks) {
    task();
  }
}

}  // namespace yrtr
