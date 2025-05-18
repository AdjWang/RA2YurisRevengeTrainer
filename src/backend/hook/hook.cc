#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "MinHook.h"
#include "Syringe/Syringe.h"
#include "base/logging.h"
#include "backend/hook/game_loop.h"
#include "backend/hook/logging.h"
#include "backend/hook/memory_api.h"

namespace yrtr {
namespace backend {
namespace hook {

void InstallHooks() {
  static std::once_flag install_once;
  std::call_once(install_once, []() {
    MemoryAPI::Init();
    // HookLogging();
    HookCreateWindow();
    HookUpdate();
    HookExitGame();
  });
}

}  // namespace hook
}  // namespace backend
}  // namespace yrtr

DEFINE_HOOK(0x006BB9D8, RA2WinMain, 5) {
  UNREFERENCED_PARAMETER(R);
  LOG_F(INFO, "WinMain");
  yrtr::backend::hook::InstallHooks();
  return 0;
}


#ifdef YRTR_DEBUG
// https://github.com/Phobos-developers/Phobos/blob/6391e7def58c8dc3168438087612cfaa8267c98d/src/Phobos.Ext.cpp#L306
bool DetachFromDebugger() {
  auto GetDebuggerProcessId = [](DWORD dwSelfProcessId) -> DWORD {
    DWORD dwParentProcessId = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(2, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hSnapshot, &pe32);
    do {
      if (pe32.th32ProcessID == dwSelfProcessId) {
        dwParentProcessId = pe32.th32ParentProcessID;
        break;
      }
    } while (Process32Next(hSnapshot, &pe32));
    CloseHandle(hSnapshot);
    return dwParentProcessId;
  };

  HMODULE hModule = LoadLibrary("ntdll.dll");
  if (hModule != NULL) {
    auto const NtRemoveProcessDebug = (NTSTATUS(__stdcall*)(
        HANDLE, HANDLE))GetProcAddress(hModule, "NtRemoveProcessDebug");
    auto const NtSetInformationDebugObject = (NTSTATUS(__stdcall*)(
        HANDLE, ULONG, PVOID, ULONG,
        PULONG))GetProcAddress(hModule, "NtSetInformationDebugObject");
    auto const NtQueryInformationProcess = (NTSTATUS(__stdcall*)(
        HANDLE, ULONG, PVOID, ULONG,
        PULONG))GetProcAddress(hModule, "NtQueryInformationProcess");
    auto const NtClose =
        (NTSTATUS(__stdcall*)(HANDLE))GetProcAddress(hModule, "NtClose");

    HANDLE hDebug;
    HANDLE hCurrentProcess = GetCurrentProcess();
    NTSTATUS status = NtQueryInformationProcess(hCurrentProcess, 30, &hDebug,
                                                sizeof(HANDLE), 0);
    if (0 <= status) {
      ULONG killProcessOnExit = FALSE;
      status = NtSetInformationDebugObject(hDebug, 1, &killProcessOnExit,
                                           sizeof(ULONG), NULL);
      if (0 <= status) {
        const auto pid = GetDebuggerProcessId(GetProcessId(hCurrentProcess));
        status = NtRemoveProcessDebug(hCurrentProcess, hDebug);
        if (0 <= status) {
          HANDLE hDbgProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
          if (INVALID_HANDLE_VALUE != hDbgProcess) {
            BOOL ret = TerminateProcess(hDbgProcess, EXIT_SUCCESS);
            CloseHandle(hDbgProcess);
            return ret;
          }
        }
      }
      NtClose(hDebug);
    }
    FreeLibrary(hModule);
  }

  return false;
}

DEFINE_HOOK(0x007CD810, DetachDebugger, 9) {
  UNREFERENCED_PARAMETER(R);
#ifdef YRTR_DEBUG
  // For debugging ares.
  if (IsDebuggerPresent()) {
    DetachFromDebugger();
  }
  LOG_F(INFO, "Detach debugger succeed={}", !IsDebuggerPresent());
#endif
  // // DEBUG
  // MessageBox(NULL, "", "", MB_OK);
  return 0;
}
#endif
