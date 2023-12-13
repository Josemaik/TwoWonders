#pragma once
#include <functional>
#include <utils/BBox.hpp>

enum BehaviorType {
    NOTHING = 0x00,
    PLAYER = 0x01,
    ENEMY = 0x02,
    STATIC = 0x04,
    ATK_PLAYER = 0x08,
    ATK_ENEMY = 0x10
};

struct ColliderComponent
{
    ColliderComponent() = default;
    ColliderComponent(vec3f pos, vec3f scale, BehaviorType behavior)
        : boundingBox{ pos, scale }, behaviorType{ behavior } {};

    void updateBox(vec3f pos, vec3f scale);
    BBox boundingBox{};
    BehaviorType behaviorType;
};
