#include "map.h"

#include "constants.h"

using namespace world_of_tanks;

void Map::InitMap() {
    // spawn buildings randomly
    int buildings_num = rand() % MAX_BUILDING_NUM + MIN_BUILDING_NUM;
    
    for (int i = 0; i < buildings_num; i++) {
        float height = (rand() % (MAX_BUILDING_SIZE * 10)) / 10 + MIN_BUILDING_SIZE;
        float width = (rand() % (MAX_BUILDING_SIZE * 10)) / 10 + MIN_BUILDING_SIZE;
        float length = (rand() % (MAX_BUILDING_SIZE * 10)) / 10 + MIN_BUILDING_SIZE;
        float x_pos = (rand() % (MAP_SIZE * 4 * 10)) / 10 - static_cast<float>(MAP_SIZE * 4) / 2;
        float z_pos = (rand() % (MAP_SIZE * 4 * 10)) / 10 - static_cast<float>(MAP_SIZE * 4) / 2;
        
        glm::vec3 position = glm::vec3(x_pos, height / 2 - MAP_Y_OFFSET, z_pos);
        buildings_.emplace_back(position, height, width, length);
    }
}
