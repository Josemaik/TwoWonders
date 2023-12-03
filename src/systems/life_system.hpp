#pragma once 
#ifndef LIFE_SYSTEM
#define LIFE_SYSTEM
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"

struct LifeSystem
{
    using SYSCMPs = MP::TypeList<LifeComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em);
};

#endif // !RENDER_SYSTEM