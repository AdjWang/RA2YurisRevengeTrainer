#include "backend/game/beacon.h"

namespace yrtr {
namespace game {

bool Beacon::IsFull(int player_idx) const {
  for (int i = 0; i < 3; i++) {
    if (beacons[3 * player_idx + i] == nullptr) {
      return false;
    }
  }
  return true;
}

void Beacon::AutoClean(int player_idx) {
  if (IsFull(player_idx)) {
    for (int i = 0; i < 3; i++) {
      if (beacons[3 * player_idx + i] != nullptr) {
        Delete(player_idx, i);
        break;
      }
    }
  }
}

}  // namespace game
}  // namespace yrtr
