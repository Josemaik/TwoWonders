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

        // Revisar que la velocidad no exceda la velocidad máxima
        if (std::abs(vel.x()) > MAX_SPEED || std::abs(vel.y()) > MAX_SPEED || std::abs(vel.z()) > MAX_SPEED)
        {
            vel.normalize();
            vel *= MAX_SPEED;
        }

        // Actualizar la posición
        pos.setX(pos.x() + vel.x());
        pos.setY(pos.y() + vel.y());
        pos.setZ(pos.z() + vel.z());

    });
}