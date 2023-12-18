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

void createCoin(EntityManager& em)
{
    auto& e{ em.newEntity() };

    em.addTag<ObjectTag>(e);

    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 71.0f, 0.f, 78.0f }, .scale = { 0.5f, 0.5f, 0.5f }, .color = YELLOW });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::Coin30, .inmortal = true });
}

void createShop(EntityManager& em)
{

    // Bomba
    auto& e{ em.newEntity() };
    em.addTag<ObjectTag>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 93.0f, 0.f, 78.0f }, .scale = { 0.5f, 0.5f, 0.5f }, .color = GRAY });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::ShopItem_Bomb, .inmortal = true });

    // Vida
    auto& e2{ em.newEntity() };
    em.addTag<ObjectTag>(e2);
    auto& r2 = em.addComponent<RenderComponent>(e2, RenderComponent{ .position = { 88.0f, 0.f, 78.0f }, .scale = { 0.5f, 0.5f, 0.5f }, .color = RED });
    auto& p2 = em.addComponent<PhysicsComponent>(e2, PhysicsComponent{ .position = { r2.position }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e2, ColliderComponent{ p2.position, r2.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e2, ObjectComponent{ .type = Object_type::ShopItem_Life, .inmortal = true });

    // Vida extra
    auto& e3{ em.newEntity() };
    em.addTag<ObjectTag>(e3);
    auto& r3 = em.addComponent<RenderComponent>(e3, RenderComponent{ .position = { 98.0f, 0.f, 78.0f }, .scale = { 0.5f, 0.5f, 0.5f }, .color = MAROON });
    auto& p3 = em.addComponent<PhysicsComponent>(e3, PhysicsComponent{ .position = { r3.position }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e3, ColliderComponent{ p3.position, r3.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e3, ObjectComponent{ .type = Object_type::ShopItem_ExtraLife, .inmortal = true });
}

void createShield(EntityManager& em, Entity& ent)
{
    auto& e{ em.newEntity() };
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<RenderComponent>(ent).position, .color = DARKBROWN });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::SHIELD });
    em.addComponent<ShieldComponent>(e, ShieldComponent{});
}

void createEnding(EntityManager& em)
{
    auto& e{ em.newEntity() };
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = {83.0f, 1.0f, -87.0f}, .scale = {1.0f, 1.0f, 1.0f}, .color = WHITE });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ENDING });
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
    // Shield
    createShield(em, e);
    // Coin
    createCoin(em);
    // Shop
    createShop(em);

    // Ending
    createEnding(em);

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
    ShieldSystem shield_system{};
    Map map{};

    createEntities(em);
    map.createMap(em);

    engine.setTargetFPS(30);

    // Nos aseguramos que los numeros aleatorios sean diferentes cada vez
    srand((unsigned int)time(NULL));

    // MemoryViewer mv{ em.getCMPStorage<ColliderComponent>() };
    // mv.printMemory();

    // Codigo para medir el tiempo de ejecucion
    //
    // - Descomentar estas líneas y dejarlas ahí
    // using std::chrono::high_resolution_clock;
    // using std::chrono::duration_cast;
    // using std::chrono::duration;
    // using std::chrono::milliseconds;
    //
    // - Colocar antes de donde se quiere medir el tiempo
    // auto t1 = high_resolution_clock::now();
    //
    // - Colocar despues de donde se quiere medir el tiempo
    // auto t2 = high_resolution_clock::now();
    // auto duration = duration_cast<milliseconds>(t2 - t1);
    // std::cout << "el _System se ejecutó en " << duration.count() << " ms.\n";
    auto& li = em.getSingleton<LevelInfo>();

    // Inicializa una variable donde tener el tiempo entre frames
    float deltaTime{}, currentTime{};

    while (!engine.windowShouldClose())
    {
        deltaTime = engine.getFrameTime();

        switch (li.currentScreen)
        {

        // CODIGO DE LA PANTALLA DE LOGO DE EMPRESA
        case GameScreen::LOGO:
            // Contador para que pasen X segundos
            currentTime += deltaTime;
            if(currentTime > 3.0f){
                li.currentScreen = GameScreen::TITLE;
                currentTime = 0;
            }
            render_system.drawLogoKaiwa(engine);
            break;
        
        // CODIGO DE LA PANTALLA DE TITULO
        case GameScreen::TITLE:
            // Input del enter para empezar la partida
            if(input_system.pressEnter())
                li.currentScreen = GameScreen::GAMEPLAY;
            render_system.drawLogoGame(engine);
            break;

        // CODIGO DEL GAMEPLAY
        case GameScreen::GAMEPLAY:
            if(em.getEntities().empty()) {
                createEntities(em);
                map.createMap(em);
            }

            input_system.update(em);
            ai_sys.update(em, deltaTime);
            physics_system.update(em);
            collision_system.update(em);
            zone_system.update(em, engine, iam);

            shield_system.update(em);
            object_system.update(em, deltaTime);
            attack_system.update(em, deltaTime);
            projectile_system.update(em, deltaTime);
            life_system.update(em, deltaTime);

            render_system.update(em, engine);
            break;

        // case GameScreen::DEAD:
        //     /* code */
        //     break;

        // CODIGO DE LA PANTALLA FINAL
        case GameScreen::ENDING:
            if(input_system.pressEnter())
                li.currentScreen = GameScreen::TITLE;
            em.destroyAll();
            render_system.drawEnding(engine);
            break;

        default:
            break;
        }
    }

    engine.closeWindow();

}