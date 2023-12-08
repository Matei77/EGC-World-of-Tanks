#pragma once
#include <glm/vec3.hpp>

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace world_of_tanks {
    const float DEFAULT_FOV_Y = RADIANS(60);
    constexpr float DEFAULT_Z_NEAR = 0.01f;
    constexpr float DEFAULT_Z_FAR = 200.0f;
    
    constexpr glm::vec3 CAMERA_OFFSET = glm::vec3(0, 2, 5);
    constexpr glm::vec3 ABSOLUTE_UP = glm::vec3(0, 1, 0);

    const glm::vec3 TANK_BODY_COLOR = NormalizedRGB(5, 74, 46);
    const glm::vec3 TANK_TURRET_COLOR = NormalizedRGB(25, 77, 56);
    const glm::vec3 TANK_TRACKS_COLOR = NormalizedRGB(214, 214, 214);
    const glm::vec3 TANK_CANNON_COLOR = NormalizedRGB(214, 214, 214);
    const glm::vec3 TERRAIN_COLOR = NormalizedRGB(76, 92, 56);
}
