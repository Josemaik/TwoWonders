#pragma once
#include <functional>
#include <utils/BBox.hpp>
#include "attack_component.hpp"

enum BehaviorType
{
    NOTHING = 0x00,
    PLAYER = 0x01,
    ENEMY = 0x02,
    STATIC = 0x04,
    ATK_PLAYER = 0x08,
    ATK_ENEMY = 0x10,
    ZONE = 0x20,
    SHIELD = 0x40,
    ENDING = 0x80,
    HEAL = 0x100,
    AREADAMAGE = 0x200,
    SPIDERWEB = 0x400,
    WARNINGZONE = 0x800,
    METEORITE = 0X1000,
    AREADAMAGECRUSHER = 0x2000,
    SPAWN = 0x4000,
    RAMP = 0x8000,
    LADDER = 0x10000,
};

struct ColliderComponent
{
    ColliderComponent() = default;
    ColliderComponent(vec3d pos, vec3d scale, BehaviorType behavior)
        : boundingBox{ pos, scale }, behaviorType{ behavior } {};

    void updateBox(vec3d pos, vec3d scale, double gravity, double orientation, vec3d& rotationVec);
    void rotatePointY(vec3d& point, double cosAngle, double sinAngle);

    BBox boundingBox{};
    BehaviorType behaviorType;
    AttackType attackType{ AttackType::None };
};
