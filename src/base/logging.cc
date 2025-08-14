#include "base/logging.h"

#include <ctime>

#ifdef _WIN32
#include "base/windows_shit.h"
#endif

namespace yrtr {
namespace logging {

#ifdef _WIN32
WindowsDebuggerLogSink WindowsDebuggerLogSink::kLogSink;

void WindowsDebuggerLogSink::Send(const absl::LogEntry& entry) {
  if (entry.log_severity() < absl::StderrThreshold()) {
    return;
  }
  mu_.lock();
  ::OutputDebugStringA(entry.text_message_with_prefix_and_newline_c_str());
  if (!entry.stacktrace().empty()) [[unlikely]] {
    ::OutputDebugStringA(entry.stacktrace().data());
  }
  mu_.unlock();
}
#endif

StderrLogSink StderrLogSink::kLogSink;

void StderrLogSink::Send(const absl::LogEntry& entry) {
  if (entry.log_severity() < absl::StderrThreshold()) {
    return;
  }
  mu_.lock();
  absl::log_internal::WriteToStderr(
      entry.text_message_with_prefix_and_newline(), entry.log_severity());
  if (!entry.stacktrace().empty()) [[unlikely]] {
    absl::log_internal::WriteToStderr(entry.stacktrace(), entry.log_severity());
  }
  mu_.unlock();
}

FileLogSink FileLogSink::kLogSink;

FileLogSink::~FileLogSink() {
  std::lock_guard<std::mutex> lock(mu_);
  if (log_file_.is_open()) {
    log_file_.close();
  }
}

bool FileLogSink::SetLogFile(const std::string& filename) {
  std::lock_guard<std::mutex> lock(kLogSink.mu_);
  if (kLogSink.log_file_.is_open()) {
    kLogSink.log_file_.close();
  }
  kLogSink.log_file_.open(filename, std::ios::out | std::ios::trunc);
  if (!kLogSink.log_file_.is_open()) {
    return false;
  }
  // Write initial marker.
  auto now = std::chrono::system_clock::now();
  auto now_time = std::chrono::system_clock::to_time_t(now);
  std::string time_str;
  time_str.resize(26);
  errno_t ret = ctime_s(time_str.data(), time_str.size(), &now_time);
  // Remove the trailing newline.
  while (ret == 0 && !time_str.empty() &&
         (time_str.back() == '\n' || time_str.back() == '\0')) {
    time_str.pop_back();
  }
  kLogSink.log_file_ << "=== Log started at " << time_str << " ===\n";
  return true;
}

void FileLogSink::Send(const absl::LogEntry& entry) {
  std::lock_guard<std::mutex> lock(mu_);
  if (log_file_.is_open()) {
    log_file_ << entry.text_message_with_prefix_and_newline();
    if (!entry.stacktrace().empty()) [[unlikely]] {
      log_file_ << entry.stacktrace();
    }
    log_file_.flush();
  }
}

namespace {
static absl::LogSink* kLogSinkInst = nullptr;
static std::ostringstream g_nullstream;
}

std::ostringstream& get_nullstream() { return g_nullstream; }

void InitLogging(LogSink log_sink, const std::string& log_file) {
  if (log_sink == LogSink::kStd) {
    absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);
    kLogSinkInst = StderrLogSink::get();
  } else if (log_sink == LogSink::kFile) {
    if (!FileLogSink::SetLogFile(log_file)) {
      perror(("Failed to open log file: " + log_file).c_str());
      abort();
    }
    kLogSinkInst = FileLogSink::get();
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
