#pragma once
#include <cstdint>
#include <memory>
#include <ranges>

#include "base/logging.h"
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/container/inlined_vector.h"
#include "absl/container/flat_hash_map.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "backend/hook/hook_point.h"

namespace yrtr {
namespace hook {

class HandleGuard;

class MemoryAPI {
 public:
  static MemoryAPI* instance() { return inst_.get(); }
  static void Init();
  static void Destroy();
  MemoryAPI();
  ~MemoryAPI();
  MemoryAPI(MemoryAPI&&) = delete;
  MemoryAPI& operator=(MemoryAPI&&) = delete;

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
    requires(!std::is_convertible_v<T, std::span<const uint8_t>>)
  bool WriteMemory(uint32_t address, T data) const {
    return WriteMemory(
        address,
        std::span<const uint8_t>(reinterpret_cast<uint8_t*>(&data), sizeof(T)));
  }

  bool HasHook(const HookPoint hook_point) const;
  bool HookJump(const HookPoint hook_point, void* dest);
  bool HookNop(const HookPoint hook_point);
  bool RestoreHook(const HookPoint hook_point);

  bool CheckHandle() const;
  bool AutoAssemble(std::string_view script, bool activate) const;

 private:
  static std::unique_ptr<MemoryAPI> inst_;
  std::unique_ptr<HandleGuard> handle_;
  absl::flat_hash_map<uint32_t /*addr*/, absl::InlinedVector<uint8_t, 10>>
      hooks_;
  
  bool WriteHook(uint32_t addr, std::span<const uint8_t> code);
  // For reclaiming resources.
  void RestoreAllHooks();

  DISALLOW_COPY_AND_ASSIGN(MemoryAPI);
};

}  // namespace hook
}  // namespace yrtr
