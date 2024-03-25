#pragma once
#ifndef NPC_SYSTEM
#define NPC_SYSTEM

#include "../utils/types.hpp"
#include "../utils/bt/behaviourtree.hpp"

struct NPCSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent, NPCComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, double dt);
};

#endif // !NPC_SYSTEM
