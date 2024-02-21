#include "game.hpp"
// #include "../utils/memory_viewer.hpp"
// #include <chrono>

void Game::createShield(EntityManager& em, Entity& ent)
{
    auto& e{ em.newEntity() };
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<RenderComponent>(ent).position, .scale = { 1.0f, 1.0f, 0.5f }, .color = DARKBROWN });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position, .scale = r.scale });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::SHIELD });
    em.addComponent<ShieldComponent>(e, ShieldComponent{ .createdby = EntitieswithShield::Player });
}

void Game::createEnding(EntityManager& em)
{
    auto& e{ em.newEntity() };
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = {83.0f, 1.0f, -87.0f}, .scale = {1.0f, 1.0f, 1.0f}, .color = WHITE });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position, .scale = r.scale, .gravity = 0 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ENDING });
}

void Game::createEntities(EntityManager& em)
{
    auto& plfi = em.getSingleton<PlayerInfo>();
    if (plfi.spawnPoint == vec3d::zero())
        plfi.spawnPoint = { -33.0, 5.5, 30.9 };
    //-9.0, 4.0, -50.0
    //26.0, 4.0, -65.0
    //-32.0   4.0  -107.0

// Player
    auto& e{ em.newEntity() };
    em.addTag<PlayerTag>(e);// -2 -12 63 -71
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = plfi.spawnPoint, .scale = { 2.0, 4.0, 2.0 }, .color = WHITE });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position, .scale = r.scale, });

    auto& lis = em.addComponent<ListenerComponent>(e, ListenerComponent{});
    em.addComponent<InputComponent>(e, InputComponent{});
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 6 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::PLAYER });

    // Listeners de eventos para el jugador
    lis.addCode(EventCodes::SpawnDungeonKey);
    lis.addCode(EventCodes::OpenChest);
    lis.addCode(EventCodes::SetSpawn);
    lis.addCode(EventCodes::OpenDoor);

    // Shield
    // createShield(em, e);

    // Ending
    // createEnding(em);

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

    Shader shader = LoadShader(TextFormat("assets/shaders/lighting.vs", 330),
        TextFormat("assets/shaders/lighting.fs", 330));

    // Get some required shader locations
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    // Ambient light level (some basic lighting)
    int ambientLoc = GetShaderLocation(shader, "ambient");
    float ambientValue[4] = { 3.1f, 3.1f, 3.1f, 20.0f };
    SetShaderValue(shader, ambientLoc, ambientValue, SHADER_UNIFORM_VEC4);

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

    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();
    auto& txti = em.getSingleton<TextInfo>();

    // Inicializa una variable donde tener el tiempo entre frames
    float deltaTime{}, currentTime{};

    createSound(em);
    li.sound_system = &sound_system;
    while (!li.gameShouldEnd)
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

        // CODIGO DE LA PANTALLA DE PAUSA
        case GameScreen::PAUSE:
        {
            render_system.drawPauseMenu(engine, em, sound_system);
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
                createEntities(em);
                map.reset(em, 0, iam);
            }

            break;
        }

        // CODIGO DEL GAMEPLAY
        case GameScreen::GAMEPLAY:
        {
            if (em.getEntities().empty() || li.resetGame)
                resetGame(em, engine, render_system);

            input_system.update(em);

            // seleccionar modo de debug ( physics o AI)
            if (!li.resetGame && !(inpi.debugPhy || inpi.debugAI1 || inpi.pause || inpi.inventory || txti.hasText()))
            {
                ai_system.update(em, deltaTime);
                physics_system.update(em, deltaTime);
                collision_system.update(em);
                zone_system.update(em, engine, iam, evm, map, deltaTime);
                lock_system.update(em);
                shield_system.update(em);
                object_system.update(em, deltaTime);
                attack_system.update(em, deltaTime);
                projectile_system.update(em, deltaTime);
                life_system.update(em, object_system, deltaTime);
                sound_system.update();
                camera_system.update(em, engine, deltaTime);
                event_system.update(em, evm, iam, map, object_system);

                if (!li.dead_entities.empty())
                {
                    em.destroyEntities(li.dead_entities);
                    li.dead_entities.clear();
                }

                render_system.update(em, engine, deltaTime, shader);
            }
            else if ((!li.resetGame) && (inpi.debugPhy || inpi.debugAI1 || inpi.pause || inpi.inventory || txti.hasText()))
                render_system.update(em, engine, deltaTime, shader);

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

            render_system.drawEnding(engine);
            resetGame(em, engine, render_system);
            break;
        }

        default:
            break;
        }

        if (engine.windowShouldClose())
            li.gameShouldEnd = true;
    }

    //liberar bancos
    sound_system.clear();
    render_system.unloadModels(em, engine);

    engine.closeWindow();
    UnloadShader(shader);
}

void Game::resetGame(EntityManager& em, GameEngine& engine, RenderSystem& rs)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& bb = em.getSingleton<BlackBoard_t>();

    em.destroyAll();
    bb.subditosData.clear();
    rs.unloadModels(em, engine);
    li.reset();
    // plfi.reset();
    lock_system.reset();
    createEntities(em);
    map.reset(em, 0, iam);
    li.sound_system = &sound_system;
}

