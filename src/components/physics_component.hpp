#pragma once
#include "utils/vec3D.hpp"

struct PhysicsComponent
{
    static constexpr float KGravity{ 1 };
    static constexpr float KMinVy{ -.6f };
    static constexpr float KMaxVy{ .6f };
    static constexpr float MAX_SPEED{ .6f };

    vec3f position{};
    vec3f velocity{};
    float gravity{ KGravity };
    //bool unCheckGravity{ false };
};
