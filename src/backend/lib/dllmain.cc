#include <windows.h>

#include "base/logging.h"
#include "hook/hook.h"

BOOL WINAPI DllMain(HINSTANCE /*hinstDLL*/, DWORD fdwReason,
                    LPVOID lpvReserved) {
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH: {
      yrtr::logging::InitLogging(yrtr::logging::LogSink::kDbgView);
      OutputDebugStringA("[SCARLET] Process attach");
      yrtr::hook::InstallHooks();
      break;
    }
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH: {
      OutputDebugStringA("[SCARLET] Process detach");
      if (lpvReserved != nullptr) {
        break;
      }
      break;
    }
  }
  return TRUE;
}
