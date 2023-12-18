#include "enemy_tank.h"

#include <iostream>

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
    float distance = glm::distance(position_, player_tank.GetPosition());

    if (distance < ENEMY_ATTACK_RADIUS) {
        glm::vec3 direction = position_ - player_tank.GetPosition();

        float target_angle = std::atan2(direction.x, direction.z);
        // std::cout << "target_angle: " << target_angle * (180.0 / M_PI) << "\n";
        // std::cout << "turret_rotation: " << turret_rotation_ * (180.0 / M_PI) << "\n";
        // std::cout << "body_rotation: " << body_rotation_ * (180.0 / M_PI) << "\n";

        // if (std::fmod(turret_rotation_, 2.0f * M_PI) == angle_rad) {
        //     // Fire
        // } else
        // if (std::fmod(turret_rotation_ + body_rotation_ + RADIANS(270), 2.0f * M_PI) < std::fmod(angle_rad - RADIANS(3), 2.0f * M_PI)) {
        //     turret_rotation_ -= 1 * delta_time;
        // } else if (std::fmod(turret_rotation_ + body_rotation_ + RADIANS(270), 2.0f * M_PI) > std::fmod(angle_rad + RADIANS(3), 2.0f * M_PI)) {
        //     turret_rotation_ += 1 * delta_time;
        // }

        float angle_diff = target_angle - turret_rotation_ - body_rotation_;
        while (angle_diff >= M_PI) angle_diff -= 2.0f * M_PI;
        while (angle_diff < -M_PI) angle_diff += 2.0f * M_PI;

        const int shoot_angle_offset = 2;

        if (angle_diff < RADIANS(shoot_angle_offset) && angle_diff > -RADIANS(shoot_angle_offset)) {
            if (reload_timer_.IsFinished()) {
                FireProjectile(projectiles, ENEMY_PROJECTILE_SPEED);
                reload_timer_.ResetTimer();
            } else {
                reload_timer_.UpdateTimer(delta_time);
            }
            turret_rotation_ += angle_diff;
            
        } else if (angle_diff > RADIANS(shoot_angle_offset)) {
            if (current_move_ == ROTATE_LEFT) {
                // turret_rotation_ += TURRET_ROTATION_SPEED * delta_time;
                
            } else if (current_move_ == ROTATE_RIGHT) {
                turret_rotation_ += 2 * TURRET_ROTATION_SPEED * delta_time;
                
            } else {
                turret_rotation_ += TURRET_ROTATION_SPEED * delta_time;
            }
            
        } else if (angle_diff < -RADIANS(shoot_angle_offset)) {
            if (current_move_ == ROTATE_LEFT) {
                turret_rotation_ -= 2 * TURRET_ROTATION_SPEED * delta_time;
                
            } else if (current_move_ == ROTATE_RIGHT) {
                // turret_rotation_ += TURRET_ROTATION_SPEED * delta_time;
                
            } else {
                turret_rotation_ -= TURRET_ROTATION_SPEED * delta_time;
            }
        }
        // turret_rotation_ += angle_diff;

        // if (turret_rotation_ < turret_rotation_ + angle_diff - RADIANS(1)) {
        //     if (current_move_ == ROTATE_LEFT) {
        //         // turret_rotation_ += TURRET_ROTATION_SPEED * delta_time;
        //         
        //     } else if (current_move_ == ROTATE_RIGHT) {
        //         turret_rotation_ += 2 * TURRET_ROTATION_SPEED * delta_time;
        //         
        //     } else {
        //         turret_rotation_ += TURRET_ROTATION_SPEED * delta_time;
        //     }
        //     
        // } else if (turret_rotation_ > turret_rotation_ + angle_diff + RADIANS(1)) {
        //     if (current_move_ == ROTATE_LEFT) {
        //         turret_rotation_ -= 2 * TURRET_ROTATION_SPEED * delta_time;
        //         
        //     } else if (current_move_ == ROTATE_RIGHT) {
        //         // turret_rotation_ += TURRET_ROTATION_SPEED * delta_time;
        //         
        //     } else {
        //         turret_rotation_ -= TURRET_ROTATION_SPEED * delta_time;
        //     }
        // } else {
        //     if (reload_timer_.IsFinished()) {
        //         FireProjectile(projectiles, ENEMY_PROJECTILE_SPEED);
        //         reload_timer_.ResetTimer();
        //     } else {
        //         reload_timer_.UpdateTimer(delta_time);
        //     }
        // }
        // // turret_rotation_ += angle_diff;
    }
}
