#include "physics_system.hpp"
#include <printf.h>
#include <math.h>

void PhysicsSystem::update(EntityManager& em, float dt)
{
    em.forEach<SYSCMPs, SYSTAGs>([dt, &em](Entity&, PhysicsComponent& phy)
    {
        auto& pos = phy.position;
        auto& vel = phy.velocity;
        // auto& vel_l = phy.v_linear;
        // auto& vel_a = phy.v_angular;

        // Aplicar gravedad y hacer Clamp
        //if (!phy.unCheckGravity)
        vel.setY(vel.y() - phy.gravity);

        // if(e.hasTag<PlayerTag>() || e.hasTag<HitPlayerTag>()){
        if (std::abs(vel.x()) > phy.max_speed || std::abs(vel.y()) > phy.max_speed || std::abs(vel.z()) > phy.max_speed)
        {
            vel.normalize();
            vel *= phy.max_speed;
        }
        // }else{
        //         //Normalizar la velocidad
        //         vel.normalize();
        // }
        // Player únicamente tiene velocidad linear
        // if(e.hasTag<PlayerTag>() || e.hasTag<HitPlayerTag>()){
        if(phy.dragactivated){
            phy.dragactivated = false;
            // float dragFactor = 0.3f;
            // vel -= dragFactor;
            vel /= 2;
        }

        pos.setX((pos.x() + vel.x()));
        pos.setY((pos.y() + vel.y()));
        pos.setZ((pos.z() + vel.z()));

        if (vel.x() != 0 || vel.z() != 0)
            phy.orientation = std::atan2(vel.x(), vel.z());
        // }else{ //Enemigo tiene aceleracion lineal y velocidad angular
        //     phy.orientation += dt * vel_a;
        //     if(phy.orientation > 2*PI) phy.orientation -= 2*PI;
        //     if(phy.orientation < 0   ) phy.orientation += 2*PI;

        //     vel.setX(vel_l * std::cos(phy.orientation) );
        //     vel.setZ(vel_l * std::sin(phy.orientation) );
        //     // e = e0 + v0t + (1/2)at² t = 1 / 30
        //     // e = vt
        //     // a = at
        //     // e = v*t
        //     pos.setX(pos.x() + (vel.x() * dt) );
        //     pos.setY(pos.y() + vel.y());
        //     pos.setZ(pos.z() + (vel.z() * dt) );
        //     // v = at
        //     phy.v_linear += phy.a_linear * dt;
        //     phy.v_angular += phy.a_angular * dt;
        //     phy.v_linear =  std::clamp(phy.v_linear,  -phy.kMaxVLin, phy.kMaxVLin);
        //     phy.v_angular = std::clamp(phy.v_angular, -phy.kMaxAAng, phy.kMaxAAng);
        //     // drag
        // if(phy.dragactivated){
        //     auto drag { dt * std::abs(phy.v_linear) * phy.kDrag };
        //     if ( phy.v_linear > 0 ) phy.v_linear -= drag;
        //     else                    phy.v_linear += drag;
        // }

        // }
        // comprobar si están en el suelo
        if (phy.alreadyGrounded)
            phy.alreadyGrounded = false;
    });
}
