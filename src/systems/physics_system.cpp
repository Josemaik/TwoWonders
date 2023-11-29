#include "physics_system.hpp"

void PhysicsSystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([&em](Entity&, PhysicsComponent& phy)
    {
        phy.position.x += phy.velocity.x;
        phy.position.y += phy.velocity.y;
        phy.position.z += phy.velocity.z;
        phy.direction = { phy.velocity.x, phy.velocity.y, phy.velocity.z };
    });
}