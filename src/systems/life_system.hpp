#pragma once 
#ifndef LIFE_SYSTEM
#define LIFE_SYSTEM
#include <set>
#include <functional>
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"

struct LifeSystem
{
    using SYSCMPs = MP::TypeList<LifeComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, float deltaTime);

private:
    void createObject(EntityManager& em, vec3f pos);
    deathSet dead_entities{};
};

#endif // !LIFE_SYSTEM