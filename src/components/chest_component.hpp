#pragma once
#include "object_component.hpp"
#include <queue>

struct ChestComponent
{
    bool isOpen{ false }, checkCrushers{ false };

    vec3d dropPosition{};
    ObjectType content{ ObjectType::None };
};