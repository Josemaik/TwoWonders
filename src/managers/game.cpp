#include "game.hpp"
// #include "../utils/memory_viewer.hpp"
// #include <chrono>

void Game::createShield(Entity& ent)
{
    auto& e{ em.newEntity() };
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<RenderComponent>(ent).position, .scale = { 1.0f, 1.0f, 0.5f }, .color = DARKBROWN });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position, .scale = r.scale });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::SHIELD });
    em.addComponent<ShieldComponent>(e, ShieldComponent{ .createdby = EntitieswithShield::Player });
}

void Game::createEnding()
{
    auto& e{ em.newEntity() };
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = {83.0f, 1.0f, -87.0f}, .scale = {1.0f, 1.0f, 1.0f}, .color = WHITE });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position, .scale = r.scale, .gravity = 0 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ENDING });
}

void Game::createEntities()
{
    auto& plfi = em.getSingleton<PlayerInfo>();
    if (plfi.spawnPoint == vec3d::zero())
        plfi.spawnPoint = { 35.0, 22.0, -23.0 };

    // 33.0, 4.0, -25.9 - Posición Incial
    // 32.0, 4.0, 43.0 - Primer cofre
    // 32.0, 4.0, 130.0 - Segundo cofre
    // -72.0, 4.0, 72.9 - Cofre con llave
    // -116.0, 4.0, 111.0 - Apisonadora
    // -125, 4.0, 138.68 - `pos chunck 3
    // 35.0, 22.0, -23.0 - Posición Incial lvl1
    // -68.0, 4.0, -22.0 - Primera rampa lvl1
    // -6.0, 4.0, 94.0 - Campamento lvl1
    // -126.0, 4.0, 152.0 - Segundo altar lvl1
    // 30.0, 13.0, 213.0 - Segundo campamento lvl1
    // -113.0, 4.0, 236.0 - Final lvl1

    // Player
    auto& e{ em.newEntity() };
    em.addTag<PlayerTag>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = plfi.spawnPoint, .scale = { 2.0, 4.0, 2.0 }, .color = WHITE });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position, .scale = r.scale });

    auto& lis = em.addComponent<ListenerComponent>(e);
    em.addComponent<InputComponent>(e);
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 7 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::PLAYER });
    // em.addComponent<AttackComponent>(e);

    // Listeners de eventos para el jugador
    lis.addCode(EventCodes::SpawnDungeonKey);
    lis.addCode(EventCodes::OpenChest);
    lis.addCode(EventCodes::SetSpawn);
    lis.addCode(EventCodes::OpenDoor);

    // Código de añadir un hechizo al jugador
    // Spell spell{ "Fireball", "Shoots a fireball", Spells::WaterBomb, 20.0, 2 };
    // plfi.addSpell(spell);

    // Código de añadir un objeto poción al inventario
    // Potion pot{ "Potion", "Heals 2 life points", PotionType::Health, 2.0 };
    // plfi.addItem(std::make_unique<Potion>(pot));

    // Shield
    // createShield(em, e);

    // Ending
    // createEnding(em);
    auto& li = em.getSingleton<LevelInfo>();
    li.playerID = e.getID();
}

//inicializar bancos
void Game::createSound() {
    sound_system.initBanks("assets/banks/Master.bank", "assets/banks/Master.strings.bank", "assets/banks/UI.bank", "assets/banks/Music.bank");
    //sound_system.createEventInstance();
    //sound_system.play();
}

void Game::run()
{
    Shader shader = engine.loadShader(TextFormat("assets/shaders/lighting.vs", 330),
        TextFormat("assets/shaders/lighting.fs", 330));

    // Get some required shader locations
    shader.locs[SHADER_LOC_VECTOR_VIEW] = engine.getShaderLocation(shader, "viewPos");

    // Ambient light level (some basic lighting)
    int ambientLoc = engine.getShaderLocation(shader, "ambient");
    float ambientValue[4] = { 3.1f, 3.1f, 3.1f, -.6f };
    engine.setShaderValue(shader, ambientLoc, ambientValue, SHADER_UNIFORM_VEC4);

    render_system.setShader(shader);
    collision_system.setEventManager(evm);

    engine.setTargetFPS(120);

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
    float currentTime{}, elapsed{};

    createSound();
    li.sound_system = &sound_system;
    attack_system.setCollisionSystem(&collision_system);

    while (!li.gameShouldEnd)
    {
        elapsed += engine.getFrameTime();

        switch (li.currentScreen)
        {

            // CODIGO DE LA PANTALLA DE LOGO DE EMPRESA
        case GameScreen::LOGO:
        {
            // Contador para que pasen X segundos
            currentTime += timeStep60;
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
            if (input_system.pressEnter(engine))
                li.currentScreen = GameScreen::GAMEPLAY;
            render_system.drawStory(engine);

            if (em.getEntities().empty())
            {
                createEntities();
                map.reset(em, 0, iam);
            }

            break;
        }

        // CODIGO DEL GAMEPLAY
        case GameScreen::GAMEPLAY:
        {
            if (em.getEntities().empty() || li.resetGame)
                resetGame();

            if (li.resetFromDeath)
                resetDeath();

            if (li.levelChanged)
                map.createMap(em, li.mapID, iam);

            input_system.update(em, engine);
            bool debugs = inpi.debugAI1 || inpi.pause || inpi.inventory || txti.hasText();
            bool resets = li.resetGame || li.resetFromDeath;

            // seleccionar modo de debug ( physics o AI)
            if (!resets && !debugs)
            {
                while (elapsed >= timeStep)
                {
                    elapsed -= timeStep;

                    ai_system.update(em, timeStep);
                    physics_system.update(em, timeStep);
                    collision_system.update(em);
                    zone_system.update(em, engine, iam, evm, map, timeStep);
                    lock_system.update(em);
                    shield_system.update(em);
                    object_system.update(em, timeStep);
                    projectile_system.update(em, timeStep);
                    attack_system.update(em, timeStep);
                    life_system.update(em, object_system, timeStep);
                    sound_system.update();
                    // if (elapsed < timeStep) - Descomentar si queremos que la cámara se actualice solo cuando se actualice el render
                    camera_system.update(em, engine, timeStep);
                    event_system.update(em, evm, iam, map, object_system);

                    if (!li.getDeath().empty())
                    {
                        em.destroyEntities(li.getDeath());
                        li.clearDeath();
                    }
                }
                render_system.update(em, engine, timeStep);
            }
            else if (!resets && debugs)
                render_system.update(em, engine, timeStep);

            break;
        }

        // case GameScreen::DEAD:
        //     /* code */
        //     break;

        // CODIGO DE LA PANTALLA FINAL
        case GameScreen::ENDING:
        {
            if (input_system.pressEnter(engine))
                li.currentScreen = GameScreen::TITLE;

            render_system.drawEnding(engine);
            resetGame();
            break;
        }

        default:
            break;
        }
        if (elapsed >= timeStep)
            elapsed = 0; // Para que no se acumule el tiempo

        if (engine.windowShouldClose())
            li.gameShouldEnd = true;
    }

    //liberar bancos
    sound_system.clear();
    render_system.unloadModels(em, engine);

    engine.unloadGifs();
    engine.unloadShader(shader);
    engine.closeWindow();
}

void Game::resetDeath()
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& plfi = em.getSingleton<PlayerInfo>();

    li.deathReset();
    plfi.onDeath();
    auto& player = *em.getEntityByID(li.playerID);
    auto& lif = em.getComponent<LifeComponent>(player);

    em.getComponent<RenderComponent>(player).visible = true;
    em.getComponent<PhysicsComponent>(player).position = plfi.spawnPoint;
    lif.life = lif.maxLife;

    map.spawnReset(em, iam);
}

void Game::resetGame()
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& bb = em.getSingleton<BlackBoard_t>();
    auto& plfi = em.getSingleton<PlayerInfo>();
    auto& zchi = em.getSingleton<ZoneCheckInfo>();

    zchi.clearSets();
    em.destroyAll();
    bb.subditosData.clear();
    render_system.unloadModels(em, engine);
    li.reset();
    plfi.reset();
    lock_system.reset();
    map.reset(em, li.mapID, iam);
    createEntities();
    li.sound_system = &sound_system;
}