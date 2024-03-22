#pragma once
#include <array>
#include "../utils/BBox.hpp"
#include <set>

struct  Navmesh
{
    /* data */
    //array de nodos std::array<vec3d>
    std::set<std::pair<uint16_t, vec3d>> nodes{};
    BBox box{};
};
