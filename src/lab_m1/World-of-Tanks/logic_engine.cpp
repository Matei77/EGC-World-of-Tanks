#include "logic_engine.h"

#include <iostream>

#include "constants.h"

using namespace world_of_tanks;

void LogicEngine::Init() {
    // set seed
    srand(time(NULL));

    map_.InitMap();
}

void LogicEngine::Update(float delta_time, Camera *camera) {
    // update game timer
    game_timer_.UpdateTimer(delta_time);
    if (game_timer_.IsFinished() || player_tank_.GetHealth() <= 0) {
        std::cout << "Game Over!\n";
        std::cout << "Tanks destroyed: " << tanks_destroyed_ << "\n";
        game_over_ = true;
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
    for (auto &enemy_tank : enemy_tanks_) {
        enemy_tank.MoveTank(delta_time);
        enemy_tank.ShootAtPlayer(delta_time, player_tank_, projectiles_);
    }

    // check enemy tank collisions
    CheckEnemyTanksCollisions();

    // check for destroyed tanks
    for (auto &enemy_tank : enemy_tanks_)
        if (enemy_tank.GetHealth() <= 0)
            ++tanks_destroyed_;

    // check projectile-building collision
    CheckProjectileBuildingCollisions();

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
        }
    }

    // end of map collisions
    if (player_tank_.GetPosition().x + TANK_RADIUS > MAP_SIZE / 2.0f) {
        const float distance_x = MAP_SIZE / 2.0f - player_tank_.GetPosition().x;
        float p_x = TANK_RADIUS - distance_x;
        glm::vec3 p2_x = glm::vec3(p_x, 0, 0) * 0.5f;
        player_tank_.SetPosition(player_tank_.GetPosition() - p2_x);
        camera->SetPosition(camera->GetPosition() - p2_x);
    }
    else if (player_tank_.GetPosition().x - TANK_RADIUS < -MAP_SIZE / 2.0f) {
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
    }
    else if (player_tank_.GetPosition().z - TANK_RADIUS < -MAP_SIZE / 2.0f) {
        const float distance_z = MAP_SIZE / 2.0f + player_tank_.GetPosition().z;
        float p_z = TANK_RADIUS - distance_z;
        glm::vec3 p2_z = glm::vec3(0, 0, p_z) * 0.5f;
        player_tank_.SetPosition(player_tank_.GetPosition() + p2_z);
        camera->SetPosition(camera->GetPosition() + p2_z);
    }

    // check buildings collisions
    for (auto building : map_.GetBuildings()) {
        // find closest points
        float closest_x = std::max(building.GetPosition().x - building.GetHalfWidth(),
                                   std::min(player_tank_.GetPosition().x,
                                            building.GetPosition().x + building.GetHalfWidth()));
        float closest_z = std::max(building.GetPosition().z - building.GetHalfLength(),
                                   std::min(player_tank_.GetPosition().z,
                                            building.GetPosition().z + building.GetHalfLength()));

        // find distance
        float direction_x = player_tank_.GetPosition().x - closest_x;
        float direction_z = player_tank_.GetPosition().z - closest_z;
        float distance = std::sqrt(direction_x * direction_x + direction_z * direction_z);

        // check for collision
        if (distance < TANK_RADIUS) {
            std::cout << "player building collision\n";
            std::cout << "building pos_x: " << building.GetPosition().x << "\n";
            std::cout << "building pos_z: " << building.GetPosition().z << "\n";
            
            float penetration_depth = TANK_RADIUS - distance;
            
            direction_x /= distance;
            direction_z /= distance;
            
            glm::vec3 new_player_pos = player_tank_.GetPosition();
            glm::vec3 new_camera_pos = camera->GetPosition();

            
            new_player_pos.x += penetration_depth * direction_x;
            new_player_pos.z += penetration_depth * direction_z;
            std::cout << "new_player_pos_x:" << new_player_pos.x << '\n';
            std::cout << "new_player_pos_z:" << new_player_pos.z << '\n';
            std::cout << "new_player_pos_y:" << new_player_pos.y << '\n';

            new_camera_pos.x += penetration_depth * direction_x;
            new_camera_pos.z += penetration_depth * direction_z;

            player_tank_.SetPosition(new_player_pos);
            camera->SetPosition(new_camera_pos);
        }
    }
}

void LogicEngine::CheckEnemyTanksCollisions() {
    // check tank - tank collisions
    if (enemy_tanks_.size() >= 2) {
        for (auto i = enemy_tanks_.begin(); i != std::prev(enemy_tanks_.end()); ++i) {
            for (auto j = std::next(i); j != enemy_tanks_.end(); ++j) {
                const float distance = glm::distance(i->GetPosition(), j->GetPosition());
                if (distance < 2 * TANK_RADIUS && distance != 0.0f) {
                    glm::vec3 dif = j->GetPosition() - i->GetPosition();
                    float P = TANK_RADIUS * 2 - distance;
                    glm::vec3 P2 = P * glm::normalize(dif);
                    i->SetPosition(i->GetPosition() + P2 * -0.5f);
                }
            }
        }
    }
    
    for (auto &enemy_tank : enemy_tanks_) {
        // check out of map
        if (enemy_tank.GetPosition().x + TANK_RADIUS > MAP_SIZE / 2.0f) {
            const float distance_x = MAP_SIZE / 2.0f - enemy_tank.GetPosition().x;
            float p_x = TANK_RADIUS - distance_x;
            glm::vec3 p2_x = glm::vec3(p_x, 0, 0) * 0.5f;
            enemy_tank.SetPosition(enemy_tank.GetPosition() - p2_x);
        }
        else if (enemy_tank.GetPosition().x - TANK_RADIUS < -MAP_SIZE / 2.0f) {
            const float distance_x = MAP_SIZE / 2.0f + enemy_tank.GetPosition().x;
            float p_x = TANK_RADIUS - distance_x;
            glm::vec3 p2_x = glm::vec3(p_x, 0, 0) * 0.5f;
            enemy_tank.SetPosition(enemy_tank.GetPosition() + p2_x);
        }

        if (enemy_tank.GetPosition().z + TANK_RADIUS > MAP_SIZE / 2.0f) {
            const float distance_z = MAP_SIZE / 2.0f - enemy_tank.GetPosition().z;
            float p_z = TANK_RADIUS - distance_z;
            glm::vec3 p2_z = glm::vec3(0, 0, p_z) * 0.5f;
            enemy_tank.SetPosition(enemy_tank.GetPosition() - p2_z);
        }
        else if (enemy_tank.GetPosition().z - TANK_RADIUS < -MAP_SIZE / 2.0f) {
            const float distance_z = MAP_SIZE / 2.0f + enemy_tank.GetPosition().z;
            float p_z = TANK_RADIUS - distance_z;
            glm::vec3 p2_z = glm::vec3(0, 0, p_z) * 0.5f;
            enemy_tank.SetPosition(enemy_tank.GetPosition() + p2_z);
        }

        // check buildings collisions
        for (auto building : map_.GetBuildings()) {
            // find closest points
            float closest_x = std::max(building.GetPosition().x - building.GetHalfWidth(),
                                       std::min(enemy_tank.GetPosition().x,
                                                building.GetPosition().x + building.GetHalfWidth()));
            float closest_y = std::max(building.GetPosition().y - building.GetHalfHeight(),
                                       std::min(enemy_tank.GetPosition().y,
                                                building.GetPosition().y + building.GetHalfHeight()));
            float closest_z = std::max(building.GetPosition().z - building.GetHalfLength(),
                                       std::min(enemy_tank.GetPosition().z,
                                                building.GetPosition().z + building.GetHalfLength()));

            // find distance
            float direction_x = enemy_tank.GetPosition().x - closest_x;
            float direction_y = enemy_tank.GetPosition().y - closest_y;
            float direction_z = enemy_tank.GetPosition().z - closest_z;
            float distance = std::sqrt(
                direction_x * direction_x + direction_y * direction_y + direction_z * direction_z);

            // check for collision
            if (distance < TANK_RADIUS) {
                float penetration_depth = TANK_RADIUS - distance;
                
                float length = std::sqrt(
                    direction_x * direction_x + direction_y * direction_y + direction_z * direction_z);
                direction_x /= length;
                direction_y /= length;
                direction_z /= length;
                
                glm::vec3 new_tank_pos = enemy_tank.GetPosition();
                new_tank_pos.x += penetration_depth * direction_x;
                new_tank_pos.y += penetration_depth * direction_y;
                new_tank_pos.z += penetration_depth * direction_z;

                enemy_tank.SetPosition(new_tank_pos);
            }
        }
    }
}

void LogicEngine::CheckProjectileBuildingCollisions() {
    for (auto &projectile : projectiles_) {
        for (auto building : map_.GetBuildings()) {
            // find closest points
            float closest_x = std::max(building.GetPosition().x - building.GetHalfWidth(),
                                       std::min(projectile.GetPosition().x,
                                                building.GetPosition().x + building.GetHalfWidth()));
            float closest_y = std::max(building.GetPosition().y - building.GetHalfHeight(),
                                       std::min(projectile.GetPosition().y,
                                                building.GetPosition().y + building.GetHalfHeight()));
            float closest_z = std::max(building.GetPosition().z - building.GetHalfLength(),
                                       std::min(projectile.GetPosition().z,
                                                building.GetPosition().z + building.GetHalfLength()));

            // find distance
            float direction_x = projectile.GetPosition().x - closest_x;
            float direction_y = projectile.GetPosition().y - closest_y;
            float direction_z = projectile.GetPosition().z - closest_z;
            float distance = std::sqrt(
                direction_x * direction_x + direction_y * direction_y + direction_z * direction_z);

            // check for collision
            if (distance < PROJECTILE_RADIUS) {
                projectile.SetHit(true);
            }
        }
    }
}
