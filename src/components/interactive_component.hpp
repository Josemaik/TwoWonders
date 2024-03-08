#pragma once
#include <cstdint>

enum struct InteractableType : uint8_t
{
    Chest = 1,
    Destructible = 2,
    Door = 3,
    Level = 4,
    NonDestructible = 5,
    Spawn = 6,
};

struct InteractiveComponent
{
    bool showButton{ false };
};