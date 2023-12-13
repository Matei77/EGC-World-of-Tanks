#include "logic_engine.h"

#include "constants.h"

using namespace world_of_tanks;

void LogicEngine::Init() {
    // set seed
    srand(time(NULL));

    // map_.InitMap();
}

void LogicEngine::Update(float delta_time) {
    // update player tank reload time
    player_tank_.GetReloadTimer().UpdateTimer(delta_time);

    // update projectiles
    for (auto &projectile : projectiles_) {
        projectile.UpdateProjectile(delta_time);
    }

    tank_spawner_.UpdateTankSpawner(delta_time, enemy_tanks_);

    DespawnObjects();
}

void LogicEngine::DespawnObjects() {
    // despawn projectiles
    projectiles_.erase(std::remove_if(projectiles_.begin(),
                                      projectiles_.end(),
                                      [](const Projectile &projectile) {
                                          return projectile.GetTimer().IsFinished();
                                      }),
                       projectiles_.end());
}
