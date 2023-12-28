#include "physics_system.hpp"
#include <printf.h>
#include <math.h>

void PhysicsSystem::update(EntityManager& em,float dt)
{
    em.forEach<SYSCMPs, SYSTAGs>([dt](Entity& e, PhysicsComponent& phy)
    {
        auto& pos = phy.position;
        auto& vel = phy.velocity;
        auto& vel_l = phy.v_linear;
        auto& vel_a = phy.v_angular;

        // Aplicar gravedad y hacer Clamp
        //if (!phy.unCheckGravity)
        vel.setY(vel.y() - phy.gravity);
        //vel.setY(std::clamp(vel.x(), phy.KMinVy, phy.KMaxVy));

        //Revisar que la velocidad no exceda la velocidad máxima
        if (std::abs(vel.x()) > phy.MAX_SPEED || std::abs(vel.y()) > phy.MAX_SPEED || std::abs(vel.z()) > phy.MAX_SPEED)
        {
            vel.normalize();
            vel *= phy.MAX_SPEED;
        }
        if(e.hasTag<PlayerTag>()){
            phy.orientation += dt * vel_a;
            if(phy.orientation > 2*PI) phy.orientation -= 2*PI;
            if(phy.orientation < 0   ) phy.orientation += 2*PI;

            vel.setX(dt * vel_l * std::cos(phy.orientation) );
            vel.setZ(dt * vel_l * std::sin(phy.orientation) );

            pos.setX(pos.x() + (vel.x() * dt) );
            pos.setY(pos.y() + vel.y());
            pos.setZ(pos.z() + (vel.z() * dt) );
        }else{
            pos.setX(pos.x() + vel.x());
            pos.setY(pos.y() + vel.y());
            pos.setZ(pos.z() + vel.z());
        }
        

        if (phy.alreadyGrounded)
            phy.alreadyGrounded = false;
    });
}