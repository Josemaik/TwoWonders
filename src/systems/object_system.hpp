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

private:
    void explodeBomb(EntityManager& em, Entity& ent);
    void createExplodeBomb(EntityManager& em, Entity& ent, BehaviorType type);
    void explodeBombHeal(EntityManager& em, Entity& ent);

    // Shop
    bool buyBomb(EntityManager& em, Entity* ent);
    bool buyExtraLife(EntityManager& em, Entity* ent);
    bool buyLife(EntityManager& em, Entity* ent);
    deathSet dead_entities{};
};

#endif // !OBJECT_SYSTEM
