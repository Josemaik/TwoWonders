#pragma once
#include <array>
#include <cstdint>
#include <utils/vec3D.hpp>
struct PatrolComponent : AIComponent
{
    //patrol enemy
    static constexpr uint8_t max_patrol{ 10 };
     std::array<vec3f, max_patrol> patrol{};
    static constexpr vec3f invalid{};
    uint8_t current{};
    float arrival_radius{ 1.f };
    bool arrived{ false };
    uint8_t nexttarget{ 1 };
};
