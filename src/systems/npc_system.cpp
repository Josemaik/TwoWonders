#include "npc_system.hpp"

// Actualizar las IA
void NPCSystem::update(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, RenderComponent& ren, NPCComponent& npc)
    {
        NPCComponent* npcptr = &npc;
        if(e.hasTag<NomadTag>() && li.npcflee){
            npc.tp = true;
            phy.position = vec3d{-80.0192,5,-25.493};
            li.npcflee = false;
        }
        // if(e.hasTag<InvestigatorTag>() && li.investigador_boat){
        //     npc.goto_boat = true;
        //     npc.path_investigadorfinalIt = npc.path_investigadorfinal.begin();
        //     li.investigador_boat = false;
        // }
        if (npc.behaviourTree) {
            npc.behaviourTree->run({ em, e, nullptr, npcptr, phy, ren, nullptr, timeStep });
            return;
        }
    });
}
