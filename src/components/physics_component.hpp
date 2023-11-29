#pragma once
#include <raylib.h>

struct PhysicsComponent
{
    Vector3 position{};
    Vector3 velocity{};
    Vector3 direction{};

    BoundingBox getBoundingBox(Vector3 scale);

};
