#include "game.hpp"
#include "game_engine.hpp"
//#include "../utils/memory_viewer.hpp"

void game()
{
    GameEngine engine{ SCREEN_WIDTH, SCREEN_HEIGHT };
    EntityManager em{};
    PhysicsSystem physics_system{};
    RenderSystem render_system{};
    InputSystem input_system{};
    CollisionSystem collision_system{};
    LifeSystem life_system{};

    // Player
    auto& e{ em.newEntity() };
    em.addTag<PlayerTag>(e);
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { 0.0f, 0.0f, 0.0f }, .velocity = { .1f, .0f, .0f } });
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 0.0f, 0.0f, 0.0f }, .scale = { 1.0f, 1.0f, 1.0f }, .color = PINK });
    em.addComponent<InputComponent>(e, InputComponent{});
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 3 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale });

    // Wall
    auto& e2{ em.newEntity() };
    em.addComponent<RenderComponent>(e2, RenderComponent{ .position = { 0.0f, 0.0f, 0.0f }, .scale = { 2.0f, 2.0f, 8.0f }, .color = GRAY });
    em.addComponent<PhysicsComponent>(e2, PhysicsComponent{ .position = { 4.0f, 0.0f, 2.0f }, .velocity = { .0f, .0f, .0f } });

    // Enemy
    auto& e3{ em.newEntity() };
    em.addTag<EnemyTag>(e3);
    auto& r3 = em.addComponent<RenderComponent>(e3, RenderComponent{ .position = { 2.0f, 0.0f, 02.0f }, .scale = { 2.0f, 2.0f, 4.0f }, .color = YELLOW });
    auto& p3 = em.addComponent<PhysicsComponent>(e3, PhysicsComponent{ .position = { 2.0f, 0.0f, 02.0f }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<LifeComponent>(e3, LifeComponent{ .life = 1 });
    em.addComponent<ColliderComponent>(e3, ColliderComponent{ p3.position, r3.scale });

    // MemoryViewer mv{ em.getCMPStorage<ColliderComponent>() };
    // MemoryViewer mv2{ em.getCMPStorage<RenderComponent>() };
    // mv2.printMemory();
    // mv.printMemory();

    while (!engine.windowShouldClose())
    {
        input_system.update(em);
        physics_system.update(em);
        collision_system.update(em);
        life_system.update(em);
        render_system.update(em, engine);
    }

    engine.closeWindow();

}