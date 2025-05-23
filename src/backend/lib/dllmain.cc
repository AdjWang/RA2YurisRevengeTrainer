#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "backend/hook/hook.h"
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/debugging/symbolize.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "base/logging.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason,
                    LPVOID lpvReserved) {
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH: {
      OutputDebugStringA("[YRTR] Process attach");
      yrtr::logging::InitLogging(yrtr::logging::LogSink::kDbgView);
      char dll_path[MAX_PATH] = {0};
      DWORD nSize = GetModuleFileNameA(hinstDLL, dll_path, _countof(dll_path));
      if (nSize != 0) {
        absl::InitializeSymbolizer(dll_path);
      } else {
        DWORD err = GetLastError();
        std::string message = std::system_category().message(err);
        MessageBoxA(NULL,
                    std::format("Failed to get module file name, err=[{}]{}",
                                err, message)
                        .c_str(),
                    "Ra2 trainer module symbol loading error", 0);
        // Allow carry on without symbol hints.
      }
      // TODO: may crash ares, move to game loop
      yrtr::backend::hook::InstallHooks();
      break;
    }
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH: {
      OutputDebugStringA("[YRTR] Process detach");
      if (lpvReserved != nullptr) {
        break;
      }
      break;
    }
  }
  return TRUE;
}
