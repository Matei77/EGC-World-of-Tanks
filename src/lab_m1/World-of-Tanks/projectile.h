#pragma once
#include <glm/vec3.hpp>

namespace world_of_tanks {
    class Projectile {
    private:
        glm::vec3 position_;
        float fired_rotation_y_;
        float time_ = 0;
    
    public:
        Projectile(glm::vec3 position, float fired_rotation_y) : position_(position), fired_rotation_y_(fired_rotation_y) {}

        void UpdateProjectile(float delta_time);
        
        glm::vec3 GetPosition() const { return position_; }
        void SetPosition(const glm::vec3 position) { position_ = position; }
        float GetRotationY() const { return fired_rotation_y_; }
        void SetRotationY(const float rotation_y) { fired_rotation_y_ = rotation_y; }
        float GetTime() const { return time_; }
        void SetTime(const float time) { time_ = time; }
    };
}
