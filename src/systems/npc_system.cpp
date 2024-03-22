#include "npc_system.hpp"

// Actualizar las IA
void NPCSystem::update(EntityManager& em, float dt)
{
    em.forEach<SYSCMPs, SYSTAGs>([&,dt](Entity& e,PhysicsComponent& phy,RenderComponent& ren,NPCComponent &npc)
    {   
        NPCComponent* npcptr = &npc;
        if (npc.behaviourTree) {
            npc.behaviourTree->run({ em,e,nullptr,npcptr,phy,ren,nullptr,dt });
            return;
        }
    });
}
