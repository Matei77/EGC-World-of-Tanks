#include "logic_engine.h"

#include <iostream>

#include "constants.h"

using namespace world_of_tanks;

void LogicEngine::Init() {
    // set seed
    srand(time(NULL));

    // map_.InitMap();
}

void LogicEngine::Update(float delta_time, Camera *camera) {
    // update game timer
    game_timer_.UpdateTimer(delta_time);
    if (game_timer_.IsFinished()) {
        std::cout << "Game Over!\n";
        std::cout << "Tanks destroyed: " << tanks_destoryed_ << "\n";
    }
    
    // check player tank - enemy tank collision
    CheckForPlayerTankCollision(camera);
    
    // update player tank reload time
    player_tank_.GetReloadTimer().UpdateTimer(delta_time);

    // update projectiles
    for (auto &projectile : projectiles_) {
        projectile.UpdateProjectile(delta_time);
    }

    // update tank spawner
    tank_spawner_.UpdateTankSpawner(delta_time, enemy_tanks_);

    // check for tank-projectile collisions
    player_tank_.CheckIfHit(projectiles_);
    for (auto &enemy_tank : enemy_tanks_)
        enemy_tank.CheckIfHit(projectiles_);
    
    // move tanks
    for (auto &enemy_tank : enemy_tanks_)
        enemy_tank.MoveTank(delta_time);

    // check enemy tank collisions
    CheckEnemyTanksCollisions();

    // check for destroyed tanks
    for (auto &enemy_tank : enemy_tanks_)
        if (enemy_tank.GetHealth() <= 0)
            ++tanks_destoryed_;

    DespawnObjects();
}

void LogicEngine::DespawnObjects() {
    // despawn projectiles
    projectiles_.erase(std::remove_if(projectiles_.begin(),
                                      projectiles_.end(),
                                      [](const Projectile &projectile) {
                                          return projectile.GetTimer().IsFinished() || projectile.IsHit() == true;
                                      }),
                       projectiles_.end());

    // despawn tanks
    enemy_tanks_.erase(std::remove_if(enemy_tanks_.begin(),
                                      enemy_tanks_.end(),
                                      [](const EnemyTank &enemy_tank) {
                                          return enemy_tank.GetHealth() <= 0;
                                      }),
                       enemy_tanks_.end());
}


void LogicEngine::CheckForPlayerTankCollision(Camera *camera) {
    // other tanks collisions
    for (auto &enemy_tank : enemy_tanks_) {
        const float distance = glm::distance(player_tank_.GetPosition(), enemy_tank.GetPosition());
        if (distance < 2 * TANK_RADIUS && distance != 0.0f) {
            glm::vec3 dif = enemy_tank.GetPosition() - player_tank_.GetPosition();
            float P = TANK_RADIUS * 2 - distance;
            glm::vec3 P2 = P * glm::normalize(dif);
            player_tank_.SetPosition(player_tank_.GetPosition() + P2 * -0.5f);
            camera->SetPosition(camera->GetPosition() + P2 * -0.5f);
            // enemy_tank.SetPosition(enemy_tank.GetPosition() + P2 * 0.5f);
        }
    }

    // end of map collisions
    if (player_tank_.GetPosition().x + TANK_RADIUS > MAP_SIZE / 2.0f) {
        const float distance_x = MAP_SIZE / 2.0f - player_tank_.GetPosition().x;
        float p_x = TANK_RADIUS - distance_x;
        glm::vec3 p2_x = glm::vec3(p_x, 0, 0) * 0.5f;
        player_tank_.SetPosition(player_tank_.GetPosition() - p2_x);
        camera->SetPosition(camera->GetPosition() - p2_x);
    
    } else if (player_tank_.GetPosition().x - TANK_RADIUS < -MAP_SIZE / 2.0f) {
        const float distance_x = MAP_SIZE / 2.0f + player_tank_.GetPosition().x;
        float p_x = TANK_RADIUS - distance_x;
        glm::vec3 p2_x = glm::vec3(p_x, 0, 0) * 0.5f;
        player_tank_.SetPosition(player_tank_.GetPosition() + p2_x);
        camera->SetPosition(camera->GetPosition() + p2_x);
    }

    if (player_tank_.GetPosition().z + TANK_RADIUS > MAP_SIZE / 2.0f) {
        const float distance_z = MAP_SIZE / 2.0f - player_tank_.GetPosition().z;
        float p_z = TANK_RADIUS - distance_z; 
        glm::vec3 p2_z = glm::vec3(0, 0, p_z) * 0.5f;
        player_tank_.SetPosition(player_tank_.GetPosition() - p2_z);
        camera->SetPosition(camera->GetPosition() - p2_z);
        
    } else if (player_tank_.GetPosition().z - TANK_RADIUS < -MAP_SIZE / 2.0f) {
        const float distance_z = MAP_SIZE / 2.0f + player_tank_.GetPosition().z;
        float p_z = TANK_RADIUS - distance_z; 
        glm::vec3 p2_z = glm::vec3(0, 0, p_z) * 0.5f;
        player_tank_.SetPosition(player_tank_.GetPosition() + p2_z);
        camera->SetPosition(camera->GetPosition() + p2_z);
    }
    
}

void LogicEngine::CheckEnemyTanksCollisions() {
    if (enemy_tanks_.size() < 2)
        return;
    
    for (auto i = enemy_tanks_.begin(); i != std::prev(enemy_tanks_.end()); ++i) {
        // check tank - tank collisions
        for (auto j = std::next(i); j != enemy_tanks_.end(); ++j) {
            
            const float distance = glm::distance(i->GetPosition(), j->GetPosition());
            if (distance < 2 * TANK_RADIUS && distance != 0.0f) {
                glm::vec3 dif = j->GetPosition() - i->GetPosition();
                float P = TANK_RADIUS * 2 - distance;
                glm::vec3 P2 = P * glm::normalize(dif);
                i->SetPosition(i->GetPosition() + P2 * -0.5f);
            }
        }

        // check out of map
        if (i->GetPosition().x + TANK_RADIUS > MAP_SIZE / 2.0f) {
            const float distance_x = MAP_SIZE / 2.0f - i->GetPosition().x;
            float p_x = TANK_RADIUS - distance_x;
            glm::vec3 p2_x = glm::vec3(p_x, 0, 0) * 0.5f;
            i->SetPosition(i->GetPosition() - p2_x);
    
        } else if (i->GetPosition().x - TANK_RADIUS < -MAP_SIZE / 2.0f) {
            const float distance_x = MAP_SIZE / 2.0f + i->GetPosition().x;
            float p_x = TANK_RADIUS - distance_x;
            glm::vec3 p2_x = glm::vec3(p_x, 0, 0) * 0.5f;
            i->SetPosition(i->GetPosition() + p2_x);
        }

        if (i->GetPosition().z + TANK_RADIUS > MAP_SIZE / 2.0f) {
            const float distance_z = MAP_SIZE / 2.0f - i->GetPosition().z;
            float p_z = TANK_RADIUS - distance_z; 
            glm::vec3 p2_z = glm::vec3(0, 0, p_z) * 0.5f;
            i->SetPosition(i->GetPosition() - p2_z);
        
        } else if (i->GetPosition().z - TANK_RADIUS < -MAP_SIZE / 2.0f) {
            const float distance_z = MAP_SIZE / 2.0f + i->GetPosition().z;
            float p_z = TANK_RADIUS - distance_z; 
            glm::vec3 p2_z = glm::vec3(0, 0, p_z) * 0.5f;
            i->SetPosition(i->GetPosition() + p2_z);
        }
    }
}