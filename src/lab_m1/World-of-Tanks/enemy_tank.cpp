#include "enemy_tank.h"

using namespace world_of_tanks;

void EnemyTank::MoveTank(const float delta_time) {
    move_timer_.UpdateTimer(delta_time);
    
    if (move_timer_.IsFinished()) {
        current_move_ = static_cast<MoveList>(rand() % MOVES_NR);
        move_timer_.SetNewTimeAndReset(rand() % 50 / 10.0f + 1);
    }
    
    switch (current_move_) {
    case FORWARD:
        // Move forward
        position_.x -= TANK_SPEED * delta_time * sin(body_rotation_);
        position_.z -= TANK_SPEED * delta_time * cos(body_rotation_);
        break;
        
    case BACK:
        // Move back
        position_.x += TANK_SPEED * delta_time * sin(body_rotation_);
        position_.z += TANK_SPEED * delta_time * cos(body_rotation_);
        break;
        
    case ROTATE_LEFT:
        // Rotate left
        body_rotation_ += 1 * delta_time;
        break;
        
    case ROTATE_RIGHT:
        // Rotate right
        body_rotation_ -= 1 * delta_time;
        break;
        
    case STAND:
    case MOVES_NR:
        // Stand still
        break;
    }
}

void EnemyTank::ShootAtPlayer(const float delta_time, const Tank& player_tank, std::vector<Projectile> &projectiles) {
    const float distance = glm::distance(position_, player_tank.GetPosition());

    if (distance < ENEMY_ATTACK_RADIUS) {
        const glm::vec3 direction = position_ - player_tank.GetPosition();

        const float target_angle = std::atan2(direction.x, direction.z);
        float angle_diff = target_angle - turret_rotation_ - body_rotation_;
        while (angle_diff >= M_PI) angle_diff -= 2.0f * M_PI;
        while (angle_diff < -M_PI) angle_diff += 2.0f * M_PI;

        if (angle_diff < RADIANS(SHOOT_ANGLE_OFFSET) && angle_diff > -RADIANS(SHOOT_ANGLE_OFFSET)) {
            if (reload_timer_.IsFinished()) {
                FireProjectile(projectiles, ENEMY_PROJECTILE_SPEED);
                reload_timer_.ResetTimer();
            } else {
                reload_timer_.UpdateTimer(delta_time);
            }
            turret_rotation_ += angle_diff;
            
        } else if (angle_diff > RADIANS(SHOOT_ANGLE_OFFSET)) {
            if (current_move_ == ROTATE_LEFT) {
                
            } else if (current_move_ == ROTATE_RIGHT) {
                turret_rotation_ += 2 * TURRET_ROTATION_SPEED * delta_time;
                
            } else {
                turret_rotation_ += TURRET_ROTATION_SPEED * delta_time;
            }
            
        } else if (angle_diff < -RADIANS(SHOOT_ANGLE_OFFSET)) {
            if (current_move_ == ROTATE_LEFT) {
                turret_rotation_ -= 2 * TURRET_ROTATION_SPEED * delta_time;
                
            } else if (current_move_ == ROTATE_RIGHT) {
                
            } else {
                turret_rotation_ -= TURRET_ROTATION_SPEED * delta_time;
            }
        }
    }
}
