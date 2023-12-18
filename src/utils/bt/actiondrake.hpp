#pragma once
#include "node.hpp"
#include <utils/types.hpp>


struct BTActionDrake : BTNode_t{
    // BTActionPatrol() = default;

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
         //Do patrol
        //si la pos actual es >= que el maximo patron vuelvo al principio
        if (ectx.ai.current >= ectx.ai.max_patrol) {
            ectx.ai.current = 0;
        }

        // Set del objetivo, next position
        auto const& target = ectx.ai.patrol[ectx.ai.current];
        if (target == ectx.ai.invalid) {
            ectx.ai.current = 0;
            ectx.ai.nexttarget = 0;
            // return BTNodeStatus_t::fail;
        }
        //    std::cout << p.position.x() << ", " << p.position.y() << ", " << p.position.z() << "\n";
            //calculo la distancia
        auto  distance = target - ectx.phy.position;
        //std::cout << distance.x() << ", " << distance.y() << ", " << distance.z() << "\n";
        // Si la distancia es < que el radio de llegada paso a la siguiente
        if (distance.length() < ectx.ai.arrival_radius) {
            ectx.ai.current++;
            ectx.ai.arrived = true;
        }
        //Normalizo la distancia y se la asigno a la velocidad
        ectx.phy.velocity = distance.normalize() * ectx.ai.SPEED_AI;
        
         if (ectx.ai.elapsed_shoot >= ectx.ai.countdown_shoot) {
            ectx.ai.elapsed_shoot = 0;
            //shoot one time
            auto& att = ectx.em.getComponent<AttackComponent>(ectx.ent);
            att.attack(AttackType::TripleShot);
         }
        ectx.ai.dec_countdown_shoot(ectx.deltatime);
        return BTNodeStatus_t::success;
    }
};