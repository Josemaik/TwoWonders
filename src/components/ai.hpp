#pragma once
#include <array>
#include <cstdint>
#include <utils/vec3D.hpp>
struct AIComponent
{
    static constexpr vec3f invalid{};
    static constexpr uint8_t max_patrol { 10 };
    std::array<vec3f,max_patrol> patrol{};
    uint8_t current{};
    float arrival_radius { 1.f };
    float detect_radius { 2.f };
    bool playerdetected {false};
};
