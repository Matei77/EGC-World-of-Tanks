#include "projectile.h"

#include "constants.h"

void world_of_tanks::Projectile::UpdateProjectile(float delta_time) {
    position_.x -= PROJECTILE_SPEED * delta_time * sin(fired_rotation_y_);
    position_.z -= PROJECTILE_SPEED * delta_time * cos(fired_rotation_y_);
    time_ += delta_time;
}
