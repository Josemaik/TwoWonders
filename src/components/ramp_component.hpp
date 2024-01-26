#pragma once
#include "utils/types.hpp"

struct RampComponent
{
    vec2d min{};
    vec2d max{};
    double slope{};
    vec2d offset{};
};