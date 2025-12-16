#include "base/crash_dump.h"

#include <iostream>

#include "client/windows/handler/exception_handler.h"

namespace yrtr {
namespace debug {

namespace {
// Callback function to handle minidump creation.
static bool MinidumpCallback(const wchar_t* dump_path,
                             const wchar_t* minidump_id, void* context,
                             EXCEPTION_POINTERS* exinfo,
                             MDRawAssertionInfo* assertion, bool succeeded) {
  UNREFERENCED_PARAMETER(context);
  if (succeeded) {
    std::wcout << L"Crash dump successfully created at: " << dump_path
               << std::endl;
    std::wcout << L"Minidump ID: " << minidump_id << std::endl;

    // You can analyze the exception information here
    if (exinfo) {
      std::wcout << L"Exception code: "
                 << exinfo->ExceptionRecord->ExceptionCode << std::endl;
      std::wcout << L"Exception address: "
                 << exinfo->ExceptionRecord->ExceptionAddress << std::endl;
    }

    // If it's an assertion failure, you can print assertion info
    if (assertion) {
      std::wcout << L"Assertion failed at: " << assertion->expression
                 << std::endl;
    }
  } else {
    std::wcout << L"Crash dump creation failed." << std::endl;
  }
  return succeeded;
}
}  // namespace

// For crash dump testing.
void CauseCrash() {
  int* p = nullptr;
  *p = 233;
}

void InitCrashDump(const fs::path& dump_dir) {
  static std::unique_ptr<google_breakpad::ExceptionHandler> handler;
  assert(fs::is_directory(dump_dir));
  assert(fs::exists(dump_dir));
  handler.reset(new google_breakpad::ExceptionHandler(
      dump_dir.wstring(),
      /*filter*/ NULL, MinidumpCallback,
      /*callback_context*/ NULL,
      /*handler_types*/ google_breakpad::ExceptionHandler::HANDLER_ALL));
}

}  // namespace debug
}  // namespace yrtr
