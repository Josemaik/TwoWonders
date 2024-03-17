#pragma once
#include <cstdint>

enum struct InteractableType : uint8_t
{
    Chest = 1,
    Destructible = 2,
    Door = 3,
    Level = 4,
    Spawn = 5,
    Lever = 6,
};

struct InteractiveComponent
{
    bool showButton{ false };
    bool showLock{ false };
};