#pragma once

#include <map>
#include <tuple>
#include "particle_component.hpp"
#include "pointlight_component.hpp"

struct SpawnComponent
{
    enum struct SpawnParts { BONFIRE, TORCH, CAULDRON };

    bool active{ false };
    std::map<SpawnParts, std::tuple<RenderComponent*, ParticleMakerComponent*, PointLightComponent*>> parts{};
};