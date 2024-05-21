#pragma once

#include<map>
#include "../motor/src/darkmoon.hpp"

// enum animationtypes

enum struct PlayerAnimations : uint8_t
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

enum struct CrusherAnimations : uint8_t
{
    ATTACK
};

enum struct ChestAnimations : uint8_t
{
    OPEN
};

enum struct DoorAnimations : uint8_t
{
    OPEN
};

enum struct NPCAnimations : uint8_t
{
    WALK,
    IDLE,
    SPEAKING,
    GIVE_ITEM
};

enum struct RockAnimations : uint8_t
{
    ROLL,
    REPOSITION
};

enum struct SnowmanAnimations : uint8_t
{
    ATTACK,
    GOT_HIT,
    IDLE,
    DEATH
};

enum struct GolemAnimations : uint8_t
{
    WALK,
    ATTACK,
    DEATH,
    GOT_HIT
};

struct AnimationComponent
{
    static constexpr std::size_t max = std::numeric_limits<std::size_t>::max();
    // std::vector<std::string>{};

    void reset()
    {
        currentAnimation = max;
        animToPlay = max;
        currentTime = 0.0f;
        animEnded = false;
        aboutToEnd = false;
    }

    void changeAnimation(std::size_t animID)
    {
        stopAnim = true;
        animToPlay = animID;
    }

    std::vector<Animation*> animationList{};
    std::size_t currentAnimation{ max };
    std::size_t animToPlay{ max };
    std::size_t idCurrent{ max };
    float multiplier{ 1.0 };
    float currentTime{ 0.0f };
    bool stopAnim{ false }, animEnded{ false }, aboutToEnd{ false };
};
