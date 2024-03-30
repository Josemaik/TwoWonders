#pragma once
#ifndef AI_SYSTEM
#define AI_SYSTEM
#include "../utils/types.hpp"
#include "../utils/vec3D.hpp"
#include "../utils/bt/behaviourtree.hpp"
#include <iostream>

struct AISystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent, AIComponent, LifeComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em);

private:
    void perception(BlackBoard_t& bb, AIComponent& ai);
};

#endif // !AI_SYSTEM
