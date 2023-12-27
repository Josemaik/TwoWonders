#pragma once
#include "utils/vec3D.hpp"

struct PhysicsComponent
{
    static constexpr double KGravity{ 1.0 };
    static constexpr double KMinVy{ -0.6 };
    static constexpr double KMaxVy{ 0.6 };
    static constexpr double MAX_SPEED{ 0.6 };

    vec3d position{};
    vec3d velocity{};
    double gravity{ KGravity };
    bool alreadyGrounded{ false };
};
