#pragma once

namespace world_of_tanks {
    class Timer {
    private:
        float time_elapsed_ = 0;
        float period_;
        bool finished_ = false;
    
    public:
        Timer(const float period) : period_(period) {}
        
        void UpdateTimer(float delta_time);
        void ResetTimer();
    
        bool IsFinished() const { return finished_; }
    };
}
