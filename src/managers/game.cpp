#include "game.hpp"
#include "game_engine.hpp"
//#include "../utils/memory_viewer.hpp"

void createEnemiesofType(EntityManager& em, std::vector<EnemyData>vec, uint16_t type) {
    for (const auto& data : vec)
    {
        auto& enemy{ em.newEntity() };
        em.addTag<EnemyTag>(enemy);
        auto& r = em.addComponent<RenderComponent>(enemy, RenderComponent{ .position = data.position, .scale = { 1.0f, 1.0f, 1.0f }, .color = ORANGE ,.visible = data.visible });
        auto& p = em.addComponent<PhysicsComponent>(enemy, PhysicsComponent{ .position = { r.position }, .velocity = { .2f, .0f, .0f } });
        switch (type)
        {
        case 0: em.addComponent<PatrolComponent>(enemy, PatrolComponent{ .patrol = data.route });
            break;
        case 1:
            em.addComponent<ShootPlayerComponent>(enemy, ShootPlayerComponent{
                .Xmin = data.Xmin,
                .Xmax = data.Xmax,
                .Zmin = data.Zmin,
                .Zmax = data.Zmax
                });
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 3.0f });
            break;
        case 2:
            em.addComponent<RandomShootComponent>(enemy, RandomShootComponent{
                .Xmin = data.Xmin,
                .Xmax = data.Xmax,
                .Zmin = data.Zmin,
                .Zmax = data.Zmax
                });
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 0.0f });
            break;
        default:
            break;
        }
        em.addComponent<LifeComponent>(enemy, LifeComponent{ .life = data.num_lifes });
        em.addComponent<ColliderComponent>(enemy, ColliderComponent{ p.position, r.scale, BehaviorType::ENEMY });
    }
}

void createEnemiesZelda(EntityManager& em)
{
    //Creamos Patrol Enemies
    std::vector<EnemyData> Vec_patrolData = {
       { {-9.0f, 0.f, -14.0f},vec3f{},
        {
            vec3f{-9.0f, 0.f, -14.0f},
            { -9.0f, 0.f, -10.0f },
            { 9.0f, 0.f, -10.0f },
            { 9.0f, 0.f, -14.0f },
            PatrolComponent::invalid
        },0.0f,5.0f,1,0.0f,0.0f,0.0f,0.0f,true}
    };
    createEnemiesofType(em, Vec_patrolData, 0);
    //Creamos Shoot Player Enemies
    std::vector<EnemyData> Vec_ShootPlayerData = {
        { {-46.0f, 0.0f, -20.0f},vec3f{},
         {
             vec3f{},
         },0.0f,10.0f,2,-43.0f,-46.0f,-11.0f,-20.0f,false},
          { {-45.0f, 0.0f, 4.0f},vec3f{},
         {
             vec3f{}
         },0.0f,10.0f,2,-43.0f,-46.0f,3.0f,-4.0f,false}
    };
    createEnemiesofType(em, Vec_ShootPlayerData, 1);
    //Creamos Random enemies
    std::vector<EnemyData> Vec_RandomShoot = {
        {{-33.0f, 0.0f, -12.0f},{0.2f,0.0f,0.0f},{},3.5f,0.f,1,-43.0f,-11.0f,-24.0f,-9.0f,true},
        {{-32.0f, 0.0f, -20.0f},{0.2f,0.0f,0.0f},{},2.0f,0.f,1,-43.0f,-11.0f,-24.0f,-9.0f,true},
        {{-28.0f, 0.0f, -11.0f},{0.2f,0.0f,0.0f},{},1.0f,0.f,1,-43.0f,-11.0f,-24.0f,-9.0f,true},
        {{-21.0f, 0.0f, -10.0f},{0.2f,0.0f,0.0f},{},1.0f,0.f,1,-43.0f,-11.0f,-24.0f,-9.0f,true},
        {{-39.0f, 0.0f, -18.0f},{0.2f,0.0f,0.0f},{},1.0f,0.f,1,-43.0f,-11.0f,-24.0f,-9.0f,true}
    };
    createEnemiesofType(em, Vec_RandomShoot, 2);
}

void createSword(EntityManager& em)
{
    auto& e{ em.newEntity() };

    em.addTag<ObjectTag>(e);

    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 49.0f, 0.f, 78.0f }, .scale = { 1.0f, 0.3f, 0.3f }, .color = LIGHTGRAY });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::Sword, .inmortal = true });
}

void createCoin(EntityManager& em){
    std::cout << "Creo moneda" << std::endl;
    auto& e{ em.newEntity() };

    em.addTag<ObjectTag>(e);

    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 71.0f, 0.f, 78.0f }, .scale = { 0.5f, 0.5f, 0.5f }, .color = YELLOW });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::Coin30, .inmortal = true});
}

void createShop(EntityManager& em){
    
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

    // Coin
    createCoin(em);

    // Shop
    createShop(em);

    // Enemies
    // createEnemiesZelda(em);

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
    ZoneSystem zone_system{};

    createEntities(em);

    Map map{};
    map.createMap(em);

    engine.setTargetFPS(30);

    // Nos aseguramos que los numeros aleatorios sean diferentes cada vez
    srand((unsigned int)time(NULL));

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
        zone_system.update(em, engine);

        object_system.update(em, deltaTime);
        attack_system.update(em, deltaTime);
        projectile_system.update(em, deltaTime);
        life_system.update(em, deltaTime);

        if (!render_system.update(em, engine)) {
            createEntities(em);
            map.createMap(em);
        }
    }

    engine.closeWindow();

}