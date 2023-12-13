#include "tank_spawner.h"

using namespace world_of_tanks;

void TankSpawner::UpdateTankSpawner(float delta_time, std::vector<EnemyTank> &enemy_tanks) {
    const int enemy_count = enemy_tanks.size();
    if (timer_.IsFinished() && enemy_count < MAX_ENEMY_NUM) {
        timer_.ResetTimer();
        const float x_pos = (rand() % (MAP_SIZE * 4 * 10)) / 10 - static_cast<float>(MAP_SIZE * 4) / 2;
        const float z_pos = (rand() % (MAP_SIZE * 4 * 10)) / 10 - static_cast<float>(MAP_SIZE * 4) / 2;
        const float body_rotation = RADIANS(rand() % 360);
        enemy_tanks.emplace_back(glm::vec3(x_pos, 0, z_pos), body_rotation);
    } else {
        timer_.UpdateTimer(delta_time);
    }
}
