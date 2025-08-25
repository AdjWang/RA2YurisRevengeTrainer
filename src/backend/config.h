#pragma once
#include <array>
#include <filesystem>
namespace fs = std::filesystem;
#include <memory>
#include <string>
#include <string_view>

#include "backend/tech.h"
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
  static constexpr std::string_view kLogFileName = "ra2_trainer_backend.log";

  static Config* instance() { return inst_.get(); }
  static bool Load(const fs::path& cfg_dir);

  Config(const fs::path& cfg_path);

  uint16_t port() const { return port_; }
  const fs::path& hotreload_dir() const { return hotreload_dir_; }
  bool auto_record() const { return auto_record_; }
  fs::path record_path() const { return cfg_dir_ / kDefaultRecordFilename; }
  const TechList& tech_list() const { return tech_list_; }

  // Inputs a relative path, return absolute path relative to configuration file
  // directory.
  fs::path GetAbsolutePath(const fs::path& relpath) const;

 private:
  static std::unique_ptr<Config> inst_;
  static constexpr std::string_view kDefaultHotreloadDir =
      "ra2_trainer_hotreload";
  static constexpr std::string_view kDefaultRecordFilename =
      "ra2_trainer_record.toml";

  const fs::path cfg_dir_;
  const fs::path cfg_path_;

  uint16_t port_;
  fs::path hotreload_dir_;
  bool auto_record_;
  TechList tech_list_;

  void LoadGlobal(const toml::table& global);
  void LoadTechList(const toml::table& tech_tb);

  DISALLOW_COPY_AND_ASSIGN(Config);
};

}  // namespace backend
}  // namespace yrtr
