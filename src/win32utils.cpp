#include "logging.h"
#include "win32utils.h"

#include <windows.h>
#include <shlobj_core.h>
#include <TlHelp32.h>

namespace yrtr {
namespace win32 {

MemoryAPI::MemoryAPI(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (hProcess == NULL) {
        PLOG(WARN, "OpenProcess pid={} failed", pid);
        return;
    }
    handle_.reset(new HandleGuard(hProcess));
}

bool MemoryAPI::CheckHandle() const {
    return handle_ != nullptr;
}

#define CHECK_HANDLE_OR_RETURN_FALSE() \
    if (!CheckHandle()) {              \
        LOG(WARN, "Invalid handle");   \
        return false;                  \
    }

bool MemoryAPI::ReadMemory(uint32_t address, std::span<uint8_t> data) const {
    CHECK_HANDLE_OR_RETURN_FALSE();
    DWORD oldprotect;
    if (!VirtualProtectEx(handle_->handle(), (void *)address, data.size(),
                          PAGE_EXECUTE_READWRITE, &oldprotect)) {
        PLOG(WARN, "VirtualProtectEx failed on addr={}", (void *)address);
        return false;
    }
    if (!ReadProcessMemory(handle_->handle(), (void *)address, data.data(),
                           data.size(), nullptr)) {
        PLOG(WARN, "ReadProcessMemory failed on addr={}", (void *)address);
        return false;
    }
    if (!VirtualProtectEx(handle_->handle(), (void *)address, data.size(),
                          oldprotect, &oldprotect)) {
        PLOG(WARN, "VirtualProtectEx failed on addr={}", (void *)address);
        return false;
    }
    return true;
}

bool MemoryAPI::ReadAddress(uint32_t address, uint32_t* data) const {
    CHECK_HANDLE_OR_RETURN_FALSE();
    CHECK(data != nullptr);
    return ReadMemory(address,
                      std::span<uint8_t>(reinterpret_cast<uint8_t *>(data),
                                         sizeof(uint32_t *)));
}

bool MemoryAPI::WriteMemory(uint32_t address,
                            std::span<const uint8_t> data) const {
    CHECK_HANDLE_OR_RETURN_FALSE();
    DWORD oldprotect;
    if (!VirtualProtectEx(handle_->handle(), (void *)address, data.size(),
                          PAGE_EXECUTE_READWRITE, &oldprotect)) {
        PLOG(WARN, "VirtualProtectEx failed on addr={}", (void *)address);
        return false;
    }
    if (!WriteProcessMemory(handle_->handle(), (void *)address, data.data(),
                            data.size(), nullptr)) {
        PLOG(WARN, "WriteProcessMemory failed on addr={}", (void *)address);
        return false;
    }
    if (!VirtualProtectEx(handle_->handle(), (void *)address, data.size(),
                          oldprotect, &oldprotect)) {
        PLOG(WARN, "VirtualProtectEx failed on addr={}", (void *)address);
        return false;
    }
    return true;
}

bool MemoryAPI::WriteLongJump(uint32_t addr_from, uint32_t addr_to) const {
    CHECK_HANDLE_OR_RETURN_FALSE();
    static_assert(sizeof(void*) == 4);
    uint8_t asm_code[5] = {0xE9};   // long jmp
    memcpy(&asm_code[1], reinterpret_cast<void*>(addr_to), sizeof(void*));
    return WriteMemory(addr_from, std::span<uint8_t>(asm_code, 5));
}

#undef CHECK_HANDLE_OR_RETURN_FALSE

namespace {
// https://stackoverflow.com/questions/5404277/porting-clock-gettime-to-windows
static inline LARGE_INTEGER getFILETIMEoffset() {
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

static std::string ConvertWideCharToMultiByte(PWSTR wideString) {
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
}  // namespace

// WARN: static variables are not thread-safe
int clock_gettime(int X, struct timespec *tv) {
    LARGE_INTEGER t;
    FILETIME f;
    double microseconds;
    static LARGE_INTEGER offset;
    static double frequencyToMicroseconds;
    static int initialized = 0;
    static BOOL usePerformanceCounter = 0;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter =
            QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds =
                (double)performanceFrequency.QuadPart / 1000000.0;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter)
        QueryPerformanceCounter(&t);
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

std::filesystem::path GetDefaultFontPath() {
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

BOOL GetProcessIDFromName(LPCSTR name, LPDWORD id) {
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE) {
        PLOG(WARN, "CreateToolhelp32Snapshot failed");
        return FALSE;
    }
    HandleGuard handle(hSnapShot);
    PROCESSENTRY32 pInfo;
    pInfo.dwSize = sizeof(pInfo);
    // Iterate through process list
    if (!Process32First(handle.handle(), &pInfo)) {
        PLOG(WARN, "Process32First failed");
        return FALSE;
    }
    CHECK(name != nullptr);
    CHECK(id != nullptr);
    do {
        // DLOG(INFO, "process name={}", pInfo.szExeFile);
        if (stricmp(pInfo.szExeFile, name) == 0) {
            *id = pInfo.th32ProcessID;
            return TRUE;
        }
    } while (Process32Next(handle.handle(), &pInfo) != FALSE);
    if (GetLastError() != ERROR_NO_MORE_FILES) {
        PLOG(WARN, "Process32Next failed");
    }
    return FALSE;
}

}  // namespace win32
}  // namespace yrtr
