#pragma once
#include <glm/vec3.hpp>

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace world_of_tanks {
    constexpr float TANK_SPAWN_TIME = 3.0f;
    constexpr int MAX_ENEMY_NUM = 15;
    
    constexpr float TANK_SPEED = 3.0f;
    constexpr int TANK_HEALTH = 5;
    constexpr float TANK_RELOAD_TIMER = 1.0f;
    constexpr float TANK_ROTATION_SPEED = 1.0f;
    constexpr float TURRET_ROTATION_SPEED = 1.0f;
    constexpr int SHOOT_ANGLE_OFFSET = 2;
    
    constexpr float ENEMY_INITIAL_MOVE_TIMER = 2.0f;
    constexpr float ENEMY_ATTACK_RADIUS = 20.0f;
    constexpr float ENEMY_RELOAD_TIME = 2.0f;
    constexpr float ENEMY_PROJECTILE_SPEED = 10.0f;
    
    constexpr float PROJECTILE_SPEED = 30.0f;
    constexpr float PROJECTILE_OFFSET_UP = 0.55f;
    constexpr float PROJECTILE_OFFSET_FORWARD = -1.9f;
    constexpr float PROJECTILE_TIME_LIMIT = 7.0f;

    constexpr int GRID_NUMBER = 20;
    constexpr int MAP_SIZE = GRID_NUMBER * 4;
    constexpr float MIN_BUILDING_SIZE = 3.5f;
    constexpr int MAX_BUILDING_SIZE = 10;
    constexpr int MIN_BUILDING_NUM = 4;
    constexpr int MAX_BUILDING_NUM = 15;
    constexpr float MAP_Y_OFFSET = 0.5f;

    constexpr float GAME_TIMER = 120.0f;

    constexpr float TURRET_RADIUS = 0.6f;
    constexpr float PROJECTILE_RADIUS = 0.2f;
    constexpr float CANNON_HALF_LENGTH = 0.95f;
    constexpr float CANNON_RADIUS = 0.2f;
    constexpr float TANK_RADIUS = 1.6f;

    constexpr float ASPECT_HEIGHT = 9.0f;
    constexpr float ASPECT_WIDTH = 16.0f;
    
    constexpr float DEFAULT_FOV_Y = RADIANS(60);
    constexpr float DEFAULT_Z_NEAR = 0.01f;
    constexpr float DEFAULT_Z_FAR = 200.0f;

    constexpr float MOUSE_SENSITIVITY = 0.001f;
    
    constexpr glm::vec3 CAMERA_OFFSET = glm::vec3(0, 2, 5);
    constexpr glm::vec3 ABSOLUTE_UP = glm::vec3(0, 1, 0);

    const glm::vec3 TANK_BODY_COLOR = NormalizedRGB(5, 74, 46);
    const glm::vec3 TANK_TURRET_COLOR = NormalizedRGB(25, 77, 56);
    const glm::vec3 TANK_TRACKS_COLOR = NormalizedRGB(214, 214, 214);
    const glm::vec3 TANK_CANNON_COLOR = NormalizedRGB(214, 214, 214);
    const glm::vec3 ENEMY_TANK_BODY_COLOR = NormalizedRGB(135, 103, 55);
    const glm::vec3 ENEMY_TANK_TURRET_COLOR = NormalizedRGB(161, 123, 66);
    const glm::vec3 ENEMY_TANK_TRACKS_COLOR = NormalizedRGB(214, 214, 214);
    const glm::vec3 ENEMY_TANK_CANNON_COLOR = NormalizedRGB(214, 214, 214);
    const glm::vec3 PROJECTILE_COLOR = NormalizedRGB(43, 43, 43);
    const glm::vec3 TERRAIN_COLOR_1 = NormalizedRGB(76, 92, 56);
    const glm::vec3 TERRAIN_COLOR_2 = NormalizedRGB(95, 107, 80);
    const glm::vec3 BACKGROUND_COLOR = NormalizedRGB(41, 185, 217);
    const glm::vec3 SCREEN_CUT_COLOR = NormalizedRGB(0, 0, 0);
    const glm::vec3 BUILDING_COLOR = NormalizedRGB(117, 115, 115);

    enum MoveList {
        FORWARD,
        BACK,
        ROTATE_LEFT,
        ROTATE_RIGHT,
        STAND,
        MOVES_NR
    };
    
}
