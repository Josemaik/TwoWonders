#include "ai.hpp"

void AISystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([&,&em](Entity&, PhysicsComponent& phy,AIComponent& ai)
    {
        //local Variables
        // auto& pos = phy.position;
        //Player detection
        if(this->isPlayerDetected(em,phy,ai)){
            std::printf("DETECTO AL PLAYER");
            //Attack
            phy.velocity = {};
            return;
        }
        //Do patrol
        //si la pos actual es >= que el maximo patron vuelvo al principio
        if(ai.current >= ai.max_patrol){
            ai.current = 0;
        }
            
        // Set del objetivo, next position
        auto const& target = ai.patrol[ai.current];
        if(target == ai.invalid){
            ai.current  = 0;
            return;
        }
        //calculo la distancia 
        auto const distance = target - phy.position;
        // Si la distancia es < que el radio de llegada paso a la siguiente
        if(distance.length() < ai.arrival_radius){
            ai.current++;
        }
        //Normalizo la distancia y se la asigno a la velocidad
        phy.velocity = distance.normalized() * SPEED_AI;
    });
}
[[nodiscard]] bool AISystem::isPlayerDetected(EntityManager&EM,PhysicsComponent const p, AIComponent const ai) const noexcept{
    auto& li = EM.getSingleton<LevelInfo>();
    auto* playerEn = EM.getEntityByID (li.playerID);
    if(not playerEn) return false;
    auto& plphy = EM.getComponent<PhysicsComponent>(*playerEn);
    auto const distance = (p.position - plphy.position).lengthSQ();
    return  distance < (ai.detect_radius * ai.detect_radius); 
}






































































































































































































































































































































