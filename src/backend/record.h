#pragma once
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

#include "protocol/model.h"

namespace yrtr {
namespace backend {

class Record {
 public:
  explicit Record(const fs::path& filepath);
  ~Record();

  bool WriteCheckboxStates(const CheckboxStateMap& state_map);
  bool ReadCheckboxStates(CheckboxStateMap& state_map);

  bool WriteSliderStates(const SliderStateMap& state_map);
  bool ReadSliderStates(SliderStateMap& state_map);

 private:
  fs::path filepath_;
  std::fstream file_;

  CheckboxStateMap ckbox_states_;
  SliderStateMap slider_states_;

  bool ReadAll();
  bool WriteAll();
  void Overwrite(const CheckboxStateMap& state_map);
  void Overwrite(const SliderStateMap& state_map);
};

}  // namespace backend
}  // namespace yrtr