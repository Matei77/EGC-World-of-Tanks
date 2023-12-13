#include "timer.h"

using namespace world_of_tanks;

void Timer::UpdateTimer(const float delta_time) {
    if (time_elapsed_ < period_) {
        time_elapsed_ += delta_time;
    } else {
        finished_ = true;
    }
}

void Timer::ResetTimer() {
    time_elapsed_ = 0;
    finished_ = false;
}
