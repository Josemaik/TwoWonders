#include "physics_system.hpp"
#include <printf.h>

void PhysicsSystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([](Entity&, PhysicsComponent& phy)
    {
        auto& pos = phy.position;
        auto& vel = phy.velocity;

        // Aplicar gravedad y hacer Clamp
        //if (!phy.unCheckGravity)
        vel.setY(vel.y() - phy.gravity);
        //vel.setY(std::clamp(vel.x(), phy.KMinVy, phy.KMaxVy));

        // Revisar que la velocidad no exceda la velocidad máxima
        if (std::abs(vel.x()) > phy.MAX_SPEED || std::abs(vel.y()) > phy.MAX_SPEED || std::abs(vel.z()) > phy.MAX_SPEED)
        {
            vel.normalize();
            vel *= phy.MAX_SPEED;
        }

        pos.setX(pos.x() + vel.x());
        pos.setY(pos.y() + vel.y());
        pos.setZ(pos.z() + vel.z());

        if (phy.alreadyGrounded)
            phy.alreadyGrounded = false;
    });
}