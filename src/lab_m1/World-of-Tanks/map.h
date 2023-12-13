#pragma once
#include <vector>

#include "building.h"

namespace world_of_tanks {
    class Map {
    private:
        std::vector<Building> buildings_;
    public:
        Map() = default;

        void InitMap();

        const std::vector<Building> & GetBuildings() const { return buildings_; }
    };
}
