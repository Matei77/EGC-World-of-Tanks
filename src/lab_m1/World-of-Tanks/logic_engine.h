#pragma once
#include <vector>

#include "camera.h"
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
        Timer game_timer_ = Timer(GAME_TIMER);
        int tanks_destroyed_ = 0;
        bool game_over_ = false;

        void DespawnObjects();
        void CheckForPlayerTankCollision(Camera *camera);
        void CheckEnemyTanksCollisions();
        void CheckProjectileBuildingCollisions();

    public:
        void Init();
        void Update(float delta_time, Camera *camera);

        Tank & GetPlayerTank() { return player_tank_; }
        const std::vector<EnemyTank> & GetEnemyTanks() const { return enemy_tanks_; }
        std::vector<Projectile> & GetProjectiles() { return projectiles_; }
        const Map & GetMap() const { return map_; }
        const Timer & GetGameTimer() const { return game_timer_; }
        bool IsGameOver() const { return game_over_; }
    };
}
