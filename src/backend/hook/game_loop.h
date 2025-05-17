#pragma once
#include <functional>

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "task_queue.h"

namespace yrtr {
namespace hook {

TaskQueue& GetGameLoopChannel();

void HookCreateWindow();
void SetOnWindowCreated(std::function<void(HWND)> cb);
void DestroyWindowOnce();

void HookUpdate();
void HookExitGame();

}  // namespace hook
}  // namespace yrtr
