#include "backend/record.h"

#include <fstream>

#include "base/logging.h"
#include "formatter/std.h"
#include "gsl/util"
#include "toml++/toml.hpp"

namespace yrtr {
namespace backend {

bool WriteCheckboxStateToToml(const CheckboxStateMap& state_map,
                             const fs::path& filepath) {
  toml::table root_tbl;
  toml::table checkbox_tbl;
  for (const auto& [label, state] : state_map) {
    checkbox_tbl.insert(StrFnLabel(label), state.activate);
  }
  root_tbl.insert("checkbox", std::move(checkbox_tbl));
  std::ofstream file(filepath);
  if (!file.is_open()) {
    return false;
  }
  auto _ = gsl::finally([&]() { file.close(); });
  DLOG_F(INFO, "Write record to file={}", filepath);
  file << root_tbl;
  return file.good();
}

bool ReadCheckboxStateFromToml(const fs::path& filepath,
                              CheckboxStateMap& state_map) {
  try {
    auto root_tbl = toml::parse_file(filepath.string());
    DLOG_F(INFO, "Load record from file={}", filepath);
    auto* checkbox_tbl = root_tbl["checkbox"].as_table();
    if (!checkbox_tbl) {
      LOG_F(WARNING, "Missing [checkbox] section in file={}", filepath);
      return false;
    }
    for (const auto& [key, value] : *checkbox_tbl) {
      FnLabel label = StrToFnLabel(key.str());
      if (label == FnLabel::kInvalid) {
        // Skip unknown keys
        continue;
      }
      if (auto activate = value.value<bool>()) {
        state_map[label].activate = *activate;
      }
    }
    return true;
  } catch (const toml::parse_error& err) {
    LOG_F(WARNING, "Failed to parse record file={} err={}", filepath,
          err.what());
    return false;
  }
}

}  // namespace backend
}  // namespace yrtr
