#pragma once 
#ifndef PROJECTILE_SYSTEM
#define PROJECTILE_SYSTEM
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"

struct ProjectileSystem
{
    using SYSCMPs = MP::TypeList<ProjectileComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, float deltaTime);
};

#endif // !PROJECTILE_SYSTEM