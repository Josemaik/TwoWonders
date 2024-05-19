#pragma once

#include <map>
#include <tuple>
#include "particle_component.hpp"
#include "pointlight_component.hpp"

struct SpawnComponent
{
    bool active{ false };
    std::vector<std::tuple<RenderComponent*, ParticleMakerComponent*, PointLightComponent*>> parts{};
};