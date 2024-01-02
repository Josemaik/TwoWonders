#include "physics_system.hpp"
#include <printf.h>
#include <math.h>

void PhysicsSystem::update(EntityManager& em,float dt)
{
    em.forEach<SYSCMPs, SYSTAGs>([dt,&em](Entity& , PhysicsComponent& phy)
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
        // if(e.hasTag<PlayerTag>()){
            phy.orientation += dt * vel_a;
            if(phy.orientation > 2*PI) phy.orientation -= 2*PI;
            if(phy.orientation < 0   ) phy.orientation += 2*PI;
            
            vel.setX(vel_l * std::cos(phy.orientation) );
            vel.setZ(vel_l * std::sin(phy.orientation) );
            // e = e0 + v0t + (1/2)at² t = 1 / 30
            // e = vt
            // a = at


            // e = v*t
            pos.setX(pos.x() + (vel.x() * dt) );
            pos.setY(pos.y() + vel.y());
            pos.setZ(pos.z() + (vel.z() * dt) );
            // v = at
            phy.v_linear += phy.a_linear * dt;
            phy.v_angular += phy.a_angular * dt;
            phy.v_linear =  std::clamp(phy.v_linear,  -phy.kMaxVLin, phy.kMaxVLin);
            phy.v_angular = std::clamp(phy.v_angular, -phy.kMaxAAng, phy.kMaxAAng);
        // }
        // }else{
        //     pos.setX(pos.x() + vel.x());
        //     pos.setY(pos.y() + vel.y());
        //     pos.setZ(pos.z() + vel.z());
        // }
        // if(e.hasTag<EnemyTag>()){
        //     auto& en = em.getComponent<PhysicsComponent>(e);
        //     std::cout << en.position.x() << en.position.z();
        // }
        if (phy.alreadyGrounded)
            phy.alreadyGrounded = false;
    });
}