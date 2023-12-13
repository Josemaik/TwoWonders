#include "game.hpp"
#include "game_engine.hpp"
//#include "../utils/memory_viewer.hpp"

struct EnemyData {
    AIComponent::AI_type aiType;
    vec3f position;
    vec3f velocity;
    std::array<vec3f, 10> route;
    float stop{};
    float detect_radius;
    int num_lifes;
    float Xmin{}, Xmax{}, Zmin{}, Zmax{};
    bool visible{};
};

void createEnemiesZelda(EntityManager& em)
{
    std::vector<EnemyData> enemyData = {
       {  AIComponent::AI_type::PatrolEnemy,
         {-9.0f, 0.f, -14.0f},vec3f{},
         {
             vec3f{-9.0f, 0.f, -14.0f},
             { -9.0f, 0.f, -10.0f },
             { 9.0f, 0.f, -10.0f },
             { 9.0f, 0.f, -14.0f },
             AIComponent::invalid
         },0.0f,5.0f,1,0.0f,0.0f,0.0f,0.0f,true},
        //  {  AIComponent::AI_type::PatrolFollowEnemy,
        //  {-2.0f, 0.0f, 10.0f},vec3f{},
        //  {
        //      vec3f{0.0f, 0.f, 13.0f},
        //      { 2.0f, 0.f, 10.0f },
        //      { -2.0f, 0.0f, 10.0f },
        //      AIComponent::invalid
        //  },0.0f,5.0f,1,0.0f,0.0f,0.0f,0.0f,true},
        //  {  AIComponent::AI_type::PatrolFollowEnemy,
        //  {-2.0f, 0.f, -13.0f},vec3f{},
        //  {
        //      vec3f{-1.0f, 0.f, -9.0f},
        //      { 2.0f, 0.f, -12.0f },
        //      {-2.0f, 0.f, -13.0f },
        //      AIComponent::invalid
        //  },0.0f,5.0f,1,0.0f,0.0f,0.0f,0.0f,true},
         { AIComponent::AI_type::ShoterEnemy2,
         {-46.0f, 0.0f, -20.0f},vec3f{},
         {
             vec3f{},
         },0.0f,10.0f,2,-43.0f,-46.0f,-11.0f,-20.0f,false},
          { AIComponent::AI_type::ShoterEnemy2,
         {-45.0f, 0.0f, 4.0f},vec3f{},
         {
             vec3f{}
         },0.0f,10.0f,2,-43.0f,-46.0f,3.0f,-4.0f,false},
        {AIComponent::AI_type::RandomEnemy,
        {-33.0f, 0.0f, -12.0f},{0.2f,0.0f,0.0f},{},3.5f,0.f,1,-36.0f,-28.0f,-21.0f,-10.0f,true},
        {AIComponent::AI_type::RandomEnemy,
        {-32.0f, 0.0f, -20.0f},{0.2f,0.0f,0.0f},{},2.0f,0.f,1,-36.0f,-28.0f,-21.0f,-10.0f,true},
        {AIComponent::AI_type::RandomEnemy,
        {-28.0f, 0.0f, -11.0f},{0.2f,0.0f,0.0f},{},1.0f,0.f,1,-36.0f,-28.0f,-21.0f,-10.0f,true}
    };
    //POner por parametro tiempo de culldown para disparar
    for (const auto& data : enemyData)
    {
        auto& enemy{ em.newEntity() };
        em.addTag<EnemyTag>(enemy);
        auto& r = em.addComponent<RenderComponent>(enemy, RenderComponent{ .position = data.position, .scale = { 1.0f, 1.0f, 1.0f }, .color = ORANGE ,.visible = data.visible });
        auto& p = em.addComponent<PhysicsComponent>(enemy, PhysicsComponent{ .position = { r.position }, .velocity = { .2f, .0f, .0f } });
        em.addComponent<AIComponent>(enemy, AIComponent{ .current_type = data.aiType,
        .patrol = data.route,
        .detect_radius = data.detect_radius,
        .elapsed_stop = data.stop,
        .Xmin = data.Xmin,
        .Xmax = data.Xmax,
        .Zmin = data.Zmin,
        .Zmax = data.Zmax
            });
        em.addComponent<LifeComponent>(enemy, LifeComponent{ .life = data.num_lifes });
        em.addComponent<ColliderComponent>(enemy, ColliderComponent{ p.position, r.scale, BehaviorType::ENEMY });
        if (data.aiType == AIComponent::AI_type::ShoterEnemy2) {
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 3.0f });
        }
        if (data.aiType == AIComponent::AI_type::RandomEnemy) {
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 0.0f });
        }
    }
}

void createSword(EntityManager& em)
{
    auto& e{ em.newEntity() };

    em.addTag<ObjectTag>(e);

    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { -6.0f, 0.f, -5.0f }, .scale = { 1.0f, 0.3f, 0.3f }, .color = LIGHTGRAY });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .1f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::Sword });
}

void createEntities(EntityManager& em)
{
    // Player
    auto& e{ em.newEntity() };
    em.addTag<PlayerTag>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 0.0f, 0.f, 0.0f }, .scale = { 1.0f, 1.0f, 1.0f }, .color = PINK });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .1f, .0f, .0f } });
    em.addComponent<InputComponent>(e, InputComponent{});
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 6 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::PLAYER });
    em.addComponent<InformationComponent>(e, InformationComponent{});

    // Sword
    createSword(em);

    // Enemies
    createEnemiesZelda(em);

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
    // GameTimer gtime{};
    AttackSystem attack_system{};
    ProjectileSystem projectile_system{};
    ObjectSystem object_system{};

    createEntities(em);

    Map map{};
    map.createMap(em);

    engine.setTargetFPS(30);

    // MemoryViewer mv{ em.getCMPStorage<ColliderComponent>() };
    // mv.printMemory();

    // Inicializa una variabloe donde tener el tiempo entre frames
    float deltaTime;
    while (!engine.windowShouldClose())
    {
        deltaTime = engine.getFrameTime();

        input_system.update(em);
        ai_sys.update(em, deltaTime);
        physics_system.update(em);
        collision_system.update(em);

        object_system.update(em, deltaTime);
        attack_system.update(em, deltaTime);
        projectile_system.update(em, deltaTime);
        life_system.update(em, deltaTime);

        if (!render_system.update(em, engine)){
            map.createMap(em);
            createEntities(em);
        }
    }

    engine.closeWindow();

}