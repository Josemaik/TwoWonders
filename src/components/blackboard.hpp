#pragma once

#include <cstdint>
#include <utils/path.hpp>
#include "ai_component.hpp"

struct BlackBoard_t {
    double tx { 0.0 } , tz { 0.0 };
    // SB behaviour { SB::Arrive };
    bool tactive { false };
    //Target Entity
    std::size_t teid{};
    //Path
    Path_t<4> path { vec3d{8.0, 0.0, 4.0} , {3.0,0.0,4.0} , {3.0,0.0,5.0}, {8.0,0.0,5.0} };
};
