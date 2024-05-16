#pragma once

#include<map>
#include "../motor/src/darkmoon.hpp"

// enum animationtypes

struct AnimationComponent
{
    // std::vector<std::string>{};
    std::map<std::size_t, Animation*> animationList{};
    std::size_t currentAnimation{ std::numeric_limits<std::size_t>::max() };
};
