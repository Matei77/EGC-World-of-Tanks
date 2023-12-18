#include "enemy_tank.h"

using namespace world_of_tanks;

void EnemyTank::MoveTank(const float delta_time) {
    move_timer_.UpdateTimer(delta_time);
    
    if (move_timer_.IsFinished()) {
        current_move_ = static_cast<MoveList>(rand() % MOVES_NR);
        move_timer_.SetNewTimeAndReset(rand() % 40 / 10.0f + 1);
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
