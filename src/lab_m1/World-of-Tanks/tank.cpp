#include "tank.h"

#include "constants.h"

void world_of_tanks::Tank::FireProjectile(std::vector<Projectile> &projectiles) {
    glm::vec3 offset;
    offset.y = PROJECTILE_OFFSET_UP;
    offset.x = PROJECTILE_OFFSET_FORWARD * sin(body_rotation_ + turret_rotation_);
    offset.z = PROJECTILE_OFFSET_FORWARD * cos(body_rotation_ + turret_rotation_);
    
    const auto projectile = Projectile(position_ + offset, body_rotation_ + turret_rotation_);

    projectiles.push_back(projectile);
}
