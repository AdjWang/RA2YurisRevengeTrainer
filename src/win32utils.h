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

    bool CheckHandle() const;

    bool ReadMemory(uint32_t address, std::span<uint8_t> data) const;

    template <class T>
        requires(!std::is_same_v<T, std::span<uint8_t>>)
    bool ReadMemory(uint32_t address, T* data) const {
        return ReadMemory(
            address,
            std::span<uint8_t>(reinterpret_cast<uint8_t*>(data), sizeof(T)));
    }

    bool ReadAddress(uint32_t address, uint32_t* data) const;

    template <size_t N>
        requires(N >= 1)
    bool ReadAddress(uint32_t address, uint32_t const (&offsets)[N],
                     uint32_t* addr_output) const {
        CHECK(addr_output != nullptr);
        uint32_t addr_data;
        if (!ReadAddress(address, &addr_data)) {
            return false;
        }
        for (uint32_t offset : offsets | std::views::take(N - 1)) {
            uint32_t addr_next = addr_data + offset;
            if (!ReadAddress(addr_next, &addr_data)) {
                return false;
            }
        }
        *addr_output = addr_data + offsets[N - 1];
        return true;
    }

    template <size_t N>
        requires(N >= 1)
    bool ReadMemory(uint32_t address, uint32_t const (&offsets)[N],
                    uint32_t* data) const {
        CHECK(data != nullptr);
        uint32_t addr_output;
        if (!ReadAddress(address, offsets, &addr_output)) {
            return false;
        }
        return ReadMemory(addr_output, data);
    }

    bool WriteMemory(uint32_t address, std::span<const uint8_t> data) const;

    template <class T>
        requires(!std::is_same_v<T, std::span<const uint8_t>>)
    bool WriteMemory(uint32_t address, T data) const {
        return WriteMemory(
            address, std::span<const uint8_t>(reinterpret_cast<uint8_t*>(&data),
                                              sizeof(T)));
    }

    bool WriteLongJump(uint32_t addr_from, uint32_t addr_to) const;

private:
    std::unique_ptr<HandleGuard> handle_;

    DISALLOW_COPY_AND_ASSIGN(MemoryAPI);
};

int clock_gettime(int X, struct timespec *tv);
std::filesystem::path GetDefaultFontPath();
BOOL GetProcessIDFromName(LPCSTR name, LPDWORD id);

}  // namespace win32
}  // namespace yrtr
