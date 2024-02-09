#pragma once 
#ifndef LIFE_SYSTEM
#define LIFE_SYSTEM
#include <set>
#include <functional>
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include "object_system.hpp"

struct LifeSystem
{
    using SYSCMPs = MP::TypeList<LifeComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, ObjectSystem& os, float deltaTime);

private:
    void createObject(EntityManager& em, ObjectSystem& os, vec3d pos);
};

#endif // !LIFE_SYSTEM