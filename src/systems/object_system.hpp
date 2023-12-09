#pragma once 
#ifndef OBJECT_SYSTEM
#define OBJECT_SYSTEM
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"

struct ObjectSystem
{
    using SYSCMPs = MP::TypeList<ObjectComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, float deltaTime);
    void explodeBomb(EntityManager& em, Entity& ent);
};

#endif // !OBJECT_SYSTEM