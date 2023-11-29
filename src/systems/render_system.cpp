#include "render_system.hpp"

void RenderSystem::update(EntityManager& em, ENGI::GameEngine& engine)
{
    em.forEach<SYSCMPs, SYSTAGs>([&em](Entity&, PhysicsComponent& phy, RenderComponent& ren)
    {
        ren.setPosition(phy.position);
    });

    engine.beginFrame();
    engine.drawAll(em);
    engine.endFrame();
}