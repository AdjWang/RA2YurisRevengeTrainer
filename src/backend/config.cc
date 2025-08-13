#include "backend/config.h"

#include <optional>

#include "base/logging.h"
#include "formatter/std.h"
#include "gsl/narrow"

namespace yrtr {
namespace backend {

namespace {
std::string_view get_log_header() { return "Config "; }

template <class T>
std::optional<T> TryLoad(const toml::table& tb, std::string_view key) {
  auto raw_val = tb.get_as<T>(key);
  if (raw_val) {
    return raw_val->get();
  } else {
    return std::nullopt;
  }
}

template <class T>
T CheckLoad(const toml::table& tb, std::string_view key) {
  auto raw_val = tb.get_as<T>(key);
  CHECK_NOTNULL(raw_val);
  if constexpr (std::is_integral_v<std::remove_pointer_t<decltype(raw_val)>>) {
    return *raw_val;
  } else {
    return raw_val->get();
  }
}
}  // namespace

std::unique_ptr<Config> Config::inst_;

bool Config::Load(const fs::path& cfg_dir) {
  fs::path cfg_path = cfg_dir / kCfgFileName;
  if (!fs::exists(cfg_path)) {
    HLOG_F(ERROR, "Not found config file={}", fs::absolute(cfg_path));
    return false;
  }
  inst_ = std::make_unique<Config>(cfg_path);
  HLOG_F(INFO, "Load from path={}", fs::canonical(cfg_path));
  auto cfg = toml::parse_file(cfg_path.string());
  if (toml::table* global = cfg["ra2_trainer"].as_table()) {
    inst_->LoadGlobal(*global);
  } else {
    HLOG_F(ERROR, "Failed to load table [ra2_trainer]");
    return false;
  }
  if (toml::table* tech_tb = cfg["tech"].as_table()) {
    inst_->LoadTechList(*tech_tb);
  } else {
    HLOG_F(ERROR, "Failed to load table [tech]");
    return false;
  }
  return true;
}

// Default configurations.
Config::Config(const fs::path& cfg_path)
    : cfg_dir_(cfg_path.parent_path()),
      cfg_path_(cfg_path),
      hotreload_dir_(kDefaultHotreloadDir) {}

fs::path Config::GetAbsolutePath(const fs::path& relpath) const {
  if (relpath.is_absolute()) {
    return relpath;
  } else {
    return cfg_dir_ / relpath;
  }
}

void Config::LoadGlobal(const toml::table& global) {
  port_ = gsl::narrow_cast<uint16_t>(CheckLoad<int64_t>(global, "port"));
  hotreload_dir_ =
      cfg_dir_ / TryLoad<std::string>(global, "hotreload_directory")
                     .value_or(std::string(kDefaultHotreloadDir));
  // Verify.
#ifdef YRTR_DEBUG
  if (hotreload_dir_ != "" && !fs::exists(hotreload_dir_)) {
    // Hotreload directory is optional, only provides warning.
    HLOG_F(WARNING, "Failed to find hotreload_dir={}", hotreload_dir_);
  }
#endif
  auto_record_ = CheckLoad<bool>(global, "auto_record");
}

void Config::LoadTechList(const toml::table& tech_tb) {
  for (auto [k, v] : tech_tb) {
    std::string_view key = k.str();
    Tech tech = GetTech(key);
    CHECK(tech != Tech::kUnknown && tech != Tech::kCount);
    auto raw_val = v.as<bool>();
    CHECK_NOTNULL(raw_val);
    bool enable = raw_val->get();
    tech_list_[static_cast<int>(tech)] = enable;
  }
}

}  // namespace backend
}  // namespace yrtr
