#pragma once
#ifndef AI_SYSTEM
#define AI_SYSTEM
#include "../utils/types.hpp"
#include "../utils/vec3D.hpp"
#include "../utils/bt/behaviourtree.hpp"
#include <iostream>

// constexpr float SPEED_AI = 0.2f;

struct AISystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, AIComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, float dt);
};

#endif // !AI_SYSTEM