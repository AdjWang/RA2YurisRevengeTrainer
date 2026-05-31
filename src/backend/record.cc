#include "backend/record.h"

#include <fstream>

#include "base/logging.h"
#include "formatter/std.h"
#include "gsl/util"
#include "toml++/toml.hpp"

namespace yrtr {
namespace backend {

Record::Record(const fs::path& filepath) : filepath_(filepath) {
  if (!fs::exists(filepath_)) {
    std::ofstream create(filepath_, std::ios::app);
  }
  file_.open(filepath_, std::ios::in | std::ios::out);
}

Record::~Record() {
  if (file_.is_open()) {
    file_.close();
  }
}

bool Record::WriteCheckboxStates(const CheckboxStateMap& state_map) {
  if (!file_.is_open()) {
    return false;
  }
  if (!ReadAll()) {
    return false;
  }
  Overwrite(state_map);
  return WriteAll();
}

bool Record::ReadCheckboxStates(CheckboxStateMap& state_map) {
  if (!ReadAll()) {
    return false;
  }
  state_map = ckbox_states_;
  return true;
}

bool Record::WriteSliderStates(const SliderStateMap& state_map) {
  if (!file_.is_open()) {
    return false;
  }
  if (!ReadAll()) {
    return false;
  }
  Overwrite(state_map);
  return WriteAll();
}

bool Record::ReadSliderStates(SliderStateMap& state_map) {
  if (!ReadAll()) {
    return false;
  }
  state_map = slider_states_;
  return true;
}

bool Record::ReadAll() {
  if (!file_.is_open()) {
    return false;
  }
  try {
    auto root_tbl = toml::parse_file(filepath_.string());
    DLOG_F(INFO, "Load record from file={}", filepath_);
    ckbox_states_.clear();
    slider_states_.clear();
    if (auto* checkbox_tbl = root_tbl["checkbox"].as_table()) {
      for (const auto& [key, value] : *checkbox_tbl) {
        FnLabel label = StrToFnLabel(key.str());
        if (label == FnLabel::kInvalid) {
          continue;
        }
        if (auto activate = value.value<bool>()) {
          ckbox_states_[label].activate = *activate;
        }
      }
    }
    if (auto* slider_tbl = root_tbl["slider"].as_table()) {
      for (const auto& [key, value] : *slider_tbl) {
        FnLabel label = StrToFnLabel(key.str());
        if (label == FnLabel::kInvalid) {
          continue;
        }
        if (auto val = value.value<uint32_t>()) {
          slider_states_[label].val = *val;
        }
      }
    }
    return true;
  } catch (const toml::parse_error& err) {
    LOG_F(WARNING, "Failed to parse record file={} err={}", filepath_,
          err.what());
    return false;
  }
}

bool Record::WriteAll() {
  if (!file_.is_open()) {
    return false;
  }
  toml::table root_tbl;
  toml::table checkbox_tbl;
  for (const auto& [label, state] : ckbox_states_) {
    checkbox_tbl.insert(StrFnLabel(label), state.activate);
  }
  root_tbl.insert_or_assign("checkbox", std::move(checkbox_tbl));
  toml::table slider_tbl;
  for (const auto& [label, state] : slider_states_) {
    slider_tbl.insert(StrFnLabel(label), state.val);
  }
  root_tbl.insert_or_assign("slider", std::move(slider_tbl));
  file_.clear();
  file_.seekp(0, std::ios::beg);
  DLOG_F(INFO, "Write record to file={}", filepath_);
  file_ << root_tbl;
  if (!file_.good()) {
    return false;
  }
  file_.flush();
  auto write_pos = file_.tellp();
  if (write_pos == std::streampos(-1)) {
    return false;
  }
  std::error_code ec;
  fs::resize_file(filepath_, static_cast<uintmax_t>(write_pos), ec);
  return !ec;
}

void Record::Overwrite(const CheckboxStateMap& state_map) {
  for (const auto& [label, state] : state_map) {
    ckbox_states_[label] = state;
  }
}

void Record::Overwrite(const SliderStateMap& state_map) {
  for (const auto& [label, state] : state_map) {
    slider_states_[label] = state;
  }
}

}  // namespace backend
}  // namespace yrtr
