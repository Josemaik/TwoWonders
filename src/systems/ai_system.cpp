#include "ai_system.hpp"
#include <cmath>
#include <random>


void perception(Entity& e,BlackBoard_t& bb, AIComponent& ai,float dt){
        // Accumulate delta time still perception time
        ai.accumulated_dt += dt;
        if( ai.accumulated_dt <= ai.perceptionTime ) return;
        //Perception time reached
        ai.accumulated_dt -= ai.perceptionTime;
        // Al pulsar la G , la ia con seek va a la posición del player
        if(bb.tactive){
            ai.tx = bb.tx;
            ai.tz = bb.tz;
            bb.tactive = false;
            // auto id {static_cast<int>(e.getID()) }; 
            // std::printf("[%d] VOY! (%.1f,%.1f)\n",id,ai.tx,ai.tz);
        }
}
// Actualizar las IA
void AISystem::update(EntityManager& em, float dt)
{
    auto& bb = em.getSingleton<BlackBoard_t>();
    em.forEach<SYSCMPs, SYSTAGs>([&, dt](Entity& e, PhysicsComponent& phy, AIComponent& ai)
    {
        //percibir el entorno
        perception(e,bb,ai,dt);
        if(ai.behaviourTree){
            ai.behaviourTree->run( {em,e,ai,phy,dt} );
            return;
        }
    });
}