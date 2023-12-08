#pragma once
#include <glm/vec3.hpp>

#include "utils/math_utils.h"

namespace world_of_tanks {
    class Tank {
    private:
        glm::vec3 position_ = glm::vec3(0);
        float body_rotation_ = 0;
        float turret_rotation_ = 0;
        float cannon_rotation_ = 0;
        
        
    public:
        Tank () = default;
        
        Tank(glm::vec3 position) : position_(position) {}

        glm::vec3 GetPosition() const { return position_; }
        void SetPosition(const glm::vec3 position) { position_ = position; }
        float GetTurretRotation() const { return turret_rotation_; }
        void SetTurretRotation(const float turret_rotation) { turret_rotation_ = turret_rotation; }
        float GetCannonRotation() const { return cannon_rotation_; }
        void SetCannonRotation(const float cannon_rotation) { cannon_rotation_ = cannon_rotation; }
        float GetBodyRotation() const { return body_rotation_; }
        void SetBodyRotation(const float body_rotation) { body_rotation_ = body_rotation; }
    };
}
