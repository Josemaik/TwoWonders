#include "ai_system.hpp"
#include <cmath>
#include <random>


void perception(BlackBoard_t& bb, AIComponent& ai,float dt){
        // Accumulate delta time still perception time
        ai.accumulated_dt += dt;
        if( ai.accumulated_dt <= ai.perceptionTime ) return;
        //Perception time reached
        ai.accumulated_dt -= ai.perceptionTime;
        // Al pulsar la G , la ia con seek va a la posición del player
        if(bb.tactive){
            ai.tx = bb.tx;
            ai.tz = bb.tz;
            ai.tactive = true;
            ai.teid = bb.teid;
            ai.behaviour = bb.behaviour;
            bb.tactive = false;
            ai.pathIt = bb.path.begin();
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
        perception(bb,ai,dt);
        if(ai.behaviourTree){
            ai.behaviourTree->clearNodes();
            switch(ai.behaviour){
                case SB::Arrive :ai.behaviourTree->createNode<BTAction_Arrive>();
                std::cout << "arrive\n";
                break;
                case SB::Seek :ai.behaviourTree->createNode<BTAction_Seek>();
                std::cout << "seek\n";
                break;
                case SB::Flee :ai.behaviourTree->createNode<BTAction_Flee>();
                std::cout << "flee\n";
                break;
                case SB::Pursue :ai.behaviourTree->createNode<BTAction_Pursue>();
                std::cout << "pursue\n";
                break;
                case SB::Avoid :ai.behaviourTree->createNode<BTAction_Avoid>();
                std::cout << "avoid\n";
                break;
                case SB::followPath :ai.behaviourTree->createNode<BTAction_Patrol>();
                std::cout << "followpath \n";
                break;
                default: ai.behaviourTree->createNode<BTAction_Arrive>();
                std::cout << "defecto \n";
                break;
            }
            ai.behaviourTree->run( {em,e,ai,phy,dt} );
            return;
        }
    });
}
