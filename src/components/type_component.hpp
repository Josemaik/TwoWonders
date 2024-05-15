#pragma once
#include <array>

enum struct ElementalType : uint8_t
{
    Neutral = 1,
    Water = 2,
    Fire = 3,
    Ice = 4
};

struct TypeComponent
{
    ElementalType type{ ElementalType::Neutral };
};