#pragma once
#include "vendor.h"

namespace yrtr {
namespace win32 {

class HandleGuard {
public:
    HandleGuard(HANDLE handle)
        : handle_(handle) {}
    
    HANDLE get() { return handle_; }

    ~HandleGuard() {
        CloseHandle(handle_);
    }
private:
    HANDLE handle_;
};

int clock_gettime(int X, struct timespec *tv);
std::filesystem::path GetDefaultFontPath();
BOOL GetProcessIDFromName(LPCSTR name, LPDWORD id);

}  // namespace win32
}  // namespace yrtr
