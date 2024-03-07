#pragma once
#ifndef PHYSICS_SYSTEM
#define PHYSICS_SYSTEM
#include "../utils/types.hpp"

struct PhysicsSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, float deltaTime);

private:
    float elapsed{ 0.f }, elapsed_limit{ 1.0f };
};

#endif // !PHYSICS_SYSTEM
