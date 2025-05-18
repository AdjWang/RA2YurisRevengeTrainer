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
#include "toml++/toml.hpp"

namespace yrtr {
namespace backend {

class Config {
 public:
  static constexpr std::string_view kCfgFileName = "ra2_trainer_backend.toml";

  static Config* instance() { return inst_.get(); }
  static bool Load(const fs::path& cfg_dir);

  Config(const fs::path& cfg_path);

  const fs::path& hotreload_dir() const { return hotreload_dir_; }

  // Inputs a relative path, return absolute path relative to configuration file
  // directory.
  fs::path GetAbsolutePath(const fs::path& relpath) const;

 private:
  static std::unique_ptr<Config> inst_;
  static constexpr std::string_view kDefaultHotreloadDir =
      "ra2_trainer_hotreload";

  const fs::path cfg_dir_;
  const fs::path cfg_path_;

  fs::path hotreload_dir_;

  void LoadGlobal(const toml::table& global);

  DISALLOW_COPY_AND_ASSIGN(Config);
};

}  // namespace backend
}  // namespace yrtr
