#pragma once
#ifndef PHYSICS_SYSTEM
#define PHYSICS_SYSTEM
#include "../managers/entity_manager.hpp"
#include "../utils/types.hpp"

struct PhysicsSystem
{
    using SYSCMPs = MP::TypeList<PhysicsComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em)
    {
        em.forEach<SYSCMPs, SYSTAGs>([&em](Entity& e, PhysicsComponent& phy)
        {
            phy.position.x += phy.velocity.x;
            phy.position.y += phy.velocity.y;
            phy.position.z += phy.velocity.z;
            phy.direction = { phy.velocity.x, phy.velocity.y, phy.velocity.z };
        });
    }
};

#endif // !PHYSICS_SYSTEM