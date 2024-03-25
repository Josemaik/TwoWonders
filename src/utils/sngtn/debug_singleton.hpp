#pragma once

#include <cstdint>
// #include "../path.hpp"
// #include "ai_component.hpp"

struct Debug_t {
    std::size_t IA_id{};
    std::size_t IA_id_debug{};
    float elapsed{ 1.0 }, countdown{ 2.0 };
    void plusDeltatime(float deltaTime, float elapsed) { elapsed += deltaTime; };
    const char* text{};
    std::vector<vec3d> path{};
    //start node
    float startnode{ 0.0f };
    float goalnode{ 0.0f };
};
