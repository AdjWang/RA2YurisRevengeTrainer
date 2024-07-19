#include "logging.h"
#include "win32utils.h"

namespace yrtr {

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
