#include "ai_system.hpp"
#include <cmath>
#include <random>

// Actualizar las IA
void AISystem::update(EntityManager& em, float dt)
{
    em.forEach<SYSCMPs, SYSTAGs>([&, dt](Entity& e, PhysicsComponent& phy, AIComponent& ai)
    {
        if(ai.behaviourTree){
            ai.behaviourTree->run( {em,e,ai,phy,dt} );
            return;
        }
    });
}