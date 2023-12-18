#pragma once
#include <glm/vec3.hpp>

namespace world_of_tanks {
    class Building {
    private:
        glm::vec3 position_;
        float half_height_;
        float half_width_;
        float half_length_;

    public:
        Building(const glm::vec3 position, const float height, const float width, const float length) :
            position_(position), half_height_(height), half_width_(width),
            half_length_(length) {}

        glm::vec3 GetPosition() const { return position_; }
        float GetHalfHeight() const { return half_height_; }
        float GetHalfWidth() const { return half_width_; }
        float GetHalfLength() const { return half_length_; }
    };
}
