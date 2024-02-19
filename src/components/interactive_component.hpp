#pragma once
#include <cstdint>

enum struct InteractableType : uint8_t
{
    Chest = 1,
    Spawn = 2,
    Door = 3
};

struct InteractiveComponent
{
    bool showButton{ false };
};