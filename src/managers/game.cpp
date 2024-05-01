#include "game.hpp"
#include <iomanip>
// #include "../utils/memory_viewer.hpp"
// #include <chrono>

void Game::createShield(Entity& ent)
{
    auto& e{ em.newEntity() };
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = em.getComponent<RenderComponent>(ent).position, .scale = { 1.0f, 1.0f, 0.5f }, .color = D_ORANGE_DARK });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position, .scale = r.scale });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::SHIELD });
    em.addComponent<ShieldComponent>(e, ShieldComponent{ .createdby = EntitieswithShield::Player });
}

void Game::createEnding()
{
    auto& e{ em.newEntity() };
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = {83.0f, 1.0f, -87.0f}, .scale = {1.0f, 1.0f, 1.0f}, .color = D_WHITE });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position, .scale = r.scale, .gravity = 0 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ENDING });
}

// ShaderType Game::createShader()
// {
//     ShaderType shader = engine.loadShader(TextFormat("assets/shaders/lighting.vs", 330),
//         TextFormat("assets/shaders/lighting.fs", 330));

//     // Get some required shader locations
//     shader.locs[SHADER_LOC_VECTOR_VIEW] = engine.getShaderLocation(shader, "viewPos");

//     // Ambient light level (some basic lighting)
//     int ambientLoc = engine.getShaderLocation(shader, "ambient");
//     float ambientValue[4] = { 3.1f, 3.1f, 3.1f, 20.0f };
//     engine.setShaderValue(shader, ambientLoc, ambientValue, SHADER_UNIFORM_VEC4);

//     return shader;
// }

void Game::createEntities()
{
    auto& plfi = em.getSingleton<PlayerInfo>();
    if (plfi.spawnPoint == vec3d::zero())
        plfi.spawnPoint = { -116.0, 4.0, 111.0 };

    // 33.0, 4.0, -25.9 - Posición Incial
    // 32.0, 4.0, 43.0 - Primer cofre
    // 32.0, 4.0, 130.0 - Segundo cofre
    // -72.0, 4.0, 72.9 - Cofre con llave
    // -116.0, 4.0, 111.0 - Apisonadora
    // -125, 4.0, 138.68 - `pos chunck 3
    // 7.0, 22.0, -21.0 - Posición Incial lvl1
    // -68.0, 4.0, -22.0 - Primera rampa lvl1
    // -126.0, 4.0, 152.0 - Segundo altar lvl1
    // -6.0, 4.0, 94.0 - Campamento lvl1
    // 34.0, 13.0, 99.0 - Pasillo antes segundo campamento lvl1
    // 30.0, 13.0, 213.0 - Segundo campamento lvl1
    // -26.0, 4.0, 235.0 - NPC lvl1
    // -113.0, 13.0, 236.0 - Final lvl1
    // 46.0, 13.0, 86.9 - pasillo nivel 1
    // -28.0, 49.0, -30.0 - Posición Incial lvl2
    // 34.0, 29.0, -60.0 - Segunda rampa lvl2
    // -7.55, 40.0, 16.0 - Lugar donde muñeco de nieve lvl2
    // 37.0, 13.0, -104.0 - Desp segunda lava lvl2
    // -95.0, 22.0, -135.0 - Spawn lvl2
    // -30.24, 49.0, -26.59 - Spawn crater lvl2
    // -58.26,31.0,16.54 - spawn ramp muñeco
    // 40.0, 50.0, -3.0 - Nomada lvl2

    // Player
    auto& e{ em.newEntity() };
    em.addTag<PlayerTag>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = plfi.spawnPoint, .scale = { 2.0, 6.0, 2.0 }, .color = D_WHITE });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = r.position, .scale = r.scale });
    // p.gravity = 0;

    em.addComponent<InputComponent>(e);
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 6 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::PLAYER });
    em.addComponent<AttackComponent>(e);
    auto& lis = em.addComponent<ListenerComponent>(e);

    // Listeners de eventos para el jugador
    for (auto i = 0; i < EventCodes::MAX; i++)
        lis.addCode(static_cast<EventCodes>(i));

    // Código de añadir un hechizo al jugador
    // Spell spell{ "Fireball", "Shoots a fireball", Spells::WaterDash, 20.0, 2 };
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
    em.getSingleton<SoundSystem>().initBanks("assets/banks/Master.bank", "assets/banks/Master.strings.bank", "assets/banks/UI.bank", "assets/banks/Ambient.bank", "assets/banks/Music.bank", "assets/banks/SFX.bank");
    //sound_system.createEventInstance();
    //sound_system.play();
}

void Game::run()
{
    // Cosas para medir el tiempo de ejecución
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    // Codigo para medir el tiempo de ejecucion
    //
    // - Colocar antes de donde se quiere medir el tiempo
    // auto t1 = high_resolution_clock::now();
    // - Colocar despues de donde se quiere medir el tiempo
    // auto t2 = high_resolution_clock::now();
    // auto duration = duration_cast<milliseconds>(t2 - t1);
    // std::cout << "Collision System: " << duration.count() << "ms" << std::endl;

    // Singletons
    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();
    auto& txti = em.getSingleton<TextInfo>();
    auto& gami = em.getSingleton<GameData>();

    // Shader ambiente
    // ShaderType shader = createShader();

    // render_system.setShader(shader);
    collision_system.setEventManager(evm);
    auto& sound_system = em.getSingleton<SoundSystem>();

    // Incializamos FPSs
    engine.setTargetFPS(60);

    // Nos aseguramos que los numeros aleatorios sean diferentes cada vez
    unsigned int seed = static_cast<unsigned int>(std::time(nullptr));

    // Revisamos si es replay
    std::ifstream is("assets/data/input/data.cereal");
    if (is.good())
    {
        cereal::BinaryInputArchive arch(is);
        arch(CEREAL_NVP(gami));
        li.replay = true;
        seed = gami.getRandomSeed();
    }

    std::srand(seed);

    gami.setRandomSeed(seed);
    engine.setReplayMode(li.replay, gami);
    sound_system.setVolumeMaster(0.3f);

    // Inicializa una variable donde tener el tiempo entre frames
    float currentTime{};
    float elapsed{};
    bool debugs{ false }, resets{ false };

    createSound();
    attack_system.setCollisionSystem(&collision_system);

    while (!li.gameShouldEnd)
    {
        elapsed += engine.getFrameTime();
        gami.updateFrame();

        switch (li.currentScreen)
        {
            // CODIGO DE LA PANTALLA DE LOGO DE EMPRESA
        case GameScreen::LOGO:
        {
            if (li.playerID == li.max)
                createEntities();
            // Contador para que pasen X segundos
            currentTime += timeStep45;
            if (currentTime > 4.0 || inpi.interact) {
                li.currentScreen = GameScreen::TITLE;
                currentTime = 0;
                inpi.interact = false;
            }
            input_system.update(em, engine);
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
            input_system.update(em, engine);
            render_system.drawLogoGame(engine, em, sound_system);
            if (li.playerID == li.max)
                createEntities();
            break;
        }

        // CODIGO DE LA PANTALLA DE OPCIONES
        case GameScreen::OPTIONS:
        {
            input_system.update(em, engine);
            render_system.drawOptions(engine, em, sound_system);
            break;
        }

        // CODIGO DE LA PANTALLA DE CONTROLES
        case GameScreen::CONTROLS:
        {
            input_system.update(em, engine);
            // render_system.update(em, engine);
            render_system.drawControls(em, engine);
            break;
        }

        // CODIGO DE LA PANTALLA DE PAUSA
        case GameScreen::PAUSE:
        {
            render_system.drawPauseMenu(engine, em);
            break;
        }

        // CODIGO DE LA PANTALLA DE HISTORIA
        case GameScreen::STORY:
        {
            // Input del enter para empezar la partida
            input_system.update(em, engine);
            if (inpi.interact)
            {
                li.currentScreen = GameScreen::GAMEPLAY;
                inpi.interact = false;
            }
            render_system.drawStory(engine);

            if (li.replay)
                gami.update();

            // TODO - Cuando se implemente el sistema de guardado, cargar el nivel en el que se quedó
            if (!map.isComplete())
                map.createMap(em, 0, iam);

            break;
        }

        // CODIGO DEL GAMEPLAY
        case GameScreen::GAMEPLAY:
        {
            if (sound_system.ambient_started == false) {
                //sound_system.playAmbient();
                sound_system.ambient_started = true;
            }
            if (em.getEntities().empty() || li.resetGame)
                resetGame();

            if (!map.isComplete())
            {
                if (!li.isCharging() && li.loading)
                {
                    li.loadingTime = 0;
                    if (!li.replay)
                    {
                        auto& gami = em.getSingleton<GameData>();
                        vec3d vel = { 0.0, 0.0, 0.0 };
                        gami.addMovementEvent(vel);
                    }
                }
                map.createMap(em, li.mapID, iam);
            }
            else if (map.isRespawning())
                map.spawnReset(em, iam);

            if (li.resetFromDeath)
                resetDeath();

            input_system.update(em, engine);
            debugs = inpi.debugAI1 || inpi.pause || inpi.inventory || txti.hasText();
            resets = li.resetGame || li.resetFromDeath;

            // seleccionar modo de debug ( physics o AI)
            if (!resets && !debugs)
            {
                if (elapsed >= timeStep45)
                {
                    elapsed -= timeStep45;

                    ai_system.update(em);
                    npc_system.update(em);
                    physics_system.update(em);
                    collision_system.update(em);
                    zone_system.update(em, engine, iam, evm, map);
                    lock_system.update(em);
                    shield_system.update(em);
                    object_system.update(em);
                    projectile_system.update(em);
                    attack_system.update(em);
                    life_system.update(em, object_system);
                    sound_system.update();
                    // if (elapsed < timeStep45) - Descomentar si queremos que la cámara se actualice solo cuando se actualice el render
                    camera_system.update(em, engine, evm);
                    event_system.update(em, evm, iam, map, object_system, sound_system);
                    particle_system.update(em);

                    if (!li.getDeath().empty())
                    {
                        em.destroyEntities(li.getDeath());
                        li.clearDeath();
                    }
                }
                render_system.update(em, engine);
            }
            else if (!resets && debugs)
                render_system.update(em, engine);

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
        if (elapsed >= timeStep45)
            elapsed = 0; // Para que no se acumule el tiempo

        if (engine.windowShouldClose())
            li.gameShouldEnd = true;
    }

    // Creamos un archivo de salida con los datos de la partida
    // Para hacer replay con este archivo hay que colocarlo en la carpeta assets/data/input
    if (!li.replay)
    {
        std::string name = "assets/data/output/data";
        // le ponemos la fecha y hora al archivo
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
        name += ss.str();
        name += ".cereal";
        std::ofstream os(name, std::ios::binary);
        cereal::BinaryOutputArchive archive(os);
        archive(CEREAL_NVP(gami));
    }

    //liberar bancos
    sound_system.clear();
    render_system.unloadModels(em, engine);

    // engine.unloadShader(shader);
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
    lif.markedForDeletion = false;

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
    render_system.resetAnimatedTexture();
    map.reset(em, li.mapID, iam);
    createEntities();
}