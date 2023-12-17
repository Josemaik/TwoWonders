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

private:
    std::set<std::size_t, std::greater<std::size_t>> dead_entities{};
};

#endif // !PROJECTILE_SYSTEM