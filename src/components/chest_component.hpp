#pragma once
#include "object_component.hpp"

struct ChestComponent
{
    bool isOpen{ false }, checkCrushers{ false };

    ObjectType content{ ObjectType::None };
    uint8_t closeEnemies{}, maxEnemies{};
};