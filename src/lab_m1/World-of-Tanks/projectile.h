#pragma once
#include <glm/vec3.hpp>

#include "constants.h"
#include "timer.h"

namespace world_of_tanks {
    class Projectile {
    private:
        glm::vec3 position_;
        float fired_rotation_y_;
        Timer timer_ = Timer(PROJECTILE_TIME_LIMIT);
        bool hit_ = false;
        float speed_;

    public:
        Projectile(const glm::vec3 position, const float fired_rotation_y, const float speed) : position_(position),
            fired_rotation_y_(fired_rotation_y), speed_(speed) {}

        void UpdateProjectile(float delta_time);

        glm::vec3 GetPosition() const { return position_; }
        void SetPosition(const glm::vec3 position) { position_ = position; }
        float GetRotationY() const { return fired_rotation_y_; }
        void SetRotationY(const float rotation_y) { fired_rotation_y_ = rotation_y; }
        Timer GetTimer() const { return timer_; }
        bool IsHit() const { return hit_; }
        void SetHit(const bool hit) { hit_ = hit; }
    };
}
