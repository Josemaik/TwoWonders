#pragma once
#include "object_component.hpp"
#include <queue>

struct ChestComponent
{
    uint8_t id{};
    uint16_t zone{};
    bool isOpen{ false };

    vec3d dropPosition{}, viewPoint{}, posWall{}, scaleWall{};
    ObjectType content{ ObjectType::None };
    std::queue<std::string> messages{};
};