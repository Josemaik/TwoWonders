#include "physics_system.hpp"

void PhysicsSystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([&em](Entity&, PhysicsComponent& phy)
    {
        auto& pos = phy.position;
        auto& vel = phy.velocity;

        pos.setX(pos.x() + vel.x());
        pos.setY(pos.y() + vel.y());
        pos.setZ(pos.z() + vel.z());
        phy.direction = { vel.x(), vel.y(), vel.z() };
    });
}