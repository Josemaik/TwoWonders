#include "npc_system.hpp"

// Actualizar las IA
void NPCSystem::update(EntityManager& em)
{
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, RenderComponent& ren, NPCComponent& npc)
    {
        NPCComponent* npcptr = &npc;
        if (npc.behaviourTree) {
            npc.behaviourTree->run({ em, e, nullptr, npcptr, phy, ren, nullptr, timeStep });
            return;
        }
    });
}
