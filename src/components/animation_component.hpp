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
    static constexpr std::size_t max = std::numeric_limits<std::size_t>::max();
    // std::vector<std::string>{};
    std::vector<Animation*> animationList{};
    std::size_t currentAnimation{ max };
    std::size_t animToPlay{ max };
    std::size_t idCurrent{ max };
    float multiplier{ 1.0 };
    float timer{ 0.0f }, currentTime{ 0.0f };
    bool stopAnim{ false }, animEnded{ false }, aboutToEnd{ false };
};
