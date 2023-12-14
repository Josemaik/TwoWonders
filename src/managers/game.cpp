#include "game.hpp"
#include "game_engine.hpp"
//#include "../utils/memory_viewer.hpp"

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
    auto& e{ em.newEntity() };

    em.addTag<ObjectTag>(e);

    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 71.0f, 0.f, 78.0f }, .scale = { 0.5f, 0.5f, 0.5f }, .color = YELLOW });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::Coin30, .inmortal = true});
}

void createShop(EntityManager& em){

    // Bomba
    auto& e{ em.newEntity() };
    em.addTag<ObjectTag>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 93.0f, 0.f, 78.0f }, .scale = { 0.5f, 0.5f, 0.5f }, .color = GRAY });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::ShopItem_Bomb, .inmortal = true});

    // Vida
    auto& e2{ em.newEntity() };
    em.addTag<ObjectTag>(e2);
    auto& r2 = em.addComponent<RenderComponent>(e2, RenderComponent{ .position = { 88.0f, 0.f, 78.0f }, .scale = { 0.5f, 0.5f, 0.5f }, .color = RED });
    auto& p2 = em.addComponent<PhysicsComponent>(e2, PhysicsComponent{ .position = { r2.position }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e2, ColliderComponent{ p2.position, r2.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e2, ObjectComponent{ .type = Object_type::ShopItem_Life, .inmortal = true});

    // Vida extra
    auto& e3{ em.newEntity() };
    em.addTag<ObjectTag>(e3);
    auto& r3 = em.addComponent<RenderComponent>(e3, RenderComponent{ .position = { 98.0f, 0.f, 78.0f }, .scale = { 0.5f, 0.5f, 0.5f }, .color = MAROON });
    auto& p3 = em.addComponent<PhysicsComponent>(e3, PhysicsComponent{ .position = { r3.position }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e3, ColliderComponent{ p3.position, r3.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e3, ObjectComponent{ .type = Object_type::ShopItem_ExtraLife, .inmortal = true});
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
    Ia_man iam{};
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
        zone_system.update(em, engine,iam);

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