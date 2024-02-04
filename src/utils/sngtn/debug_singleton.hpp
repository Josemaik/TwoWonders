#pragma once

#include <cstdint>
// #include <utils/path.hpp>
// #include "ai_component.hpp"

struct Debug_t {
    std::size_t IA_id{};
    std::size_t IA_id_debug{};
    double elapsed{1.0},countdown{2.0};
    void plusdeltatime(double deltaTime, double& elapsed) { elapsed += deltaTime; };
    const char *text{};
};
