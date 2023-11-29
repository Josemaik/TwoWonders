#include "game.hpp"
#include "game_engine.hpp"

void game()
{
    GameEngine engine{ SCREEN_WIDTH, SCREEN_HEIGHT };
    EntityManager em{};
    PhysicsSystem physics_system{};
    RenderSystem render_system{};
    InputSystem input_system{};
    CollisionSystem collision_system{};

    auto& e{ em.newEntity() };
    em.addTag<PlayerTag>(e);
    em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { 0.0f, 0.0f, 0.0f }, .velocity = { .1f, .0f, .0f } });
    em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 0.0f, 0.0f, 0.0f }, .scale = { 1.0f, 1.0f, 1.0f }, .color = PINK });
    em.addComponent<InputComponent>(e, InputComponent{});

    auto& e2{ em.newEntity() };
    em.addComponent<RenderComponent>(e2, RenderComponent{ .position = { 2.0f, 0.0f, 02.0f }, .scale = { 2.0f, 2.0f, 4.0f }, .color = YELLOW });
    em.addComponent<PhysicsComponent>(e2, PhysicsComponent{ .position = { 2.0f, 0.0f, 02.0f }, .velocity = { .0f, .0f, .0f } });

    // MemoryViewer mv{ em.getCMPStorage<PhysicsComponent>() };
    // MemoryViewer mv2{ em.getCMPStorage<RenderComponent>() };
    // mv2.printMemory();
    // mv.printMemory();
    // Cubo c{};
    // e.render.setAll({ c.position.x, c.position.y, c.position.z }, { c.scale.x, c.scale.y, c.scale.z }, c.color);

    while (engine.run())
    {
        input_system.update(em);
        physics_system.update(em);
        collision_system.update(em);
        render_system.update(em, engine);
    }

    engine.endWindow();

}