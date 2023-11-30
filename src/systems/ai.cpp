#include "ai.hpp"

void AISystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([&em](Entity&, PhysicsComponent& phy,AIComponent ai)
    {
        // posicion y velocidad
        auto& pos = phy.position;
        auto& vel = phy.velocity;
        // si la pos actual es >= que el maximo patron vuelvo al principio
        if(ai.current >= ai.max_patrol)
            ai.current = 0;
        // Set del objetivo, next position
        auto const& target = ai.patrol[ai.current];
        if(target == ai.invalid){
            ai.current  = 0;
            return;
        }
        //calculo la distancia y la normalizo
        auto const distance = target - pos;
        if(distance.length() < ai.arrival_radious){
            ++ai.current;
        }
        vel = distance.normalized();
    });
}