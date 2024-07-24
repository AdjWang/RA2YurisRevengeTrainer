#include "logging.h"
#include "win32utils.h"

namespace yrtr {

#ifdef YRTR_LOG_FILE
static RingBuffer<std::string, 50> kLogBuffer;

void DumpLog() {
    std::vector<std::string> logs = kLogBuffer.PollAll();
    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, "dbglog.txt", "w");
    if (err != 0) {
        perror("Failed to open output log file");
    }
    for (const std::string& line : logs) {
        fprintf(fp, "%s", line.c_str());
    }
    fclose(fp);
}
#endif

// NOTE: DO NOT use LOG macro here as a recursive invocation
Logger::Logger(LogLevel level, const char* filename, int line)
    : log_level_(level) {
    InitLogHeader(level);
    log_stream_ << fmt::format("{} {}:{}] ", log_header_, filename, line);
}

Logger::Logger(bool, LogLevel level, const char* filename, int line)
    : log_level_(level) {
    DWORD err_val = GetLastError();
    std::error_code ec(err_val, std::system_category());
    InitLogHeader(level);
    log_stream_ << fmt::format("{} {}:{}] ({}:{}) ", log_header_, filename,
                               line, err_val, ec.message());
}

Logger::~Logger() {
#ifdef YRTR_LOG_STD
    ::printf("%s\n", log_stream_.str().c_str());
    ::fflush(stdout);
#elif YRTR_LOG_DBGVIEW
    log_stream_ << "\n";
    ::OutputDebugString(log_stream_.str().c_str());
#endif
#ifdef YRTR_LOG_FILE
    kLogBuffer.Push(log_stream_.str());
#endif
    if (UNLIKELY(log_level_ == LogLevel::kFATAL)) {
#ifdef YRTR_LOG_FILE
        DumpLog();
#endif
        ::abort();
    }
}

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif
void Logger::InitLogHeader(LogLevel level) {
    struct timespec ts;
    if (UNLIKELY(win32::clock_gettime(CLOCK_REALTIME, &ts) == -1)) {
        ::perror("clock_gettime failed");
        ::abort();
    }
    struct tm datetime;
    // NOTE: win32 function! not gnu's localtime_r(&ts.tv_sec, &datetime) == NULL
    if (UNLIKELY(localtime_s(&datetime, &ts.tv_sec) != NULL)) {
        ::perror("localtime_r failed");
        ::abort();
    }
    log_header_[0] = kLogPrefix[level];
    // 13 bytes time string and 1 byte '\0'
    strftime(log_header_ + 1, 14, "%m%d %H:%M:%S", &datetime);
    log_header_[14] = '.';
    // warning C4996: 'sprintf': This function or variable may be unsafe.
    #pragma warning(push)
    #pragma warning(disable:4996)   // yeah!
    sprintf(log_header_ + 15, "%06d", static_cast<int>(ts.tv_nsec / 1000));
    #pragma warning(pop)
    log_header_[21] = '\0';
}

}  // namespace yrtr
