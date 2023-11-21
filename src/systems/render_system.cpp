#include "render_system.hpp"

void RenderSystem::update(EntityManager<PhysicsComponent, RenderComponent, Entity>& em, GameEngine& engine)
{
    em.forAll([](Entity& e)
    {
        auto& phy{ e.physics };
        std::cout << " aaaaa " << phy.position.x << std::endl;
        e.render.setPosition(phy.position);
    });

    engine.beginFrame();
    engine.drawAll(em);
    engine.endFrame();
}