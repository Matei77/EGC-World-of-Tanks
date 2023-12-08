#pragma once
#include <vector>

#include "tank.h"

namespace world_of_tanks {
    class LogicEngine {
    private:
        Tank player_tank_;
        std::vector<Tank> enemy_tanks_;

    public:
        Tank GetPlayerTank() const { return player_tank_; }
        void SetPlayerTank(const Tank &player_tank) { player_tank_ = player_tank; }
        std::vector<Tank> GetEnemyTanks() const { return enemy_tanks_; }
        void SetEnemyTanks(const std::vector<Tank> &enemy_tanks) { enemy_tanks_ = enemy_tanks; }
    };
}
