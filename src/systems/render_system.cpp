#include "render_system.hpp"
#include <iomanip>
#include "../../libs/raygui.h"

void RenderSystem::update(EntityManager& em, ENGI::GameEngine& engine, bool debug)
{

    // Actualizamos la posicion de render del componente de fisicas
    em.forEach<SYSCMPs, SYSTAGs>([](Entity&, PhysicsComponent& phy, RenderComponent& ren)
    {
        ren.setPosition(phy.position);
        ren.setOrientation(phy.orientation);
    });

    beginFrame(engine);

    // Dibuja todas las entidades con componente de render
    drawEntities(em, engine);

    endFrame(engine, em, debug);
}

void RenderSystem::drawLogoGame(ENGI::GameEngine& engine, EntityManager& em, SoundSystem& ss) {
    engine.beginDrawing();
    engine.clearBackground(WHITE);
    // Logo del videojuego
    engine.drawTexture(engine.texture_logo_two_wonders,
        engine.getScreenWidth() / 2 - engine.texture_logo_two_wonders.width / 2,
        static_cast<int>(engine.getScreenHeight() / 2.5 - engine.texture_logo_two_wonders.height / 2),
        WHITE);

    // Funcionalidad de botones
    Rectangle btn1Rec = { 300, 450, 200, 50 };
    Rectangle btn2Rec = { 300, 520, 200, 50 };

    auto& li = em.getSingleton<LevelInfo>();

    if (GuiButton(btn1Rec, "JUGAR")) {
        li.currentScreen = GameScreen::STORY;
        ss.seleccion_menu();
        ss.music_stop();
    }

    if (CheckCollisionPointRec(GetMousePosition(), btn1Rec) || CheckCollisionPointRec(GetMousePosition(), btn2Rec)) {
        if (ss.pushed == false)
            ss.sonido_mov();
        ss.pushed = true;
    }
    else
        ss.pushed = false;

    if (GuiButton(btn2Rec, "CONFIGURACION")) {
        li.currentScreen = GameScreen::OPTIONS;
        ss.seleccion_menu();
    }
    engine.endDrawing();
}

void RenderSystem::drawOptions(ENGI::GameEngine& engine, EntityManager& em, SoundSystem& ss) {
    engine.beginDrawing();
    engine.clearBackground(WHITE);

    // Slider del volumen
    Rectangle volumenSlider = { 100, 100, 200, 20 };
    float volumen = 50; // supongo que esto inicializa volumen
    float nuevoValor = static_cast<float>(GuiSliderBar(volumenSlider, "Volumen", NULL, &volumen, 0, 100));

    // Ahora asignamos el nuevo valor al puntero volumen
    volumen = nuevoValor;

    // Boton de volver al inicio
    Rectangle btn1Rec = { 300, 520, 200, 50 };
    auto& li = em.getSingleton<LevelInfo>();

    if (GuiButton(btn1Rec, "VOLVER")) {
        li.currentScreen = GameScreen::TITLE;
        ss.seleccion_menu();
    }

    if (CheckCollisionPointRec(GetMousePosition(), btn1Rec)) {
        if (ss.pushed == false)
            ss.sonido_mov();
        ss.pushed = true;
    }
    else
        ss.pushed = false;

    engine.endDrawing();
}

void RenderSystem::drawLogoKaiwa(ENGI::GameEngine& engine) {
    engine.beginDrawing();
    engine.clearBackground(Color({ 136, 219, 152, 255 }));
    engine.drawTexture(engine.texture_logo_kaiwa_games,
        engine.getScreenWidth() / 2 - engine.texture_logo_kaiwa_games.width / 2,
        engine.getScreenHeight() / 2 - engine.texture_logo_kaiwa_games.height / 2,
        WHITE);
    engine.endDrawing();
}

void RenderSystem::drawEnding(ENGI::GameEngine& engine) {
    engine.beginDrawing();
    engine.clearBackground(WHITE);
    engine.drawText("FIN DEL NIVEL 1", 250, 250, 50, BLACK);
    engine.drawText("PRESS [ENTER] TO RETURN TITLE",
        engine.getScreenWidth() / 2 - 280,
        engine.getScreenHeight() - 50, 30,
        BLACK);
    engine.endDrawing();
}

void RenderSystem::drawStory(ENGI::GameEngine& engine) {
    engine.beginDrawing();
    engine.clearBackground(WHITE);
    engine.drawText("Busca la espada en la cueva", 45, 200, 40, BLACK);
    engine.drawText("Extermina al dragón de la mazmorra", 45, 250, 40, BLACK);
    engine.drawText("(Ana quería historia)", 45, 300, 40, BLACK);
    engine.drawText("PRESS [ENTER] TO PLAY ",
        engine.getScreenWidth() / 2 - 200,
        engine.getScreenHeight() - 50, 30,
        BLACK);
    engine.endDrawing();
}

void RenderSystem::drawEntities(EntityManager& em, ENGI::GameEngine& engine)
{
    for (auto const& e : em.getEntities())
    {
        if (e.hasComponent<RenderComponent>())
        {
            auto& r{ em.getComponent<RenderComponent>(e) };
            if (r.visible) {
                // Revisamos si es el jugador para mover la cámara
                if (e.hasTag<PlayerTag>())
                {
                    if (!r.cameraChange)
                        engine.setPositionCamera({ r.position.x() + 8.f, r.position.y() + 10.f, r.position.z() + 8.f });
                    else
                        engine.setPositionCamera({ r.position.x() - 8.f, r.position.y() + 10.f, r.position.z() + 8.f });

                    engine.setTargetCamera(r.position);
                }
                // Comprobar si tiene el componente vida

                Color colorEntidad = r.color;

                // Comprobar el tipo y si es enemigo cambiarle el color
                if (!e.hasTag<PlayerTag>() && e.hasComponent<TypeComponent>()) {
                    auto& t{ em.getComponent<TypeComponent>(e) };

                    switch (t.type)
                    {
                    case ElementalType::Neutral:
                        colorEntidad = GRAY;
                        break;

                    case ElementalType::Agua:
                        colorEntidad = BLUE;
                        break;

                    case ElementalType::Fuego:
                        colorEntidad = RED;
                        break;

                    case ElementalType::Hielo:
                        colorEntidad = SKYBLUE;
                        break;

                    default:
                        break;
                    }
                }

                if (e.hasComponent<LifeComponent>()) {
                    auto& l{ em.getComponent<LifeComponent>(e) };
                    if (l.elapsed < l.countdown)
                        colorEntidad = MAROON;
                }
                if (!e.hasTag<ZoneTag>())
                {
                    vec3d scl = { 1.0, 1.0, 1.0 };
                    vec3d pos = { r.position.x(), r.position.y(), r.position.z() };
                    // Solo generamos la malla si no existe
                    if (!r.meshLoaded)
                    {
                        if (e.hasTag<PlayerTag>())
                        {
                            r.model = LoadModel("assets/models/main_character.obj");
                            Texture2D t0 = LoadTexture("assets/models/textures/main_character_uv_V2.png");
                            Texture2D t = LoadTexture("assets/models/textures/main_character_texture_V2.png");
                            r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
                            r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
                        }
                        else if (e.hasTag<SlimeTag>())
                        {
                            r.model = LoadModel("assets/models/Slime.obj");
                        }
                        else if (e.hasTag<SnowmanTag>())
                        {
                            r.model = LoadModel("assets/models/snowman.obj");
                            Texture2D t0 = LoadTexture("assets/models/textures/snowman_uv.png");
                            Texture2D t = LoadTexture("assets/models/textures/snowman_texture.png");
                            r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
                            r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
                        }
                        else
                        {
                            r.mesh = engine.genMeshCube(static_cast<float>(r.scale.x()), static_cast<float>(r.scale.y()), static_cast<float>(r.scale.z()));
                            r.model = engine.loadModelFromMesh(r.mesh);
                        }
                        r.meshLoaded = true;
                    }

                    if (e.hasTag<PlayerTag>())
                    {
                        scl = { 0.33, 0.33, 0.33 };
                        pos.setY(pos.y() - .5);
                    }
                    else if (e.hasTag<SlimeTag>())
                    {
                        scl = { 0.33, 0.33, 0.33 };
                        pos.setY(pos.y() - .6);
                    }
                    else if (e.hasTag<SnowmanTag>())
                    {
                        scl = { 0.33, 0.33, 0.33 };
                        pos.setY(pos.y() - 1.1);
                    }

                    float orientationInDegrees = static_cast<float>(r.orientation * (180.0f / M_PI));
                    engine.drawModel(r.model, pos, r.rotationVec, orientationInDegrees, scl, colorEntidad);

                    if (!e.hasTag<PlayerTag>() && !e.hasTag<SlimeTag>() && !e.hasTag<SnowmanTag>())
                    {
                        int orientationInDegreesInt = static_cast<int>(orientationInDegrees);
                        if (orientationInDegreesInt % 90 == 0)
                            engine.drawCubeWires(r.position, static_cast<float>(r.scale.x()), static_cast<float>(r.scale.y()), static_cast<float>(r.scale.z()), BLACK);
                        else
                            engine.drawModelWires(r.model, pos, r.rotationVec, orientationInDegrees, scl, BLACK);
                    }
                }
            }
        }
    }
}

// Empieza el dibujado y se limpia la pantalla
void RenderSystem::beginFrame(ENGI::GameEngine& engine)
{
    engine.beginDrawing();

    engine.clearBackground(RAYWHITE);

    engine.beginMode3D();
    //engine.drawGrid(50, 1.f);
}

// Se termina el dibujado
void RenderSystem::endFrame(ENGI::GameEngine& engine, EntityManager& em, bool debug)
{
    engine.endMode3D();

    drawHUD(em, engine, debug);

    engine.endDrawing();
}

// Se dibuja el HUD
void RenderSystem::drawHUD(EntityManager& em, ENGI::GameEngine& engine, bool debug)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto* playerEn = em.getEntityByID(li.playerID);
    if (not playerEn) { drawDeath(engine); return; }
    else if (!playerEn->hasTag<PlayerTag>()) { drawDeath(engine); return; }

    // Visualizar las vidas del player
    for (auto const& e : em.getEntities())
    {

        if (e.hasTag<PlayerTag>())
        {
            // Dibujar background HUD
            engine.drawRectangle(0, 0, 580, 60, WHITE);
            engine.drawRectangle(0, 55, 100, 30, WHITE);

            // Dibujar vidas restantes del player en el HUD
            if (e.hasComponent<LifeComponent>())
            {
                auto const& l{ em.getComponent<LifeComponent>(e) };
                std::string vida = "Vidas: " + std::to_string(l.life) + " (max " + std::to_string(l.maxLife) + ") - ";
                engine.drawText(vida.c_str(), 10, 10, 20, BLACK);
            }

            // Dibujar las bombas y monedas actuales
            if (e.hasComponent<InformationComponent>())
            {
                auto const& info{ em.getComponent<InformationComponent>(e) };
                std::string info_text = "Bombas: " + std::to_string(info.bombs) + " (max " + std::to_string(info.max_bombs) + ") - Monedas: " + std::to_string(info.coins);
                engine.drawText(info_text.c_str(), 200, 10, 20, BLACK);

                std::string debug_txt = "F1 to DEBUG";
                engine.drawText(debug_txt.c_str(), 420, 35, 20, BLACK);
            }

            // Dibujar el countdown restante del ataque del player en el HUD
            if (e.hasComponent<AttackComponent>())
            {
                auto const& a{ em.getComponent<AttackComponent>(e) };
                std::string countdown_ataque;
                if (a.elapsed > a.countdown)
                    countdown_ataque = "Ataque listo (SPACE) - Bomba lista (B)";
                else
                    countdown_ataque = "Ataque listo en: " + std::to_string(-1 * (a.elapsed - 1.0f)) + " segundos";

                engine.drawText(countdown_ataque.c_str(), 10, 35, 20, BLACK);
            }

            // Dibujar el tipo de ataque que tiene equipado
            if (e.hasComponent<TypeComponent>())
            {
                auto const& t{ em.getComponent<TypeComponent>(e) };

                if (t.type == ElementalType::Neutral)
                    engine.drawText("Neutral", 10, 60, 20, BLACK);
                else if (t.type == ElementalType::Agua)
                    engine.drawText("Agua", 10, 60, 20, BLUE);
                else if (t.type == ElementalType::Fuego)
                    engine.drawText("Fuego", 10, 60, 20, RED);
                else
                    engine.drawText("Hielo", 10, 60, 20, SKYBLUE);
            }

        }

        // Dibujar el precio d elos objetos de la tienda
        if (e.hasTag<ObjectTag>()) {
            if (e.hasComponent<ObjectComponent>() && e.hasComponent<RenderComponent>())
            {
                auto& ren{ em.getComponent<RenderComponent>(e) };
                auto& obj{ em.getComponent<ObjectComponent>(e) };
                if (obj.type == Object_type::ShopItem_Bomb)
                    engine.drawText("20",
                        static_cast<int>(engine.getWorldToScreenX(ren.position) - 10),
                        static_cast<int>(engine.getWorldToScreenY(ren.position) + ren.scale.y() * 50),
                        20,
                        BLACK);
                else if (obj.type == Object_type::ShopItem_Life)
                    engine.drawText("10",
                        static_cast<int>(engine.getWorldToScreenX(ren.position) - 10),
                        static_cast<int>(engine.getWorldToScreenY(ren.position) + ren.scale.y() * 50),
                        20,
                        BLACK);
                else if (obj.type == Object_type::ShopItem_ExtraLife)
                    engine.drawText("30",
                        static_cast<int>(engine.getWorldToScreenX(ren.position) - 10),
                        static_cast<int>(engine.getWorldToScreenY(ren.position) + ren.scale.y() * 50),
                        20,
                        BLACK);
            }
        }


        if (e.hasComponent<LifeComponent>() && em.getComponent<RenderComponent>(e).visible)
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            auto const& l{ em.getComponent<LifeComponent>(e) };

            engine.drawText(std::to_string(l.life).c_str(),
                static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
                static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 50),
                20,
                BLACK);
        }


        if (debug && e.hasComponent<LifeComponent>() && em.getComponent<RenderComponent>(e).visible)
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            //auto const& l{ em.getComponent<LifeComponent>(e) };

            //engine.drawText(std::to_string(l.life).c_str(),
            //    static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
            //    static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 50),
            //    20,
            //    BLACK);

            if (e.hasComponent<TypeComponent>())
            {
                auto const& t{ em.getComponent<TypeComponent>(e) };

                std::string tipo = "Hielo";
                Color color = SKYBLUE;

                if (t.type == ElementalType::Neutral)
                {
                    tipo = "Neutral";
                    color = BLACK;
                }
                else if (t.type == ElementalType::Agua)
                {
                    tipo = "Agua";
                    color = BLUE;
                }
                else if (t.type == ElementalType::Fuego)
                {
                    tipo = "Fuego";
                    color = RED;
                }

                engine.drawText(tipo.c_str(),
                    static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
                    static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 70),
                    20,
                    color);
            }

        }

        if (debug && e.hasComponent<ColliderComponent>())
        {
            auto& col{ em.getComponent<ColliderComponent>(e) };

            // Calcular la posición y el tamaño de la bounding box
            vec3d boxPosition = (col.boundingBox.min + col.boundingBox.max) / 2;
            vec3d boxSize = col.boundingBox.max - col.boundingBox.min;

            // Dibujar la bounding box
            engine.beginMode3D();
            engine.drawCubeWires(boxPosition,
                static_cast<float>(boxSize.x()),
                static_cast<float>(boxSize.y()),
                static_cast<float>(boxSize.z()),
                BLUE);
            engine.endMode3D();

        }

        if (debug && e.hasComponent<PhysicsComponent>() && e.hasComponent<ColliderComponent>() && e.hasComponent<RenderComponent>())
        {
            auto& phy = em.getComponent<PhysicsComponent>(e);

            RayCast ray = engine.getMouseRay();

            auto& col = em.getComponent<ColliderComponent>(e);
            auto& ren = em.getComponent<RenderComponent>(e);
            // Comprobar si el rayo intersecta con el collider
            if (col.boundingBox.intersectsRay(ray.origin, ray.direction) && !(col.behaviorType & BehaviorType::STATIC || col.behaviorType & BehaviorType::ZONE))
            {
                // Dibujar el HUD de debug
                engine.drawRectangle(0, 60, 150, 240, WHITE);
                engine.drawText("Posición", 10, 70, 20, BLACK);
                std::string posX = "X: " + std::to_string(static_cast<int>(phy.position.x()));
                engine.drawText(posX.c_str(), 10, 95, 20, BLACK);
                std::string posY = "Y: " + std::to_string(static_cast<int>(phy.position.y()));
                engine.drawText(posY.c_str(), 10, 120, 20, BLACK);
                std::string posZ = "Z: " + std::to_string(static_cast<int>(phy.position.z()));
                engine.drawText(posZ.c_str(), 10, 145, 20, BLACK);

                engine.drawText("Velocidad", 10, 175, 20, BLACK);
                std::string velX = "X: " + std::to_string(phy.velocity.x());
                engine.drawText(velX.c_str(), 10, 200, 20, BLACK);
                std::string velY = "Y: " + std::to_string(phy.velocity.y());
                engine.drawText(velY.c_str(), 10, 225, 20, BLACK);
                std::string velZ = "Z: " + std::to_string(phy.velocity.z());
                engine.drawText(velZ.c_str(), 10, 250, 20, BLACK);

                engine.beginMode3D();
                engine.drawCubeWires(ren.position, static_cast<float>(ren.scale.x()), static_cast<float>(ren.scale.y()), static_cast<float>(ren.scale.z()), RED);
                engine.endMode3D();
            }
        }

        // Dibujar el ID de las entidades // DEBUG
        // if (debug)
        // {
        //     auto const& r{ em.getComponent<RenderComponent>(e) };
        //     engine.drawText(std::to_string(e.getID()).c_str(),
        //         static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
        //         static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 50),
        //         20,
        //         BLACK);
        // }
    }
}

void RenderSystem::drawDeath(ENGI::GameEngine& engine)
{
    engine.drawRectangle(0, 0, engine.getScreenWidth(), engine.getScreenHeight(), Fade(BLACK, 0.5f));
    engine.drawText("HAS MUERTO", 250, 250, 40, RED);
    engine.drawText("[ENTER] para volver a jugar", 165, 300, 30, RED);
}

void RenderSystem::unloadModels(EntityManager& em, ENGI::GameEngine& engine)
{
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity&, PhysicsComponent&, RenderComponent& ren)
    {
        // engine.unloadMesh(ren.mesh);
        engine.unloadModel(ren.model);
        ren.meshLoaded = false;
    });
}
