#pragma once
#include "utils/types.hpp"

enum struct RampType : uint8_t
{
    Normal,
    Triangular
};

struct RampComponent
{
    vec2d min{};
    vec2d max{};
    double slope{};
    vec3d offset{};
    RampType type{ RampType::Normal };
};