#include "backend/game/beacon.h"

namespace yrtr {
namespace game {

void Beacon::AutoClean(int player_idx) {
  if (beacons[3 * player_idx + 2] != nullptr) {
    Delete(player_idx, /*index*/ 0);
    // Move forward.
    beacons[3 * player_idx + 0] = beacons[3 * player_idx + 1];
    beacons[3 * player_idx + 1] = beacons[3 * player_idx + 2];
    beacons[3 * player_idx + 2] = nullptr;
  }
}

}  // namespace game
}  // namespace yrtr
