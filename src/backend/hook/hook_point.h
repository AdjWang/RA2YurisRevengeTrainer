// List all hook points here to detect conflictions easily.
// Assume the executable's image base is always 0x00400000.
#pragma once
#include <cstdint>
#include <utility>

namespace yrtr {
namespace hook {
// Hook at where and how many asm code bytes the hook point occupies.
using HookPoint = std::pair<uint32_t /*addr*/, uint32_t /*bytes*/>;

constexpr uint32_t GetJumpBack(HookPoint hp) {
  return hp.first + hp.second;
}

static constexpr HookPoint kHpBuiltinLogging = {0x004068E0, 5};
static constexpr HookPoint kHpCreateWindow = {0x00777C30, 5};
static constexpr HookPoint kHpGetCursorPos = {0x007BA0ED, 7};
static constexpr HookPoint kHpRender = {0x006D465F, 5};
static constexpr HookPoint kHpUpdate = {0x0055DE40, 5};
static constexpr HookPoint kHpExitGame = {0x006BE0CB, 17};

}  // namespace hook
}  // namespace yrtr
