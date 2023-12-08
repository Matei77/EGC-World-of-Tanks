#pragma once
#include <vector>

#include "projectile.h"
#include "tank.h"

namespace world_of_tanks {
    class LogicEngine {
    private:
        Tank player_tank_;
        std::vector<Tank> enemy_tanks_;
        std::vector<Projectile> projectiles_;

    public:
        void Update(float delta_time);
        void DespawnObjects();
        Tank & GetPlayerTank() { return player_tank_; }
        // void SetPlayerTank(const Tank &player_tank) { player_tank_ = player_tank; }
        const std::vector<Tank> & GetEnemyTanks() const { return enemy_tanks_; }
        // void SetEnemyTanks(const std::vector<Tank> &enemy_tanks) { enemy_tanks_ = enemy_tanks; }
        std::vector<Projectile> & GetProjectiles() { return projectiles_; }
        // void SetProjectiles(const std::vector<Projectile> &projectiles) { projectiles_ = projectiles; }
    };
}
