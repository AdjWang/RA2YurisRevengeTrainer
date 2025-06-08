#include "backend/record.h"

#include <fstream>

#include "base/logging.h"
#include "formatter/std.h"
#include "toml++/toml.hpp"

namespace yrtr {
namespace backend {

bool WriteCheckboxStateToToml(const CheckboxStateMap& state_map,
                              const fs::path& filepath) {
  toml::table tbl;
  for (const auto& [label, state] : state_map) {
    tbl.insert(StrFnLabel(label), state.activate);
  }
  std::ofstream file(filepath);
  if (!file.is_open()) {
    return false;
  }
  DLOG_F(INFO, "Write record to file={}", filepath);
  file << tbl;
  return file.good();
}

bool ReadCheckboxStateFromToml(const fs::path& filepath,
                               CheckboxStateMap& state_map) {
  try {
    auto tbl = toml::parse_file(filepath.string());
    DLOG_F(INFO, "Load record from file={}", filepath);
    for (const auto& [key, value] : tbl) {
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
