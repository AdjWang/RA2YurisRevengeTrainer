#pragma once
#include "macro.h"
#include "vendor.h"

namespace yrtr {

enum LogLevel {
    kDEBUG,
    kINFO,
    kWARN,
    kERROR,
    kFATAL,
};
constexpr static const char* kLogPrefix = "DIWEF";

#ifdef YRTR_LOG_FILE

template <class T, size_t N>
class RingBuffer {
public:
    RingBuffer()
        : size_(0),
          head_(0) {}
    
    void Push(T&& val) {
        buffer_[head_] = std::forward<T>(val);
        head_ = (head_ + 1) % N;
        size_++;
        if (size_ > N) {
            size_ = N;
        }
    }

    std::vector<T> PollAll() {
        std::vector<T> ret;
        if (size_ < N) {
            ret.resize(size_);
            for (size_t i = 0; i < size_; i++) {
                ret[i] = std::move(buffer_[i]);
            }
        } else {
            if (head_ == 0) {
                ret.resize(size_);
                for (size_t i = 0; i < size_; i++) {
                    ret[i] = std::move(buffer_[i]);
                }
            } else {
                ret.reserve(N);
                size_t p_data = head_;
                do {
                    ret.emplace_back(std::move(buffer_[p_data]));
                    p_data = (p_data + 1) % N;
                } while (p_data != head_);
            }
        }
        return ret;
    }

private:
    std::array<T, N> buffer_;
    size_t size_;
    size_t head_;
};

extern void DumpLog();
#endif

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
    Logger(LogLevel level, const char* filename, int line);
    Logger(bool, LogLevel level, const char* filename, int line);

    template <class... Args>
    Logger(bool, LogLevel level, const char* filename, int line,
           fmt::format_string<Args...>&& fmt_str, Args&&... args)
    : log_level_(level) {
        DWORD err_val = GetLastError();
        std::error_code ec(err_val, std::system_category());
        InitLogHeader(level);
        log_stream_ << fmt::format("{} {}:{}] ({}:{}) ", log_header_, filename,
                                   line, err_val, ec.message())
                    << fmt::format(
                           std::forward<fmt::format_string<Args...>>(fmt_str),
                           std::forward<Args>(args)...);
    }

    template <class... Args>
    Logger(LogLevel level, const char* filename, int line,
           fmt::format_string<Args...>&& fmt_str, Args&&... args)
    : log_level_(level) {
        InitLogHeader(level);
        log_stream_ << fmt::format("{} {}:{}] ", log_header_, filename, line)
                    << fmt::format(
                           std::forward<fmt::format_string<Args...>>(fmt_str),
                           std::forward<Args>(args)...);
    }

    ~Logger();

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

namespace {
constexpr const char* GetFileName(const char* file_path) {
    size_t p = 0;
    while (file_path[p++] != '\0');
    for (; p > 0; p--) {
        if (file_path[p] == '\\' || file_path[p] == '/') {
            return &file_path[p + 1];
        }
    }
    return file_path;
}
}  // namespace

// #define __LOG_FILE  __FILE_NAME__
#define __LOG_FILE  GetFileName(__FILE__)

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

#ifdef _DEBUG
#define DLOG(level, ...) LOG(level, ##__VA_ARGS__)
#else
#define DLOG(level, ...) DEVNULL
#endif

#define LOG_IF(lvl, cond) !(cond) ? DEVNULL : PLOG(lvl)

#define CHECK(cond) LOG_IF(FATAL, !(cond)) << "Check failed: " #cond " "
#define CHECK_EQ(a, b) LOG_IF(FATAL,  ((a)!=(b))) << fmt::format("Check failed: " #a "={} EQ " #b "={}", (a), (b))
#define CHECK_GE(a, b) LOG_IF(FATAL, !((a)>=(b))) << fmt::format("Check failed: " #a "={} GE " #b "={}", (a), (b))
#define CHECK_GT(a, b) LOG_IF(FATAL, !((a)> (b))) << fmt::format("Check failed: " #a "={} GT " #b "={}", (a), (b))
#define CHECK_LE(a, b) LOG_IF(FATAL, !((a)<=(b))) << fmt::format("Check failed: " #a "={} LE " #b "={}", (a), (b))
#define CHECK_LT(a, b) LOG_IF(FATAL, !((a)< (b))) << fmt::format("Check failed: " #a "={} LT " #b "={}", (a), (b))