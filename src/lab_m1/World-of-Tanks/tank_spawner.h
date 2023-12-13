#pragma once
#include "enemy_tank.h"

namespace world_of_tanks {
    class TankSpawner {
    private:
        Timer timer_ = Timer(TANK_SPAWN_TIME);

    public:
        TankSpawner() = default;

        void UpdateTankSpawner(float delta_time, std::vector<EnemyTank> &enemy_tanks);
    };

}

