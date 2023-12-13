#pragma once
#include <vector>

#include "enemy_tank.h"
#include "map.h"
#include "projectile.h"
#include "tank.h"
#include "tank_spawner.h"

namespace world_of_tanks {
    class LogicEngine {
    private:
        Tank player_tank_;
        std::vector<EnemyTank> enemy_tanks_;
        std::vector<Projectile> projectiles_;
        Map map_;
        TankSpawner tank_spawner_;

    public:
        void Init();
        void Update(float delta_time);
        void DespawnObjects();
        Tank & GetPlayerTank() { return player_tank_; }
        // void SetPlayerTank(const Tank &player_tank) { player_tank_ = player_tank; }
        const std::vector<EnemyTank> & GetEnemyTanks() const { return enemy_tanks_; }
        // void SetEnemyTanks(const std::vector<Tank> &enemy_tanks) { enemy_tanks_ = enemy_tanks; }
        std::vector<Projectile> & GetProjectiles() { return projectiles_; }
        // void SetProjectiles(const std::vector<Projectile> &projectiles) { projectiles_ = projectiles; }
        const Map & GetMap() const { return map_; }
    };
}
