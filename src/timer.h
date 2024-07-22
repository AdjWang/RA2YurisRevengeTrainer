#pragma once
#include "vendor.h"

namespace yrtr {

class GuiContext;

class Timer{
public:
    using Handler = std::function<void()>;
    static void SetTimer(int id, double interval, Handler handler);
    static void Update();

private:
    struct TimerData {
        double interval;
        double time_stamp;
        Handler handler;
    };
    static std::unordered_map<int, TimerData> timer_data_;
};

}  // namespace yrtr
