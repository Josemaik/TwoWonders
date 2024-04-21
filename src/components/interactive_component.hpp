#pragma once
#include <cstdint>

enum struct InteractableType : uint8_t
{
    Chest = 1,
    Destructible = 2,
    Door = 3,
    Level = 4,
    Spawn = 6,
    Lever = 7,
    DamageObj = 8,
    Roca = 9,
    MissionOBJ = 10,
    Table = 11,
    Ladder = 12,
    Sign = 13,
    Camp = 14,
    NPC = 17,
};

struct InteractiveComponent
{
    double range{ 0.0 };
    bool showButton{ false };
    bool showLock{ false };
};