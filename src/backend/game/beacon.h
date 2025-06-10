#pragma once
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "ASMMacros.h"
#include "Helpers/CompileTime.h"
__YRTR_END_THIRD_PARTY_HEADERS

namespace yrtr {
namespace game {

#pragma warning(push)
// unreferenced formal parameter
#pragma warning(disable : 4100)
// frame pointer register 'ebp' modified by inline assembly code
#pragma warning(disable : 4731)

struct Beacon {
  static constexpr yrpp::reference<Beacon, 0x89C3B0u> Instance{};
  void* beacons[24];

  // From VA:0x430F30
  bool IsFull(int player_idx) const;
  // Input index range is [-1, 3), if index == -1, automatically find the
  // first slot to place.
  void Place(int player_idx, int coord_x, int coord_y, int coord_z, int index) {
    JMP_THIS(0x430BA0);
  }
  // Input index range is [0, 3), or input player_idx=-1 && index=-1 to get
  // last empty entry.
  void Delete(int player_idx, int index) { JMP_THIS(0x4311C0); }
  void DeleteAll(int player_idx) { JMP_THIS(0x431410); }
  void ShowText(const wchar_t* text, int player_idx, int index,
                char is_sender) {
    JMP_THIS(0x431450);
  }

  void AutoClean(int player_idx);
};

#pragma warning(pop)

}  // namespace game
}  // namespace yrtr
