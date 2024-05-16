#pragma once

#include<map>
#include "../motor/src/darkmoon.hpp"

// enum animationtypes

struct AnimationComponent
{
    // std::vector<std::string>{};
    std::map<std::size_t,Animation*> animations{};
    std::size_t currentanimation{std::numeric_limits<std::size_t>::max()};
};
