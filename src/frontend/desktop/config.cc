#include "frontend/desktop/config.h"

#include <optional>

#include "base/logging.h"
#include "formatter/std.h"

namespace yrtr {
namespace frontend {

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
  return raw_val->get();
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
  return true;
}

// Default configurations.
Config::Config(const fs::path& cfg_path)
    : cfg_dir_(cfg_path.parent_path()),
      cfg_path_(cfg_path),
      lang_(kDefaultLang),
      enable_dpi_awareness_(false),
      font_path_(kDefaultFontPath),
      fontex_path_(kDefaultFontExPath) {}

Lang Config::lang() const {
  if (lang_ == "zh") {
    return Lang::kZh;
  } else if (lang_ == "en") {
    return Lang::kEn;
  } else {
    UNREACHABLE();
  }
}

void Config::DisableHotkeyGUI(int key) {
  disabled_hot_key_.emplace(key);
}

std::string Config::GetFnStrWithKey(FnLabel label) {
  int hot_key = Config::GetHotkey(label);
  if (disabled_hot_key_.contains(hot_key) || hot_key == GLFW_KEY_UNKNOWN) {
    return std::format("{}()",
                       reinterpret_cast<const char*>(GetFnStr(label, lang())));
  }
  const char8_t* fn_str = GetFnStr(label, lang());
  const char8_t* key_str = Config::KeyToString(hot_key);
  return std::format("{}({})", reinterpret_cast<const char*>(fn_str),
                     reinterpret_cast<const char*>(key_str));
}

fs::path Config::GetAbsolutePath(const fs::path& relpath) const {
  if (relpath.is_absolute()) {
    return relpath;
  } else {
    return cfg_dir_ / relpath;
  }
}

void Config::LoadGlobal(const toml::table& global) {
  lang_ = CheckLoad<std::string>(global, "language");
  enable_dpi_awareness_ =
      TryLoad<bool>(global, "enable_dpi_awareness").value_or(false);
  font_path_ = TryLoad<std::string>(global, "font_path")
                   .value_or(std::string(kDefaultFontPath));
  fontex_path_ = CheckLoad<std::string>(global, "fontex_path");
  // Verify.
  CHECK_F(std::find(kAvailableLang.begin(), kAvailableLang.end(), lang_) !=
              kAvailableLang.end(),
          "Unable to find lang={} in available={}", lang_, kAvailableLang);
  if (font_path_ != "") {
    CHECK_F(fs::exists(font_path_), "Failed to find font_path={}", font_path_);
  }
  CHECK_F(fs::exists(fontex_path_), "Failed to find fontex_path={}",
          fontex_path_);
  CHECK_F(!(font_path_ == "" && fontex_path_ == ""), "Not providing font");
}

}  // namespace frontend
}  // namespace yrtr
