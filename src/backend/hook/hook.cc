#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "MinHook.h"
#include "Syringe/Syringe.h"
#include "base/logging.h"
#include "hook/game_loop.h"
#include "hook/logging.h"
#include "hook/memory_api.h"
#include "hook/message_box.h"
#include "hook/render_loop.h"

namespace yrtr {
namespace hook {

#define HOOK_VFUNC(vft, index, hook, original)                                 \
  do {                                                                         \
    MH_STATUS status_temp = MH_CreateHook(                                     \
        reinterpret_cast<LPVOID>(vft[index]), reinterpret_cast<LPVOID>(&hook), \
        reinterpret_cast<LPVOID*>(&original));                                 \
    CHECK_F(status_temp == MH_STATUS::MH_OK, "status={}",                      \
            static_cast<int>(status));                                         \
  } while (0)

struct HookGuard {
  HookGuard() {
    MH_STATUS status;
    status = MH_Initialize();
    CHECK_F(status == MH_STATUS::MH_OK, "status={}", static_cast<int>(status));
    static constexpr uint32_t kResetIndex = 16u;
    static constexpr uint32_t kPresentIndex = 17u;
    HOOK_VFUNC(GetD3D9DeviceVtbl(), kResetIndex, HookD3D9Reset,
               OriginalD3D9Reset);
    HOOK_VFUNC(GetD3D9DeviceVtbl(), kPresentIndex, HookD3D9Present,
               OriginalD3D9Present);
    status = MH_EnableHook(MH_ALL_HOOKS);
    CHECK_F(status == MH_STATUS::MH_OK, "status={}", static_cast<int>(status));
  }
  ~HookGuard() {
    MH_STATUS status = MH_Uninitialize();
    CHECK_F(status == MH_STATUS::MH_OK, "status={}", static_cast<int>(status));
  }
};
#undef HOOK_VFUNC
static std::unique_ptr<HookGuard> hook_guard;

void InstallHooks() {
  static std::once_flag install_once;
  std::call_once(install_once, []() {
    MemoryAPI::Init();
    // HookLogging();
    HookCreateWindow();
    HookUpdate();
    HookExitGame();

    SetOnWindowCreated([&](HWND hWnd) {
      LOG_F(INFO, "Hook D3D9 functions");
      CHECK(InitD3D9DeviceVtbl(hWnd));
      hook_guard = std::make_unique<HookGuard>();
    });
  });
}

}  // namespace hook
}  // namespace yrtr

DEFINE_HOOK(0x006BB9D8, RA2WinMain, 5) {
  UNREFERENCED_PARAMETER(R);
  LOG_F(INFO, "WinMain");
  yrtr::hook::InstallHooks();
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
  // yrtr::debug::HookMessageBox();
  return 0;
}
#endif
