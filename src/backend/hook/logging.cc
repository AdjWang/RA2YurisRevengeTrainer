#include "backend/hook/logging.h"

#include <cstdint>
#include <cstdio>

#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "Unsorted.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "backend/hook/hook_point.h"
#include "backend/hook/memory_api.h"
#include "base/logging.h"

namespace yrtr {
namespace backend {
namespace hook {

namespace {
static char log_buf[0x200];

static void __cdecl BuiltinLogging(const char* fmt, ...) {
  // caller ptr
  uint32_t* p_caller =
      reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(&fmt) - 4);
  va_list arg = (va_list)(reinterpret_cast<char*>(&fmt) + 4);
  int n = vsprintf_s(log_buf, 0x200, fmt, arg);
  DCHECK_LE(n, 0x200);
  if (n > 0 && log_buf[n - 1] == '\n') {
    log_buf[n - 1] = '\0';
  }
  int frame = yrpp::Unsorted::CurrentFrame;
  LOG_F(INFO, "F={} P={:08X} {}", frame, *p_caller, log_buf);
}
}  // namespace

void HookLogging() {
  DLOG_F(INFO, "[YRTR-HOOK] {}", __func__);
  MemoryAPI::instance()->HookJump(kHpBuiltinLogging, BuiltinLogging);
}

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
