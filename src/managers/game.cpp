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

    // Player
    auto& e{ em.newEntity() };
    em.addTag<PlayerTag>(e);
    em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { 0.0f, 0.0f, 0.0f }, .velocity = { .1f, .0f, .0f } });
    em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 0.0f, 0.0f, 0.0f }, .scale = { 1.0f, 1.0f, 1.0f }, .color = PINK });
    em.addComponent<InputComponent>(e, InputComponent{});
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 3 });

    // Wall
    auto& e2{ em.newEntity() };
    em.addComponent<RenderComponent>(e2, RenderComponent{ .position = { 0.0f, 0.0f, 0.0f }, .scale = { 2.0f, 2.0f, 8.0f }, .color = GRAY });
    em.addComponent<PhysicsComponent>(e2, PhysicsComponent{ .position = { 4.0f, 0.0f, 2.0f }, .velocity = { .0f, .0f, .0f } });

    // Enemy
    auto& e3{ em.newEntity() };
    em.addTag<EnemyTag>(e3);
    em.addComponent<RenderComponent>(e3, RenderComponent{ .position = { 0.0f, 0.0f, 0.0f }, .scale = { 2.0f, 2.0f, 2.0f }, .color = RED });
    em.addComponent<PhysicsComponent>(e3, PhysicsComponent{ .position = { 0.0f, 0.0f, -6.0f }, .velocity = { .0f, .0f, .0f } });

    // MemoryViewer mv{ em.getCMPStorage<PhysicsComponent>() };
    // MemoryViewer mv2{ em.getCMPStorage<RenderComponent>() };
    // mv2.printMemory();
    // mv.printMemory();
    // Cubo c{};
    // e.render.setAll({ c.position.x, c.position.y, c.position.z }, { c.scale.x, c.scale.y, c.scale.z }, c.color);

    while (!engine.windowShouldClose())
    {
        input_system.update(em);
        physics_system.update(em);
        collision_system.update(em);
        render_system.update(em, engine);
    }

    engine.closeWindow();

}