#pragma once
#ifndef PHYSICS_SYSTEM
#define PHYSICS_SYSTEM
#include "../utils/types.hpp"

struct PhysicsSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em);
};

#endif // !PHYSICS_SYSTEM