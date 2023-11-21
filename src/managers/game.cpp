#include "game.hpp"

void game()
{
    GameEngine engine{ SCREEN_WIDTH, SCREEN_HEIGHT };
    EntityManager<PhysicsComponent, RenderComponent, Entity> em{};
    PhysicsSystem physics_system{};
    RenderSystem render_system{};
    InputSystem input_system{};
    CollisionSystem collision_system{};

    auto& e{ em.newEntity() };

    Cubo c{};
    e.render.setAll({ c.position.x, c.position.y, c.position.z }, { c.scale.x, c.scale.y, c.scale.z }, c.color);

    while (engine.run())
    {
        input_system.update(em);
        physics_system.update(em);
        collision_system.update(em);
        render_system.update(em, engine);
    }

    engine.endWindow();

}