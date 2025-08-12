#pragma once
#include <array>
#include <filesystem>
namespace fs = std::filesystem;
#include <memory>
#include <string>
#include <string_view>

#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/container/flat_hash_set.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "frontend/desktop/char_table.h"
#include "frontend/desktop/glfw.h"
#include "toml++/toml.hpp"

namespace yrtr {
namespace frontend {

class Config {
 public:
  static constexpr std::string_view kCfgFileName = "ra2_trainer.toml";
  static constexpr int kWin32HotKeyMod = MOD_ALT;

  constexpr int GetHotkey(FnLabel label) {
    switch(label) {
      case FnLabel::kApply:               return GLFW_KEY_1;
      case FnLabel::kIAMWinner:           return GLFW_KEY_2;
      case FnLabel::kDeleteUnit:          return GLFW_KEY_3;
      case FnLabel::kClearShroud:         return GLFW_KEY_4;
      case FnLabel::kGiveMeABomb:         return GLFW_KEY_5;
      case FnLabel::kUnitLevelUp:         return GLFW_KEY_6;
      case FnLabel::kUnitSpeedUp:         return GLFW_KEY_7;
      case FnLabel::kFastBuild:           return GLFW_KEY_8;
      case FnLabel::kThisIsMine:          return GLFW_KEY_9;
      case FnLabel::kGod:                 return GLFW_KEY_Q;
      case FnLabel::kInstBuild:           return GLFW_KEY_W;
      case FnLabel::kUnlimitSuperWeapon:  return GLFW_KEY_E;
      case FnLabel::kInstFire:            return GLFW_KEY_T;
      case FnLabel::kInstTurn:            return GLFW_KEY_Y;
      case FnLabel::kRangeToYourBase:     return GLFW_KEY_U;
      case FnLabel::kFireToYourBase:      return GLFW_KEY_I;
      case FnLabel::kFreezeGapGenerator:  return GLFW_KEY_O;
      case FnLabel::kSellTheWorld:        return GLFW_KEY_A;
      case FnLabel::kBuildEveryWhere:     return GLFW_KEY_D;
      case FnLabel::kAutoRepair:          return GLFW_KEY_F;
      case FnLabel::kSocialismMajesty:    return GLFW_KEY_H;
      case FnLabel::kMakeGarrisonedMine:  return GLFW_KEY_L;
      case FnLabel::kInvadeMode:          return GLFW_KEY_Z;
      case FnLabel::kUnlimitTech:         return GLFW_KEY_X;
      case FnLabel::kUnlimitFirePower:    return GLFW_KEY_V;
      case FnLabel::kInstChrono:          return GLFW_KEY_B;
      case FnLabel::kSpySpy:              return GLFW_KEY_N;
      case FnLabel::kAdjustGameSpeed:     return GLFW_KEY_PERIOD;   /* . */
      default: return GLFW_KEY_UNKNOWN;
    }
  }

  constexpr const char8_t* KeyToString(int key) {
    switch (key) {
      case GLFW_KEY_1: return u8"1";
      case GLFW_KEY_2: return u8"2";
      case GLFW_KEY_3: return u8"3";
      case GLFW_KEY_4: return u8"4";
      case GLFW_KEY_5: return u8"5";
      case GLFW_KEY_6: return u8"6";
      case GLFW_KEY_7: return u8"7";
      case GLFW_KEY_8: return u8"8";
      case GLFW_KEY_9: return u8"9";
      case GLFW_KEY_0: return u8"0";
      case GLFW_KEY_Q: return u8"Q";
      case GLFW_KEY_W: return u8"W";
      case GLFW_KEY_E: return u8"E";
      case GLFW_KEY_R: return u8"R";
      case GLFW_KEY_T: return u8"T";
      case GLFW_KEY_Y: return u8"Y";
      case GLFW_KEY_U: return u8"U";
      case GLFW_KEY_I: return u8"I";
      case GLFW_KEY_O: return u8"O";
      case GLFW_KEY_P: return u8"P";
      case GLFW_KEY_A: return u8"A";
      case GLFW_KEY_S: return u8"S";
      case GLFW_KEY_D: return u8"D";
      case GLFW_KEY_F: return u8"F";
      case GLFW_KEY_G: return u8"G";
      case GLFW_KEY_H: return u8"H";
      case GLFW_KEY_J: return u8"J";
      case GLFW_KEY_K: return u8"K";
      case GLFW_KEY_L: return u8"L";
      case GLFW_KEY_Z: return u8"Z";
      case GLFW_KEY_X: return u8"X";
      case GLFW_KEY_C: return u8"C";
      case GLFW_KEY_V: return u8"V";
      case GLFW_KEY_B: return u8"B";
      case GLFW_KEY_N: return u8"N";
      case GLFW_KEY_M: return u8"M";
      case GLFW_KEY_COMMA: return u8",";
      case GLFW_KEY_PERIOD: return u8".";
      default: return u8"unknown";
    }
  }

  static Config* instance() { return inst_.get(); }
  static bool Load(const fs::path& cfg_dir);

  Config(const fs::path& cfg_path);
  Lang lang() const;
  bool enable_dpi_awareness() const { return enable_dpi_awareness_; }

  uint16_t port() const { return port_; }
  const fs::path& font_path() const { return font_path_; }
  const fs::path& fontex_path() const { return fontex_path_; }

  void DisableHotkeyGUI(int key);
  std::string GetFnStrWithKey(FnLabel label);
  // Inputs a relative path, return absolute path relative to configuration file
  // directory.
  fs::path GetAbsolutePath(const fs::path& relpath) const;

 private:
  static std::unique_ptr<Config> inst_;
  static constexpr std::string_view kDefaultLang = "zh";
  static constexpr std::string_view kDefaultFontPath = "";
  static constexpr std::string_view kDefaultFontExPath =
      "C:\\Windows\\Fonts\\seguiemj.ttf";
  static constexpr std::array<std::string_view, 2> kAvailableLang{"zh", "en"};

  const fs::path cfg_dir_;
  const fs::path cfg_path_;
  std::string lang_;
  bool enable_dpi_awareness_;
  uint16_t port_;

  fs::path font_path_;
  fs::path fontex_path_;

  absl::flat_hash_set<int> disabled_hot_key_;

  void LoadGlobal(const toml::table& global);

  DISALLOW_COPY_AND_ASSIGN(Config);
};

}  // namespace frontend
}  // namespace yrtr
