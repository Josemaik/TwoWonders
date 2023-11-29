#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER
#include <raylib.h>
#include "../utils/types.hpp"

constexpr float BORDER = 10.f;

struct CollisionSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent>;
    using SYSTAGs = MP::TypeList<>;
    void update(EntityManager& em);
};

#endif // !INPUT_MANAGER