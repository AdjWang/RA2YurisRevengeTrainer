#include "frontend/desktop/timer.h"

#include "frontend/desktop/glfw.h"

namespace yrtr {
namespace frontend {

std::unordered_map<int, Timer::TimerData> Timer::timer_data_;

void Timer::SetTimer(int id, double interval, Handler handler) {
  timer_data_.erase(id);
  timer_data_.emplace(id, TimerData{
                              .interval = interval,
                              .time_stamp = glfwGetTime(),
                              .handler = std::move(handler),
                          });
}

void Timer::Update() {
  double ts = glfwGetTime();
  for (auto& [id, data] : timer_data_) {
    if (ts - data.time_stamp > data.interval) {
      data.handler();
      data.time_stamp = ts;
    }
  }
}

}  // namespace frontend
}  // namespace yrtr
