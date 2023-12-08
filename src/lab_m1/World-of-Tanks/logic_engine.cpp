#include "logic_engine.h"

#include "constants.h"

using namespace world_of_tanks;

void LogicEngine::Update(float delta_time) {
    // update player tank reload time
    if (player_tank_.GetReloadTimer() > 0.0f) {
        player_tank_.SetReloadTimer(player_tank_.GetReloadTimer() - delta_time);
    }

    // update projectiles
    for (auto &projectile : projectiles_) {
        projectile.UpdateProjectile(delta_time);
    }

    DespawnObjects();
}

void LogicEngine::DespawnObjects() {
    // despawn projectiles
    projectiles_.erase(std::remove_if(projectiles_.begin(),
                                      projectiles_.end(),
                                      [](const Projectile &projectile) {
                                          return projectile.GetTime() >= PROJECTILE_TIME_LIMIT;
                                      }),
                       projectiles_.end());
}
