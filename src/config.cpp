#include "config.h"
#include "logging.h"
#include "win32utils.h"

namespace yrtr {
namespace config {

static GlobalConfig global_config;

namespace {
static std::filesystem::path GetDefaultFontPath(Lang font_lang) {
    std::filesystem::path font_dir = win32::GetDefaultFontDir();
    if (font_lang == Lang::kZh) {
        return font_dir / kDefaultFontFileZh;
    } else if (font_lang == Lang::kEn) {
        return font_dir / kDefaultFontFileEn;
    } else {
        UNREACHABLE();
    }
}

static std::string ParseExecName(const toml::v3::ex::parse_result &yrtr_config,
                                 const std::string& default_exec_name) {
    try {
        return yrtr_config["global"]["executable_name"].value_or("gamemd.exe");
    } catch (std::exception &e) {
        LOG(WARN, "Failed to parse executable_name config. Exception={}",
            e.what());
    }
    return default_exec_name;
}

static Lang ParseLang(const toml::v3::ex::parse_result &yrtr_config,
                      Lang default_lang) {
    try {
        std::string lang_config =
            yrtr_config["global"]["language"].value_or("zh");
        if (lang_config == "zh") {
            return Lang::kZh;
        } else if (lang_config == "en") {
            return Lang::kEn;
        } else {
            LOG(WARN, "Invalid setting language={}. Available: zh, en.",
                lang_config);
        }
    } catch (std::exception &e) {
        LOG(WARN, "Failed to parse language config. Exception={}", e.what());
    }
    return default_lang;
}

static std::filesystem::path
ParseFontPath(const toml::v3::ex::parse_result &yrtr_config,
              const std::filesystem::path &default_path) {
    try {
        std::string font_path_config =
            yrtr_config["global"]["font_path"].value_or("");
        if (font_path_config != "") {
            if (std::filesystem::exists(font_path_config)) {
                return font_path_config;
            } else {
                LOG(WARN, "Specified font path={} not exist", font_path_config);
            }
        }
    } catch (std::exception &e) {
        LOG(WARN, "Failed to parse font_path config. Exception={}", e.what());
    }
    return default_path;
}
} // namespace

void InitConfig() {
    // Default configs
    global_config.exec_name = "gamemd.exe";
    global_config.lang = Lang::kZh;
    global_config.font_path = GetDefaultFontPath(global_config.lang);
    // Try parsing configs
    if (std::filesystem::exists(kConfigFile)) {
        toml::v3::ex::parse_result yrtr_config;
        try {
            yrtr_config = toml::parse_file(kConfigFile);
            global_config.exec_name =
                ParseExecName(yrtr_config, global_config.exec_name);
            global_config.lang = ParseLang(yrtr_config, global_config.lang);
            global_config.font_path =
                ParseFontPath(yrtr_config, global_config.font_path);
            CHECK(global_config.font_path != "");
        } catch (std::exception &e) {
            LOG(WARN,
                "Failed to parse config file={} exception={}. Use default "
                "configurations.",
                kConfigFile, e.what());
        }
    } else {
        LOG(INFO, "Not found config file={}. Use default configurations.",
            kConfigFile);
    }
    // Hints
    LOG(INFO, "GlobalConfig.ExecutableName={}", global_config.exec_name);
    LOG(INFO, "GlobalConfig.Language={}", StrLang(global_config.lang));
    LOG(INFO, "GlobalConfig.Font={}", global_config.font_path.string());
}

const GlobalConfig& GetGlobalConfig() {
    return global_config;
}

} // namespace config
} // namespace yrtr
