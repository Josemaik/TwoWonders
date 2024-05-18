#pragma once

#include<map>
#include "../motor/src/darkmoon.hpp"

// enum animationtypes

enum struct PlayerAnimations
{
    NORMAL_WALK,
    MELEE_ATTACK,
    RANGED_ATTACK,
    GOT_HIT,
    DEATH,
    SPEAKING,
    MAKING_POTION,
    USING_POTION,
    OPEN_INVENTORY,
    STAFF_WALK,
    IDLE
};

struct AnimationComponent
{
    // std::vector<std::string>{};
    std::map<std::size_t, Animation*> animationList{};
    std::size_t currentAnimation{ std::numeric_limits<std::size_t>::max() };
};
