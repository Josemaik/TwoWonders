#pragma once 
#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM
#include "../managers/entity_manager.hpp"
#include "../managers/game_engine.hpp"

struct RenderSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, ENGI::GameEngine& engine)
    {
        em.forEach<SYSCMPs, SYSTAGs>([&em](Entity&, PhysicsComponent& phy, RenderComponent& ren)
        {
            ren.setPosition(phy.position);
        });

        engine.beginFrame();
        engine.drawAll(em);
        engine.endFrame();
    }
};

#endif // !RENDER_SYSTEM