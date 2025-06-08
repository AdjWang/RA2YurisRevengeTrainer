#pragma once
#include <filesystem>
namespace fs = std::filesystem;

#include "protocol/model.h"

namespace yrtr {
namespace backend {

bool WriteCheckboxStateToToml(const CheckboxStateMap& state_map,
                              const fs::path& filepath);
bool ReadCheckboxStateFromToml(const fs::path& filepath,
                               CheckboxStateMap& state_map);

}  // namespace backend
}  // namespace yrtr