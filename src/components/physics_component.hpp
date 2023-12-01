#pragma once
#include "utils/vec3D.hpp"

struct PhysicsComponent
{
    vec3f position{};
    vec3f velocity{};
    float immobilizeTimer = 0.0f;
    float invincibilityTimer = 0.0f;
};
