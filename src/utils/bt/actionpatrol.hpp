#pragma once
#include "node.hpp"
#include <utils/types.hpp>

constexpr float SPEED_AI = 0.2f;

struct BTActionPatrol : BTNode_t{
    BTActionPatrol(vec3f v) : where{v} {}

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        //Do patrol
        //si la pos actual es >= que el maximo patron vuelvo al principio
        if (ectx.pc.current >= ectx.pc.max_patrol) {
            ectx.pc.current = 0;
        }

        // Set del objetivo, next position
        auto const& target = ectx.pc.patrol[ectx.pc.current];
        if (target == ectx.pc.invalid) {
            ectx.pc.current = 0;
            ectx.pc.nexttarget = 0;
            // return BTNodeStatus_t::fail;
        }
        //    std::cout << p.position.x() << ", " << p.position.y() << ", " << p.position.z() << "\n";
            //calculo la distancia
        auto  distance = target - ectx.phy.position;
        //std::cout << distance.x() << ", " << distance.y() << ", " << distance.z() << "\n";
        // Si la distancia es < que el radio de llegada paso a la siguiente
        if (distance.length() < ectx.pc.arrival_radius) {
            ectx.pc.current++;
            ectx.pc.arrived = true;
        }
        //Normalizo la distancia y se la asigno a la velocidad
        ectx.phy.velocity = distance.normalize() * SPEED_AI;
        return BTNodeStatus_t::success;
    }
private:
    vec3f where {};
};