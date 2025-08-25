// Abseil forces the initial log sink to be both stderr and windows debugger,
// causing logs printed twice in vscode console. This file is to select only
// one sink through the LogMessage::ToSinkOnly method.

#pragma once
#include <format>
#include <fstream>
#include <mutex>

#include "absl/log/absl_vlog_is_on.h"
#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/internal/globals.h"
#include "absl/log/internal/log_impl.h"
#include "absl/log/log_sink.h"

namespace yrtr {
namespace logging {

enum class LogSink {
  kStd,
  kFile,
#ifdef _WIN32
  kDbgView,
#endif
};

#ifdef _WIN32
class WindowsDebuggerLogSink final : public absl::LogSink {
 public:
  static absl::LogSink* get() { return &kLogSink; }
  ~WindowsDebuggerLogSink() override = default;

  void Send(const absl::LogEntry& entry) override;

 private:
  static WindowsDebuggerLogSink kLogSink;
  std::mutex mu_;
};
#endif
class StderrLogSink final : public absl::LogSink {
 public:
  static absl::LogSink* get() { return &kLogSink; }
  ~StderrLogSink() override = default;

  void Send(const absl::LogEntry& entry) override;

 private:
  static StderrLogSink kLogSink;
  std::mutex mu_;
};

class FileLogSink final : public absl::LogSink {
 public:
  static absl::LogSink* get() { return &kLogSink; }
  ~FileLogSink() override;

  void Send(const absl::LogEntry& entry) override;

  // Call this before any logging occurs.
  static bool SetLogFile(std::string_view filename);

 private:
  static FileLogSink kLogSink;
  std::mutex mu_;
  std::ofstream log_file_;
};

extern std::ostringstream& get_nullstream();
extern void InitLogging(LogSink log_sink = LogSink::kStd,
                        std::string_view log_file = "");
extern absl::LogSink* GetLogSink();
}  // namespace logging
}  // namespace yrtr

// ABSL_LOG()
#define EXABSL_LOG_INTERNAL_LOG_IMPL(severity)           \
  ABSL_LOG_INTERNAL_CONDITION##severity(STATELESS, true) \
      ABSL_LOGGING_INTERNAL_LOG##severity                \
          .ToSinkOnly(yrtr::logging::GetLogSink())    \
          .InternalStream()

// ABSL_PLOG()
#define EXABSL_LOG_INTERNAL_PLOG_IMPL(severity)          \
  ABSL_LOG_INTERNAL_CONDITION##severity(STATELESS, true) \
      ABSL_LOGGING_INTERNAL_LOG##severity                \
          .ToSinkOnly(yrtr::logging::GetLogSink())    \
          .InternalStream()                              \
          .WithPerror()

// ABSL_DLOG()
#ifndef NDEBUG
#define EXABSL_LOG_INTERNAL_DLOG_IMPL(severity)          \
  ABSL_LOG_INTERNAL_CONDITION##severity(STATELESS, true) \
      ABSL_LOGGING_INTERNAL_DLOG##severity               \
          .ToSinkOnly(yrtr::logging::GetLogSink())    \
          .InternalStream()
#else
#define EXABSL_LOG_INTERNAL_DLOG_IMPL(severity)           \
  ABSL_LOG_INTERNAL_CONDITION##severity(STATELESS, false) \
      ABSL_LOGGING_INTERNAL_DLOG##severity                \
          .ToSinkOnly(yrtr::logging::GetLogSink())        \
          .InternalStream()
#endif

// ABSL_VLOG()
#define EXABSL_LOG_INTERNAL_VLOG_IMPL(verbose_level)                       \
  switch (const int absl_logging_internal_verbose_level = (verbose_level)) \
  case 0:                                                                  \
  default:                                                                 \
    EXABSL_LOG_INTERNAL_LOG_IF_IMPL(                                       \
        _INFO, ABSL_VLOG_IS_ON(absl_logging_internal_verbose_level))       \
        .WithVerbosity(absl_logging_internal_verbose_level)

#ifndef NDEBUG
#define EXABSL_LOG_INTERNAL_DVLOG_IMPL(verbose_level)                      \
  switch (const int absl_logging_internal_verbose_level = (verbose_level)) \
  case 0:                                                                  \
  default:                                                                 \
    EXABSL_LOG_INTERNAL_DLOG_IF_IMPL(                                      \
        _INFO, ABSL_VLOG_IS_ON(absl_logging_internal_verbose_level))       \
        .WithVerbosity(absl_logging_internal_verbose_level)
#else
#define EXABSL_LOG_INTERNAL_DVLOG_IMPL(verbose_level)                         \
  switch (const int absl_logging_internal_verbose_level = (verbose_level))    \
  case 0:                                                                     \
  default:                                                                    \
    EXABSL_LOG_INTERNAL_DLOG_IF_IMPL(                                         \
        _INFO, false && ABSL_VLOG_IS_ON(absl_logging_internal_verbose_level)) \
        .WithVerbosity(absl_logging_internal_verbose_level)
#endif

#define EXABSL_LOG_INTERNAL_LOG_IF_IMPL(severity, condition)  \
  ABSL_LOG_INTERNAL_CONDITION##severity(STATELESS, condition) \
      ABSL_LOGGING_INTERNAL_LOG##severity                     \
          .ToSinkOnly(yrtr::logging::GetLogSink())            \
          .InternalStream()

#define EXABSL_LOG_INTERNAL_PLOG_IF_IMPL(severity, condition) \
  ABSL_LOG_INTERNAL_CONDITION##severity(STATELESS, condition) \
      ABSL_LOGGING_INTERNAL_LOG##severity                     \
          .ToSinkOnly(yrtr::logging::GetLogSink())            \
          .WithPerror()                                       \
          .InternalStream()

#ifndef NDEBUG
#define EXABSL_LOG_INTERNAL_DLOG_IF_IMPL(severity, condition) \
  ABSL_LOG_INTERNAL_CONDITION##severity(STATELESS, condition) \
      ABSL_LOGGING_INTERNAL_DLOG##severity                    \
          .ToSinkOnly(yrtr::logging::GetLogSink())            \
          .InternalStream()
#else
#define EXABSL_LOG_INTERNAL_DLOG_IF_IMPL(severity, condition)            \
  ABSL_LOG_INTERNAL_CONDITION##severity(STATELESS, false && (condition)) \
      ABSL_LOGGING_INTERNAL_DLOG##severity                               \
          .ToSinkOnly(yrtr::logging::GetLogSink())                       \
          .InternalStream()
#endif

#define LOG_IF(severity, condition) \
  EXABSL_LOG_INTERNAL_LOG_IF_IMPL(_##severity, condition)

#define PLOG_IF(severity, condition) \
  EXABSL_LOG_INTERNAL_PLOG_IF_IMPL(_##severity, condition)

// LOG()
//
// `LOG` takes a single argument which is a severity level.  Data streamed in
// comprise the logged message.
// Example:
//
//   LOG(INFO) << "Found " << num_cookies << " cookies";
#define LOG(severity) EXABSL_LOG_INTERNAL_LOG_IMPL(_##severity)

// PLOG()
//
// `PLOG` behaves like `LOG` except that a description of the current state of
// `errno` is appended to the streamed message.
#define PLOG(severity) EXABSL_LOG_INTERNAL_PLOG_IMPL(_##severity)

// DLOG()
//
// `DLOG` behaves like `LOG` in debug mode (i.e. `#ifndef NDEBUG`).  Otherwise
// it compiles away and does nothing.  Note that `DLOG(FATAL)` does not
// terminate the program if `NDEBUG` is defined.
#define DLOG(severity) EXABSL_LOG_INTERNAL_DLOG_IMPL(_##severity)

// `VLOG` uses numeric levels to provide verbose logging that can configured at
// runtime, including at a per-module level.  `VLOG` statements are logged at
// `INFO` severity if they are logged at all; the numeric levels are on a
// different scale than the proper severity levels.  Positive levels are
// disabled by default.  Negative levels should not be used.
// Example:
//
//   VLOG(1) << "I print when you run the program with --v=1 or higher";
//   VLOG(2) << "I print when you run the program with --v=2 or higher";
//
// See vlog_is_on.h for further documentation, including the usage of the
// --vmodule flag to log at different levels in different source files.
//
// `VLOG` does not produce any output when verbose logging is not enabled.
// However, simply testing whether verbose logging is enabled can be expensive.
// If you don't intend to enable verbose logging in non-debug builds, consider
// using `DVLOG` instead.
#define VLOG(severity) EXABSL_LOG_INTERNAL_VLOG_IMPL(severity)

// `DVLOG` behaves like `VLOG` in debug mode (i.e. `#ifndef NDEBUG`).
// Otherwise, it compiles away and does nothing.
#define DVLOG(severity) EXABSL_LOG_INTERNAL_DVLOG_IMPL(severity)

// Extend abseil log macros
#define LOG_F(severity, f, ...) \
  EXABSL_LOG_INTERNAL_LOG_IMPL(_##severity) << std::format(f, ##__VA_ARGS__)
#define DLOG_F(severity, f, ...) \
  EXABSL_LOG_INTERNAL_DLOG_IMPL(_##severity) << std::format(f, ##__VA_ARGS__)
#define PLOG_F(severity, f, ...) \
  EXABSL_LOG_INTERNAL_PLOG_IMPL(_##severity) << std::format(f, ##__VA_ARGS__)
#define VLOG_F(severity, f, ...) \
  EXABSL_LOG_INTERNAL_VLOG_IMPL(severity) << std::format(f, ##__VA_ARGS__)
#define DVLOG_F(severity, f, ...) \
  EXABSL_LOG_INTERNAL_DVLOG_IMPL(severity) << std::format(f, ##__VA_ARGS__)

#define HLOG(lvl)     LOG_##lvl     << get_log_header()
#define HLOG_TRACE    LOG(TRACE)    << get_log_header()
#define HLOG_INFO     LOG(INFO)     << get_log_header()
#define HLOG_WARNING  LOG(WARNING)  << get_log_header()
#define HLOG_ERROR    LOG(ERROR)    << get_log_header()
#define HLOG_FATAL    LOG(FATAL)    << get_log_header()
#define HLOG_F(lvl, f, ...) HLOG_##lvl << std::format(f, ##__VA_ARGS__)
#define HVLOG(lvl) VLOG(lvl) << get_log_header()
#define HVLOG_F(lvl, f, ...) HVLOG(lvl) << std::format(f, ##__VA_ARGS__)

#ifdef YRTR_DEBUG
#define DLOG_TRACE    LOG(TRACE)
#define DLOG_INFO     LOG(INFO)
#define DLOG_WARNING  LOG(WARNING)
#define DLOG_ERROR    LOG(ERROR)
#define DLOG_FATAL    LOG(FATAL)
#define HDLOG(lvl)    DLOG_##lvl << get_log_header()
#define HDLOG_F(lvl, f, ...) HDLOG(lvl) << std::format(f, ##__VA_ARGS__)
#else
#define DLOG_TRACE
#define DLOG_INFO
#define DLOG_WARNING
#define DLOG_ERROR
#define DLOG_FATAL
#define HDLOG(lvl)
#define HDLOG_F(lvl, f, ...)
#endif

#define CHECK(cond) LOG_IF(FATAL, !(cond)) << "Check failed: " #cond
#define CHECK_EQ(a, b) LOG_IF(FATAL,  ((a)!=(b))) << std::format("Check failed: " #a "={} EQ " #b "={} ", (a), (b))
#define CHECK_NE(a, b) LOG_IF(FATAL,  ((a)==(b))) << std::format("Check failed: " #a "={} NE " #b "={} ", (a), (b))
#define CHECK_GE(a, b) LOG_IF(FATAL, !((a)>=(b))) << std::format("Check failed: " #a "={} GE " #b "={} ", (a), (b))
#define CHECK_GT(a, b) LOG_IF(FATAL, !((a)> (b))) << std::format("Check failed: " #a "={} GT " #b "={} ", (a), (b))
#define CHECK_LE(a, b) LOG_IF(FATAL, !((a)<=(b))) << std::format("Check failed: " #a "={} LE " #b "={} ", (a), (b))
#define CHECK_LT(a, b) LOG_IF(FATAL, !((a)< (b))) << std::format("Check failed: " #a "={} LT " #b "={} ", (a), (b))

#define DCHECK(cond) LOG_IF(FATAL, !(cond)) << "Check failed: " #cond
#define DCHECK_EQ(a, b) LOG_IF(FATAL,  ((a)!=(b))) << std::format("Check failed: " #a "={} EQ " #b "={} ", (a), (b))
#define DCHECK_NE(a, b) LOG_IF(FATAL,  ((a)==(b))) << std::format("Check failed: " #a "={} NE " #b "={} ", (a), (b))
#define DCHECK_GE(a, b) LOG_IF(FATAL, !((a)>=(b))) << std::format("Check failed: " #a "={} GE " #b "={} ", (a), (b))
#define DCHECK_GT(a, b) LOG_IF(FATAL, !((a)> (b))) << std::format("Check failed: " #a "={} GT " #b "={} ", (a), (b))
#define DCHECK_LE(a, b) LOG_IF(FATAL, !((a)<=(b))) << std::format("Check failed: " #a "={} LE " #b "={} ", (a), (b))
#define DCHECK_LT(a, b) LOG_IF(FATAL, !((a)< (b))) << std::format("Check failed: " #a "={} LT " #b "={} ", (a), (b))

#define PCHECK(cond) PLOG_IF(FATAL, !(cond)) << "Check failed: " #cond

#define CHECK_F(cond, f, ...) LOG_IF(FATAL, !(cond)) << "Check failed: " #cond " " << std::format(f, ##__VA_ARGS__)
#define CHECK_EQ_F(a, b, f, ...) LOG_IF(FATAL,  ((a)!=(b))) << std::format("Check failed: " #a "={} EQ " #b "={} ", (a), (b)) << std::format(f, ##__VA_ARGS__)
#define CHECK_NE_F(a, b, f, ...) LOG_IF(FATAL,  ((a)==(b))) << std::format("Check failed: " #a "={} NE " #b "={} ", (a), (b)) << std::format(f, ##__VA_ARGS__)
#define CHECK_GE_F(a, b, f, ...) LOG_IF(FATAL, !((a)>=(b))) << std::format("Check failed: " #a "={} GE " #b "={} ", (a), (b)) << std::format(f, ##__VA_ARGS__)
#define CHECK_GT_F(a, b, f, ...) LOG_IF(FATAL, !((a)> (b))) << std::format("Check failed: " #a "={} GT " #b "={} ", (a), (b)) << std::format(f, ##__VA_ARGS__)
#define CHECK_LE_F(a, b, f, ...) LOG_IF(FATAL, !((a)<=(b))) << std::format("Check failed: " #a "={} LE " #b "={} ", (a), (b)) << std::format(f, ##__VA_ARGS__)
#define CHECK_LT_F(a, b, f, ...) LOG_IF(FATAL, !((a)< (b))) << std::format("Check failed: " #a "={} LT " #b "={} ", (a), (b)) << std::format(f, ##__VA_ARGS__)

#ifdef YRTR_DEBUG
#define DCHECK_F(cond, f, ...)      CHECK_F(cond, f, ##__VA_ARGS__)
#define DCHECK_EQ_F(a, b, f, ...)   CHECK_EQ_F(a, b, f, ##__VA_ARGS__)
#define DCHECK_NE_F(a, b, f, ...)   CHECK_NE_F(a, b, f, ##__VA_ARGS__)
#define DCHECK_GE_F(a, b, f, ...)   CHECK_GE_F(a, b, f, ##__VA_ARGS__)
#define DCHECK_GT_F(a, b, f, ...)   CHECK_GT_F(a, b, f, ##__VA_ARGS__)
#define DCHECK_LE_F(a, b, f, ...)   CHECK_LE_F(a, b, f, ##__VA_ARGS__)
#define DCHECK_LT_F(a, b, f, ...)   CHECK_LT_F(a, b, f, ##__VA_ARGS__)
#else
#define DCHECK_F(cond, f, ...)
#define DCHECK_EQ_F(a, b, f, ...)
#define DCHECK_NE_F(a, b, f, ...)
#define DCHECK_GE_F(a, b, f, ...)
#define DCHECK_GT_F(a, b, f, ...)
#define DCHECK_LE_F(a, b, f, ...)
#define DCHECK_LT_F(a, b, f, ...)
#endif

#define CHECK_NOTNULL(var) CHECK(var != nullptr)
#ifdef YRTR_DEBUG
#define DCHECK_NOTNULL(var) DCHECK(var != nullptr)
#else
#define DCHECK_NOTNULL(var)
#endif

#define CHECK_DX(hr) CHECK_EQ(hr, D3D_OK)
