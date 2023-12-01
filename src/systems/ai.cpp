#include "ai.hpp"
#include <cmath>

void AISystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([&em](Entity&, PhysicsComponent& phy, AIComponent& ai)
    {
        auto& pos = phy.position;
        //si la pos actual es >= que el maximo patron vuelvo al principio
        if (ai.current >= ai.max_patrol) {
            ai.current = 0;
        }

        // Set del objetivo, next position
        auto const& target = ai.patrol[ai.current];
        if (target == ai.invalid) {
            ai.current = 0;
            return;
        }
        //calculo la distancia 
        auto const distance = target - pos;
        // Si la distancia es < que el radio de llegada paso a la siguiente
        if (distance.length() < ai.arrival_radius) {
            ai.current++;
        }
        //Normalizo la distancia y se la asigno a la velocidad
        phy.velocity = distance.normalized();
    });
}