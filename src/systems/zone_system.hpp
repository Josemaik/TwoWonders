#pragma once 
#ifndef ZONE_SYSTEM
#define ZONE_SYSTEM
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include "../managers/ia_manager.hpp"

struct ZoneSystem
{
    using SYSCMPs = MP::TypeList<ZoneComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, ENGI::GameEngine& engine);
};

#endif // !ZONE_SYSTEM