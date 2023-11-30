#pragma once
#include "utils/vec3D.hpp"

struct PhysicsComponent
{
    vec3f position{};
    vec3f velocity{};
    vec3f direction{};
};
