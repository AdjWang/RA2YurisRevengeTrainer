#pragma once
#include "macro.h"
#include "vendor.h"

namespace yrtr {
namespace win32 {

class HandleGuard {
public:
    HandleGuard(HANDLE handle)
        : handle_(handle) {}
    HandleGuard(HandleGuard&&) = delete;
    HandleGuard& operator=(HandleGuard&&) = delete;
    
    HANDLE handle() const { return handle_; }

    ~HandleGuard() {
        CloseHandle(handle_);
    }
private:
    HANDLE handle_;

    DISALLOW_COPY_AND_ASSIGN(HandleGuard);
};

class MemoryAPI {
public:
    MemoryAPI(DWORD pid);
    MemoryAPI(MemoryAPI&&) = delete;
    MemoryAPI& operator=(MemoryAPI&&) = delete;

    BOOL CheckHandle() const;

    BOOL ReadMemory(DWORD address, std::span<uint8_t> data) const;
    BOOL ReadAddress(DWORD address, LPDWORD data) const;

    template <size_t N>
        requires(N >= 1)
    BOOL ReadAddress(DWORD address, std::array<DWORD, N> offsets,
                     LPDWORD data) const {
        CHECK(data != nullptr);
        DWORD addr_data;
        if (!ReadAddress(address, &addr_data)) {
            return FALSE;
        }
        for (DWORD offset : offsets | std::views::take(offsets.size() - 1)) {
            DWORD addr_next = addr_data + offset;
            if (!ReadAddress(addr_next, &addr_data)) {
                return FALSE;
            }
        }
        *data = addr_data + offsets[offsets.size() - 1];
        return TRUE;
    }

    BOOL WriteMemory(DWORD address, std::span<const uint8_t> data) const;
    BOOL WriteLongJump(DWORD addr_from, DWORD addr_to) const;

private:
    std::unique_ptr<HandleGuard> handle_;

    DISALLOW_COPY_AND_ASSIGN(MemoryAPI);
};

int clock_gettime(int X, struct timespec *tv);
std::filesystem::path GetDefaultFontPath();
BOOL GetProcessIDFromName(LPCSTR name, LPDWORD id);

}  // namespace win32
}  // namespace yrtr
