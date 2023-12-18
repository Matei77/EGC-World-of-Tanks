#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "constants.h"
#include "projectile.h"
#include "timer.h"

namespace world_of_tanks {
    class Tank {
    protected:
        glm::vec3 position_ = glm::vec3(0);
        float body_rotation_ = 0.0f;
        float turret_rotation_ = 0.0f;
        float cannon_rotation_ = 0.0f;
        Timer reload_timer_ = Timer(TANK_RELOAD_TIMER);
        int health_ = TANK_HEALTH;
        
    public:
        Tank () = default;

        explicit Tank(const glm::vec3 position) : position_(position) {}

        void FireProjectile(std::vector<Projectile> &projectiles, float speed) const;
        void CheckIfHit(std::vector<Projectile> &projectiles);
        inline bool CheckTurretHit(glm::vec3 projectile_pos) const;

        glm::vec3 GetPosition() const { return position_; }
        void SetPosition(const glm::vec3 position) { position_ = position; }
        float GetTurretRotation() const { return turret_rotation_; }
        void SetTurretRotation(const float turret_rotation) { turret_rotation_ = turret_rotation; }
        float GetCannonRotation() const { return cannon_rotation_; }
        void SetCannonRotation(const float cannon_rotation) { cannon_rotation_ = cannon_rotation; }
        float GetBodyRotation() const { return body_rotation_; }
        void SetBodyRotation(const float body_rotation) { body_rotation_ = body_rotation; }
        Timer & GetReloadTimer() { return reload_timer_; }
        void SetReloadTimer(const float reload_timer) { this->reload_timer_ = reload_timer; }
        int GetHealth() const { return health_; }
    };
}
