#include "tank.h"

#include <iostream>

#include "constants.h"
#include "enemy_tank.h"
#include "camera.h"

using namespace world_of_tanks;

void Tank::FireProjectile(std::vector<Projectile> &projectiles, float speed) {
    glm::vec3 offset;
    offset.y = PROJECTILE_OFFSET_UP;
    offset.x = PROJECTILE_OFFSET_FORWARD * sin(body_rotation_ + turret_rotation_);
    offset.z = PROJECTILE_OFFSET_FORWARD * cos(body_rotation_ + turret_rotation_);
    
    const auto projectile = Projectile(position_ + offset, body_rotation_ + turret_rotation_, speed);

    projectiles.push_back(projectile);
}

void Tank::CheckIfHit(std::vector<Projectile> &projectiles) {
    for (auto &projectile : projectiles) {
        if (CheckTurretHit(projectile.GetPosition())) {
            // WE GOT HIT!!
            // std::cout <<"HIT\n";
            health_--;
            projectile.SetHit(true);
        }
    }
}

inline bool Tank::CheckTurretHit(glm::vec3 projectile_pos) {
    return glm::distance(projectile_pos, position_) < TURRET_RADIUS + PROJECTILE_RADIUS;
}

// inline bool Tank::CheckCannonHit(glm::vec3 projectile_pos) {
//     glm::vec3 cannon_pos;
//     cannon_pos.x = position_.x + CANNON_HALF_LENGTH * sin(body_rotation_ + turret_rotation_);
//     cannon_pos.y = position_.y;
//     cannon_pos.z = position_.z + CANNON_HALF_LENGTH * cos(body_rotation_ + turret_rotation_);
//     return true;
// }
