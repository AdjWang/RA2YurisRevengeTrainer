#pragma once
#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "base/task_queue.h"

namespace yrtr {
namespace backend {
namespace hook {

TaskQueue& GetGameLoopChannel();

void HookCreateWindow();
void DestroyWindowOnce();

void HookUpdate();
void HookExitGame();

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
