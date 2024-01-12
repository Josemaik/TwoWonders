#pragma once
#include <functional>
#include <utils/BBox.hpp>

enum BehaviorType {
    NOTHING = 0x00,
    PLAYER = 0x01,
    ENEMY = 0x02,
    STATIC = 0x04,
    ATK_PLAYER = 0x08,
    ATK_ENEMY = 0x10,
    ZONE = 0x20,
    SHIELD = 0x40,
    ENDING = 0x80,
};

struct ColliderComponent
{
    ColliderComponent() = default;
    ColliderComponent(vec3d pos, vec3d scale, BehaviorType behavior)
        : boundingBox{ pos, scale }, behaviorType{ behavior } {};

    void updateBox(vec3d pos, vec3d scale, double gravity, double orientation);
    vec3d rotateY(vec3d point, vec3d center, double angle);

    BBox boundingBox{};
    BehaviorType behaviorType;
};
