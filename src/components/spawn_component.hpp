#pragma once

#include <map>
#include "particle_component.hpp"

struct SpawnComponent
{
    enum struct SpawnParts { BONFIRE, TORCH, CAULDRON };

    bool active{ false };
    std::map<SpawnParts, ParticleMakerComponent*> parts{};
};