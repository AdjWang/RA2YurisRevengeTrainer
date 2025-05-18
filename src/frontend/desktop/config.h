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
#include "toml++/toml.hpp"

namespace yrtr {
namespace frontend {

class Config {
 public:
  static constexpr std::string_view kCfgFileName = "ra2_trainer.toml";
  static const Config* instance() { return inst_.get(); }
  static bool Load(const fs::path& cfg_dir);

  Config(const fs::path& cfg_path);
  Lang lang() const;
  bool enable_dpi_awareness() const { return enable_dpi_awareness_; }

  const fs::path& font_path() const { return font_path_; }
  const fs::path& fontex_path() const { return fontex_path_; }
  const fs::path& hotreload_dir() const { return hotreload_dir_; }

  // Inputs a relative path, return absolute path relative to configuration file
  // directory.
  fs::path GetAbsolutePath(const fs::path& relpath) const;

 private:
  static std::unique_ptr<Config> inst_;
  static constexpr std::string_view kDefaultLang = "zh";
  static constexpr std::string_view kDefaultFontPath = "";
  static constexpr std::string_view kDefaultFontExPath =
      "C:\\Windows\\Fonts\\seguiemj.ttf";
  static constexpr std::string_view kDefaultHotreloadDir =
      "ra2_trainer_hotreload";
  static constexpr std::array<std::string_view, 2> kAvailableLang{"zh", "en"};
  const fs::path cfg_dir_;
  const fs::path cfg_path_;
  std::string lang_;
  bool enable_dpi_awareness_;

  fs::path font_path_;
  fs::path fontex_path_;
  fs::path hotreload_dir_;

  void LoadGlobal(const toml::table& global);

  DISALLOW_COPY_AND_ASSIGN(Config);
};

}  // namespace frontend
}  // namespace yrtr
