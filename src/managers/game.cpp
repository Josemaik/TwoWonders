#include "game.hpp"
//#include "../utils/memory_viewer.hpp"

void Game::createShield(EntityManager& em, Entity& ent)
{
    auto& e{ em.newEntity() };
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<RenderComponent>(ent).position, .scale = { 1.0f, 1.0f, 0.5f }, .color = DARKBROWN });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::SHIELD });
    em.addComponent<ShieldComponent>(e, ShieldComponent{});
}

void Game::createEnding(EntityManager& em)
{
    auto& e{ em.newEntity() };
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = {83.0f, 1.0f, -87.0f}, .scale = {1.0f, 1.0f, 1.0f}, .color = WHITE });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position, .gravity = 0 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ENDING });
}

void Game::createEntities(EntityManager& em, Eventmanager& evm)
{
    // Player
    auto& e{ em.newEntity() };
    em.addTag<PlayerTag>(e);// -2 -12 63 -71
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { -0.0f, 0.0f, 0.0f }, .scale = { 1.0f, 1.0f, 1.0f }, .color = WHITE });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .1f, .0f, .0f } });
    em.addComponent<InputComponent>(e, InputComponent{});
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 6 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::PLAYER });

    em.addComponent<InformationComponent>(e, InformationComponent{});
    em.addComponent<TypeComponent>(e, TypeComponent{});
    // em.addComponent<EventComponent>(e);
    evm.registerListener(e, EVENT_CODE_CHANGE_ZONE);

    // Shield
    createShield(em, e);

    // Ending
    createEnding(em);

    auto& li = em.getSingleton<LevelInfo>();
    li.playerID = e.getID();
}

//inicializar bancos
void Game::createSound(EntityManager&) {
    sound_system.initBanks("assets/banks/Master.bank", "assets/banks/Master.strings.bank", "assets/banks/UI.bank", "assets/banks/Music.bank");
    //sound_system.createEventInstance();
    //sound_system.play();
}

void Game::run()
{
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
    // duration<float, std::milli> duration = t2 - t1;
    // std::cout << "el _System se ejecutó en " << duration.count() << " ms.\n";

    auto& li = em.getSingleton<LevelInfo>();

    // Inicializa una variable donde tener el tiempo entre frames
    float deltaTime{}, currentTime{};

    createSound(em);

    while (!engine.windowShouldClose())
    {
        deltaTime = engine.getFrameTime();

        switch (li.currentScreen)
        {

            // CODIGO DE LA PANTALLA DE LOGO DE EMPRESA
        case GameScreen::LOGO:
        {
            // Contador para que pasen X segundos
            currentTime += deltaTime;
            if (currentTime > 4.0f) {
                li.currentScreen = GameScreen::TITLE;
                currentTime = 0;
            }
            render_system.drawLogoKaiwa(engine);
            break;
        }

        // CODIGO DE LA PANTALLA DE TITULO
        case GameScreen::TITLE:
        {
            if (sound_system.music_started == false) {
                sound_system.playMusicMenu();
                sound_system.music_started = true;
            }
            render_system.drawLogoGame(engine, em, sound_system);
            break;
        }

        // CODIGO DE LA PANTALLA DE OPCIONES
        case GameScreen::OPTIONS:
        {
            render_system.drawOptions(engine, em, sound_system);
            break;
        }

        // CODIGO DE LA PANTALLA DE HISTORIA
        case GameScreen::STORY:
        {
            // Input del enter para empezar la partida
            if (input_system.pressEnter())
                li.currentScreen = GameScreen::GAMEPLAY;
            render_system.drawStory(engine);

            if (em.getEntities().empty())
            {
                createEntities(em, evm);
                map.reset(em, 0, iam);
            }

            break;
        }

        // CODIGO DEL GAMEPLAY
        case GameScreen::GAMEPLAY:
        {
            if (em.getEntities().empty())
            {
                li.num_zone = 0;
                zone_system.reset();
                li.notLoadSet.clear();
                createEntities(em, evm);
                map.reset(em, 0, iam);
            }

            input_system.update(em);
            // seleccionar modo de debug ( physics o AI)
            if (input_system.debugModePhysics) {
                debugExecution(em, true, false);
            }
            else if (input_system.debugModeAI) {
                debugExecution(em, false, true);
            }
            else {
                normalExecution(em, deltaTime);
            }
            break;

        }

        // case GameScreen::DEAD:
        //     /* code */
        //     break;

        // CODIGO DE LA PANTALLA FINAL
        case GameScreen::ENDING:
        {
            if (input_system.pressEnter())
                li.currentScreen = GameScreen::TITLE;

            em.destroyAll();
            render_system.unloadModels(em, engine);
            render_system.drawEnding(engine);
            zone_system.reset();
            li.notLoadSet.clear();
            break;
        }

        default:
            break;
        }
    }

    //liberar bancos
    sound_system.liberar();
    render_system.unloadModels(em, engine);

    engine.closeWindow();
}

void Game::normalExecution(EntityManager& em, float deltaTime)
{
    ai_system.update(em, deltaTime);
    physics_system.update(em, deltaTime);
    collision_system.update(em);
    zone_system.update(em, engine, iam, evm, map);
    shield_system.update(em);
    object_system.update(em, deltaTime);
    attack_system.update(em, deltaTime);
    projectile_system.update(em, deltaTime);
    life_system.update(em, deltaTime);
    sound_system.update();
    render_system.update(em, engine, false, false);
    event_system.update(evm, em);
}
void Game::debugExecution(EntityManager& em, bool debugphy, bool debugai)
{
    render_system.update(em, engine, debugphy, debugai);
}
