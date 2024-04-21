#pragma once
#ifndef OBJECT_SYSTEM
#define OBJECT_SYSTEM
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"

struct ObjectSystem
{
    using SYSCMPs = MP::TypeList<ObjectComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em);
    void addObject(ObjectType type, vec3d pos);

private:
    void explodeBomb(EntityManager& em, Entity& ent);
    void createExplodeBomb(EntityManager& em, Entity& ent, BehaviorType type, Color color);
    void explodeBombHeal(EntityManager& em, Entity& ent);

    // Shop
    bool buyBomb(EntityManager& em);
    bool buyExtraLife(EntityManager& em, Entity* ent);
    bool buyLife(EntityManager& em, Entity* ent);
    void createObjects(EntityManager& em);

    // Vector de crear objetos
    std::vector<std::pair<ObjectType, vec3d>> toCreate{};
};

#endif // !OBJECT_SYSTEM
