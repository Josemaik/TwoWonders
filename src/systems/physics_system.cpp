#include "physics_system.hpp"

void PhysicsSystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([](Entity&, PhysicsComponent& phy)
    {
        // Inmovilizar cuando recibe daño
        if (phy.immobilizeTimer > 0.0f)
        {
            phy.immobilizeTimer -= 1;

            if (phy.immobilizeTimer <= 0.0f)
            {
                phy.invincibilityTimer = 30.0f;
            }
            return;
        }
        else if (phy.invincibilityTimer > 0.0f)
        {
            phy.invincibilityTimer -= 1;
        }

        auto& pos = phy.position;
        auto& vel = phy.velocity;

        pos.setX(pos.x() + vel.x());
        pos.setY(pos.y() + vel.y());
        pos.setZ(pos.z() + vel.z());



    });
}