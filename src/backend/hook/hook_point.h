// List all hook points here to detect conflictions easily.
#pragma once
#include <cstdint>
#include <utility>

namespace yrtr {
namespace backend {
namespace hook {
// Hook at where and how many asm code bytes the hook point occupies.
using HookPoint = std::pair<uint32_t /*addr*/, uint32_t /*bytes*/>;

constexpr uint32_t GetJumpBack(HookPoint hp) {
  return hp.first + hp.second;
}

// Assume the executable's image base always be 0x00400000.
static constexpr uint32_t kImageBase = 0x00400000;

static constexpr HookPoint kHpBuiltinLogging = {0x004068E0, 5};
static constexpr HookPoint kHpUpdate = {0x007E1530, 5};
static constexpr HookPoint kHpExitGame = {0x007E14AC, 5};

static constexpr HookPoint kHpSellTheWorldCursor = {0x006929D1, 7};
static constexpr HookPoint kHpSellTheWorldBlong = {0x004C6F48, 6};
static constexpr HookPoint kHpSellTheWorldBuilder = {0x0044711B, 6};
static constexpr HookPoint kHpGodPlayer = {0x005F5509, 7};
// Chrono legionnaire check attackable.
static constexpr HookPoint kHpCanWrapTarget = {0x0071AE9D, 7};
static constexpr HookPoint kHpInstBuild = {0x004C9B92, 5};
static constexpr HookPoint kHpInstFire = {0x006FC955, 6};
static constexpr HookPoint kHpRangeToYourBase = {0x006F7248, 6};
static constexpr HookPoint kHpFireToYourBase = {0x0070138F, 7};
static constexpr HookPoint kHpFreezeGapGenerator = {0x006FAF0D, 6};
static constexpr HookPoint kHpBuildEveryWhereGround = {0x004A8EB0, 5};
static constexpr HookPoint kHpBuildEveryWhereWater = {0x0047C9CD, 7};
static constexpr HookPoint kHpAutoRepairNeutral = {0x00452644, 6};
static constexpr HookPoint kHpCapturedMine = {0x00519F71, 6};
static constexpr HookPoint kHpSocialismMajestyCome = {0x004692BD, 6};
static constexpr HookPoint kHpSocialismMajestyBack = {0x00471FF0, 8};
static constexpr HookPoint kHpGarrisonedMine = {0x0045831A, 6};
static constexpr HookPoint kHpInvadeMode = {0x006F85DD, 5};
static constexpr HookPoint kHpUnlimitTech = {0x004F7870, 7};
static constexpr HookPoint kHpFastReload = {0x006B7D08, 6};
static constexpr HookPoint kHpMoreAmmunition = {0x006B6D0A, 6};
static constexpr HookPoint kHpInstChronoMove = {0x00719579, 7};
static constexpr HookPoint kHpInstChronoAttack = {0x0071AFB7, 5};
static constexpr HookPoint kHpSpySpy = {0x0051A002, 6};
static constexpr HookPoint kHpEverythingElited = {0x0075001A, 5};

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
