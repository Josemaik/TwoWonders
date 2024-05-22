#pragma once
#include <array>
#include "../utils/BBox.hpp"
#include <set>

struct Navmesh
{
    /* data */
    std::set<std::pair<uint16_t, vec3d>> nodes{};
    BBox box{};
    std::pair<uint16_t, vec3d>  centerpoint{};
    bool ramp{false};
};
