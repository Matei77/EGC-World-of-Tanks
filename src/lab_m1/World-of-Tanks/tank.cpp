#include "tank.h"

#include "constants.h"
#include "camera.h"

using namespace world_of_tanks;

void Tank::FireProjectile(std::vector<Projectile> &projectiles, const float speed) const {
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
            health_--;
            projectile.SetHit(true);
        }
    }
}

inline bool Tank::CheckTurretHit(const glm::vec3 projectile_pos) const {
    return glm::distance(projectile_pos, position_) < TURRET_RADIUS + PROJECTILE_RADIUS;
}
