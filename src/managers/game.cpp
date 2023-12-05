#include "game.hpp"
#include "game_engine.hpp"
//#include "../utils/memory_viewer.hpp"

void createWalls(EntityManager& em)
{
    std::vector<std::pair<vec3f, vec3f>> wallData = {
        { { 9.5f, 0.0f, 6.0f }, { 1.0f, 1.0f, 8.0f } },
        { { -9.5f, 0.0f, 6.0f }, { 1.0f, 1.0f, 8.0f } },
        { { 9.5f, 0.0f, -6.0f }, { 1.0f, 1.0f, 8.0f } },
        { { -9.5f, 0.0f, -6.0f }, { 1.0f, 1.0f, 8.0f } },
        { { -5.5f, 0.0f, -9.5f }, { 7.0f, 1.0f, 1.0f } },
        { { -5.5f, 0.0f, 9.5f }, { 7.0f, 1.0f, 1.0f } },
        { { 5.5f, 0.0f, -9.5f }, { 7.0f, 1.0f, 1.0f } },
        { { 5.5f, 0.0f, 9.5f }, { 7.0f, 1.0f, 1.0f } },
        { { 5.f, 0.0f, 5.f }, { 3.0f, 1.0f, 3.0f } },
        { { 5.f, 0.0f, -5.f }, { 3.0f, 1.0f, 3.0f } },
        { { -5.f, 0.0f, 5.f }, { 3.0f, 1.0f, 3.0f } },
        { { -5.f, 0.0f, -5.f }, { 3.0f, 1.0f, 3.0f } },
        { { 5.f, 0.0f, 0.f }, { 3.0f, 1.0f, 3.0f } },
        { { -5.f, 0.0f, 0.f }, { 3.0f, 1.0f, 3.0f } },
        { { 0.f, 0.0f, -5.f }, { 3.0f, 1.0f, 3.0f } },
        { { 0.f, 0.0f, 5.f }, { 3.0f, 1.0f, 3.0f } },

    };

    for (const auto& [pos, scl] : wallData)
    {
        auto& wall{ em.newEntity() };
        auto& wr = em.addComponent<RenderComponent>(wall, RenderComponent{ .position = pos, .scale = scl, .color = GRAY });
        auto& wp = em.addComponent<PhysicsComponent>(wall, PhysicsComponent{ .position = { wr.position }, .velocity = { .0f, .0f, .0f } });
        em.addComponent<ColliderComponent>(wall, ColliderComponent{ wp.position, wr.scale, BehaviorType::STATIC });
    }
}
struct EnemyData {
    AIComponent::AI_type aiType;
    vec3f position;
    std::array<vec3f, 10> route;
};
void createEnemies(EntityManager& em)
{
    std::vector<EnemyData> enemyData = {
    //    {  AIComponent::AI_type::PatrolEnemy,
    //      {0.0f, 0.0f, -8.0f},
    //      {
    //          vec3f{0.f, 0.f, -8.0f},
    //          { -8.5f, 0.f, -8.0f },
    //          { -8.5f, 0.f, 8.0f },
    //          { 0.f, 0.f, 8.0f },
    //          { -8.5f, 0.f, 8.0f },
    //          { -8.5f, 0.f, -8.0f },
    //          AIComponent::invalid
    //      }},
    //    {  AIComponent::AI_type::PatrolFollowEnemy,
    //      {0.0f, 0.0f, 8.0f},
    //      {
    //          vec3f{0.f, 0.f, 8.0f},
    //          { 8.5f, 0.f, 8.0f },
    //          { 8.5f, 0.f, -8.0f },
    //          { 0.f, 0.f, -8.0f },
    //          { 8.5f, 0.f, -8.0f },
    //          { 8.5f, 0.f, 8.0f },
    //          AIComponent::invalid
    //      }},
        //  { AIComponent::AI_type::ShoterEnemy,
        //  {2.8f, 0.0f, -2.8f},
        //  {
        //      vec3f{2.8f, 0.0f, -2.8f},
        //      {-2.8f, 0.0f, -2.8f},
        //       {-2.8f, 0.0f, 2.8f},
        //       {2.8f, 0.0f, 2.8f},
        //      AIComponent::invalid
        //  }}
         { AIComponent::AI_type::ShoterEnemy2,
         {2.8f, 0.0f, -2.8f},
         {
             vec3f{-2.8f, 0.0f, -2.8f},
             {2.8f, 0.0f, -2.8f},
             AIComponent::invalid
         }}
    };

    for (const auto& enemyDataItem : enemyData)
    {
        auto& enemy{ em.newEntity() };
        auto& r = em.addComponent<RenderComponent>(enemy, RenderComponent{ .position = enemyDataItem.position, .scale = { 1.0f, 1.0f, 1.0f }, .color = ORANGE });
        auto& p = em.addComponent<PhysicsComponent>(enemy, PhysicsComponent{ .position = { r.position }, .velocity = {} });
        em.addComponent<AIComponent>(enemy, AIComponent{.current_type = enemyDataItem.aiType,.patrol = enemyDataItem.route });
        em.addComponent<LifeComponent>(enemy, LifeComponent{ .life = 1 });
        em.addComponent<ColliderComponent>(enemy, ColliderComponent{ p.position, r.scale, BehaviorType::ENEMY });
        if(enemyDataItem.aiType == AIComponent::AI_type::ShoterEnemy2){
             em.addComponent<AttackComponent>(enemy, AttackComponent{.countdown = 3.5f});
        }
    }
}

void createEntities(EntityManager& em)
{
    // Player
    auto& e{ em.newEntity() };
    em.addTag<PlayerTag>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 0.0f, -0.5f, 0.0f }, .scale = { 1.0f, 1.0f, 1.0f }, .color = PINK });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .1f, .0f, .0f } });
    em.addComponent<InputComponent>(e, InputComponent{});
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 3 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::PLAYER });
    em.addComponent<AttackComponent>(e, AttackComponent{});

    // Ground
    auto& e0{ em.newEntity() };
    em.addTag<GroundTag>(e0);
    auto& r0 = em.addComponent<RenderComponent>(e0, RenderComponent{ .position = { 0.0f, -1.5f, 0.0f }, .scale = { 20.0f, 2.f, 20.0f }, .color = GREEN });
    auto& p0 = em.addComponent<PhysicsComponent>(e0, PhysicsComponent{ .position{ r0.position }, .velocity{ .0f, .0f, .0f }, .gravity = .0f });
    em.addComponent<ColliderComponent>(e0, ColliderComponent{ p0.position, r0.scale, BehaviorType::STATIC });

    // Walls
    createWalls(em);

    // // Enemy
    createEnemies(em);



    auto& li = em.getSingleton<LevelInfo>();
    li.playerID = e.getID();
}

void game()
{
    GameEngine engine{ SCREEN_WIDTH, SCREEN_HEIGHT };
    EntityManager em{};
    PhysicsSystem physics_system{};
    RenderSystem render_system{};
    InputSystem input_system{};
    CollisionSystem collision_system{};
    LifeSystem life_system{};
    AISystem   ai_sys{};
    GameTimer gtime{};
    AttackSystem attack_system{};

    createEntities(em);

    // MemoryViewer mv{ em.getCMPStorage<ColliderComponent>() };
    // MemoryViewer mv2{ em.getCMPStorage<RenderComponent>() };
    // mv2.printMemory();
    // mv.printMemory();
      // Inicializa el reloj para medir el tiempo entre frames
    while (!engine.windowShouldClose())
    {
        input_system.update(em);
        ai_sys.update(em);
        physics_system.update(em);
        collision_system.update(em);
        life_system.update(em);
        attack_system.update(em);

        render_system.update(em, engine);
    }

    engine.closeWindow();

}