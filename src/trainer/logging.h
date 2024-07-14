#pragma once
#include "vendor.h"
#include "macro.h"

namespace yrtr {

enum LogLevel {
    kDEBUG,
    kINFO,
    kWARN,
    kERROR,
    kFATAL,
};
constexpr static const char* kLogPrefix = "DIWEF";

class DummyLogger {
public:
    DummyLogger(LogLevel, const char*, int) {}
    DummyLogger(bool, LogLevel, const char*, int) {}
    std::ostream& stream() { return null_stream_; }
private:
    std::ostream null_stream_{nullptr};
    DISALLOW_COPY_AND_ASSIGN(DummyLogger);
};

class Logger {
public:
    // NOTE: DO NOT use LOG macro here as a recursive invocation
    Logger(LogLevel level, const char* filename, int line)
    : log_level_(level) {
        InitLogHeader(level);
        log_stream_ << std::format("{} {}:{}] ", log_header_, filename, line);
    }

    Logger(bool, LogLevel level, const char* filename, int line)
    : log_level_(level) {
        DWORD err_val = GetLastError();
        std::error_code ec(err_val, std::system_category());
        std::system_error err(ec);
        InitLogHeader(level);
        log_stream_ << std::format("{} {}:{}] ({}:{}) ", log_header_, filename,
                                   line, err.what(),
                                   ec.message());
    }

    template <class... Args>
    Logger(bool, LogLevel level, const char* filename, int line,
           std::format_string<Args...>&& fmt_str, Args&&... args)
    : log_level_(level) {
        DWORD err_val = GetLastError();
        std::error_code ec(err_val, std::system_category());
        std::system_error err(ec);
        InitLogHeader(level);
        log_stream_ << std::format("{} {}:{}] ({}:{}) ", log_header_, filename,
                                   line, err.what(),
                                   ec.message())
                    << std::format(
                           std::forward<std::format_string<Args...>>(fmt_str),
                           std::forward<Args>(args)...);
    }

    template <class... Args>
    Logger(LogLevel level, const char* filename, int line,
           std::format_string<Args...>&& fmt_str, Args&&... args)
    : log_level_(level) {
        InitLogHeader(level);
        log_stream_ << std::format("{} {}:{}] ", log_header_, filename, line)
                    << std::format(
                           std::forward<std::format_string<Args...>>(fmt_str),
                           std::forward<Args>(args)...);
    }

    ~Logger() {
        ::printf("%s\n", log_stream_.str().c_str());
        ::fflush(stdout);
        if (UNLIKELY(log_level_ == LogLevel::kFATAL)) {
            ::abort();
        }
    }

    std::ostream& stream() { return log_stream_; }

private:
    LogLevel log_level_;
    char log_header_[22];
    std::stringstream log_stream_;

    void InitLogHeader(LogLevel level);

    // Prevent allocate on heap
    void operator delete(void*);

    DISALLOW_COPY_AND_ASSIGN(Logger);
};

}  // namespace yrtr

// #define __LOG_FILE  __FILE_NAME__
#define __LOG_FILE  __FILE__

#define DEVNULL \
    yrtr::DummyLogger(yrtr::LogLevel::kINFO, __LOG_FILE, __LINE__).stream()

#define LOG(level, ...)                                          \
    yrtr::Logger(yrtr::LogLevel::k##level, __LOG_FILE, __LINE__, \
                 ##__VA_ARGS__)                                  \
        .stream()

#define PLOG(level, ...)                                               \
    yrtr::Logger(true, yrtr::LogLevel::k##level, __LOG_FILE, __LINE__, \
                 ##__VA_ARGS__)                                        \
        .stream()

#ifdef __DEBUG
#define DLOG(level) LOG(level)
#else
#define DLOG(level) DEVNULL
#endif

#define LOG_IF(lvl, cond) !(cond) ? DEVNULL : LOG(lvl)

#define CHECK(cond) LOG_IF(FATAL, !(cond)) << "Check failed: " #cond " "
#define CHECK_EQ(a, b) LOG_IF(FATAL,  ((a)!=(b))) << std::format("Check failed: " #a "={} EQ " #b "={}", (a), (b))
#define CHECK_GE(a, b) LOG_IF(FATAL, !((a)>=(b))) << std::format("Check failed: " #a "={} GE " #b "={}", (a), (b))
#define CHECK_GT(a, b) LOG_IF(FATAL, !((a)> (b))) << std::format("Check failed: " #a "={} GT " #b "={}", (a), (b))
#define CHECK_LE(a, b) LOG_IF(FATAL, !((a)<=(b))) << std::format("Check failed: " #a "={} LE " #b "={}", (a), (b))
#define CHECK_LT(a, b) LOG_IF(FATAL, !((a)< (b))) << std::format("Check failed: " #a "={} LT " #b "={}", (a), (b))