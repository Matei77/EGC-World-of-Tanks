#pragma once

namespace world_of_tanks {
    namespace utils {
        struct Viewport {
            int x;
            int y;
            int width;
            int height;
            
            Viewport() : x(0), y(0), width(1), height(1) {}
            Viewport(const int x, const int y, const int width, const int height)
                : x(x), y(y), width(width), height(height) {}
        };

        
        inline float liner_conversion(float value, float min, float max, float new_min, float new_max) {
            float new_value = 0;

            if (value > max) {
                value = max;
            } else if (value < min) {
                value = min;
            }

            const float range = (max - min);
            if (range == 0.0f) {
                new_value = new_min;
            } else {
                const float new_range = (new_max - new_min);
                new_value = (((value - min) * new_range) / range) + new_min;
            }

            return new_value;
        }
    }
}