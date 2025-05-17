#pragma once
#include <string>

namespace yrtr {
using ThreadId = uint32_t;
// Use 0 as invalid thread id.
// https://learn.microsoft.com/en-us/windows/win32/procthread/thread-handles-and-identifiers

ThreadId GetGameLoopThreadId();
ThreadId GetRendererThreadId();
void SetupGameLoopThreadOnce(ThreadId tid = 0);
void SetupRendererThreadOnce(ThreadId tid = 0);
bool IsWithinThread(ThreadId tid);
bool IsWithinGameLoopThread();
bool IsWithinRendererThread();
// For debugging.
std::string InspectThreads();

}  // namespace yrtr
