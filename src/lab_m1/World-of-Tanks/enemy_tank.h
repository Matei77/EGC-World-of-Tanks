#pragma once

#include "tank.h"

namespace world_of_tanks {
    class EnemyTank : public Tank {
    private:
        Timer move_timer_ = Timer(ENEMY_INITIAL_MOVE_TIMER);
        MoveList current_move_ = STAND;
        
    public:
        EnemyTank(const glm::vec3 position, const float body_rotation) : Tank(position) {
            body_rotation_ = body_rotation;
            reload_timer_ = Timer(ENEMY_RELOAD_TIME);
        }
        
        void MoveTank(float delta_time);
        void ShootAtPlayer(float delta_time, const Tank &player_tank, std::vector<Projectile> &projectiles);
    };
}

