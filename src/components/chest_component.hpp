#pragma once
#include "object_component.hpp"

struct ChestComponent
{
    uint8_t id{};
    uint16_t zone{};
    bool isOpen{ false };
    bool showButton{ false };

    vec3d dropPosition{};
    ObjectType content{ ObjectType::None };
};