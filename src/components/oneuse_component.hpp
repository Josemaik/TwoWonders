#pragma once
#include <cstdint>
#include <set>

struct OneUseComponent
{
    uint8_t id{};
    std::set<uint16_t> zones{};
};