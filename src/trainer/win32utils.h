#pragma once
#include "logging.h"

#ifdef _WIN32
#include <windows.h>
#include <shlobj_core.h>
#endif

namespace win32 {
#ifdef _WIN32

// https://stackoverflow.com/questions/5404277/porting-clock-gettime-to-windows
inline LARGE_INTEGER getFILETIMEoffset()
{
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

// WARN: static variables are not thread-safe
inline int clock_gettime(int X, struct timespec *tv)
{
    LARGE_INTEGER           t;
    FILETIME                f;
    double                  microseconds;
    static LARGE_INTEGER    offset;
    static double           frequencyToMicroseconds;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 0;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = microseconds;
    tv->tv_sec = t.QuadPart / 1000000;
    // tv->tv_usec = t.QuadPart % 1000000;
    tv->tv_nsec = 0;
    return (0);
}

inline std::string ConvertWideCharToMultiByte(PWSTR wideString) {
    if (wideString == nullptr) {
        LOG(FATAL, "Input wide string is null.");
    }

    // Determine the length of the multibyte string
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideString, -1, nullptr, 0,
                                         nullptr, nullptr);
    if (bufferSize == 0) {
        LOG(FATAL, "Failed to determine buffer size for multibyte string.");
    }

    // Allocate buffer for the multibyte string
    std::string multiByteString(bufferSize, '\0');

    // Perform the conversion
    int result =
        WideCharToMultiByte(CP_UTF8, 0, wideString, -1, &multiByteString[0],
                            bufferSize, nullptr, nullptr);
    if (result == 0) {
        LOG(FATAL, "Failed to convert wide string to multibyte string.");
    }

    // Remove the null terminator added by WideCharToMultiByte
    multiByteString.resize(bufferSize - 1);

    return multiByteString;
}

inline std::filesystem::path GetDefaultFontPath() {
    PWSTR ppszPath;
    HRESULT res = SHGetKnownFolderPath(FOLDERID_Fonts, /*dwFlags*/ 0,
                                       /*hToken*/ NULL, &ppszPath);
    if (res != S_OK) {
        LOG(FATAL, "Failed to get font path with ret={}", res);
    }
    std::string path = ConvertWideCharToMultiByte(ppszPath);
    CoTaskMemFree(ppszPath);
    return path;
}

#endif
}  // namespace win32
