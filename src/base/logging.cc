#include "base/logging.h"

#ifdef _WIN32
#include "base/windows_shit.h"
#endif

namespace yrtr {
namespace logging {

#ifdef _WIN32
WindowsDebuggerLogSink WindowsDebuggerLogSink::kLogSink;

void WindowsDebuggerLogSink::Send(const absl::LogEntry& entry) {
  if (entry.log_severity() < absl::StderrThreshold() &&
      absl::log_internal::IsInitialized()) {
    return;
  }
  mu_.lock();
  ::OutputDebugStringA(entry.text_message_with_prefix_and_newline_c_str());
  mu_.unlock();
}
#endif

StderrLogSink StderrLogSink::kLogSink;

void StderrLogSink::Send(const absl::LogEntry& entry) {
  if (entry.log_severity() < absl::StderrThreshold()) {
    return;
  }
  mu_.lock();
  if (!entry.stacktrace().empty()) {
    absl::log_internal::WriteToStderr(entry.stacktrace(), entry.log_severity());
  } else {
    absl::log_internal::WriteToStderr(
        entry.text_message_with_prefix_and_newline(), entry.log_severity());
  }
  mu_.unlock();
}

namespace {
static absl::LogSink* kLogSinkInst = nullptr;
static std::ostringstream g_nullstream;
}

std::ostringstream& get_nullstream() { return g_nullstream; }

void InitLogging(LogSink log_sink) {
  if (log_sink == LogSink::kStd) {
    absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);
    kLogSinkInst = StderrLogSink::get();
  }
#ifdef _WIN32
  else if (log_sink == LogSink::kDbgView) {
    kLogSinkInst = WindowsDebuggerLogSink::get();
  }
#endif
  else {
    perror("Invalid log sink.");
    abort();
  }
  // BUG: crash in DllMain
  // absl::InitializeLog();
}

absl::LogSink* GetLogSink() {
  return kLogSinkInst;
}

}  // namespace logging
}  // namespace yrtr
