#pragma once
#include "tank.h"

namespace world_of_tanks {
    class EnemyTank : public Tank {
    public:
        EnemyTank(const glm::vec3 position, const float body_rotation) : Tank(position) {
            body_rotation_ = body_rotation;
        }

        void MoveTank();
    };
}

