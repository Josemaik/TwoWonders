#include "render_system.hpp"
#include <iomanip>
#include "../../libs/raygui.h"

void RenderSystem::update(EntityManager& em, ENGI::GameEngine& engine, double dt)
{
    // Actualizamos la posicion de render del componente de fisicas
    em.forEach<SYSCMPs, SYSTAGs>([](Entity&, PhysicsComponent& phy, RenderComponent& ren)
    {
        ren.setPosition(phy.position);
        ren.setOrientation(phy.orientation);
    });

    // Empezamos el frame
    beginFrame(engine);

    // Dibuja todas las entidades con componente de render
    drawEntities(em, engine);

    // Terminamos el frame
    endFrame(engine, em, dt);
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
    using SYSCMPs = MP::TypeList<RenderComponent>;

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, RenderComponent& r)
    {
        if (r.visible) {
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
                        Texture2D t0 = LoadTexture("assets/models/textures/entity_textures/main_character_uv_V2.png");
                        Texture2D t = LoadTexture("assets/models/textures/entity_textures/main_character_texture_V2.png");
                        r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
                        r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
                    }
                    else if (e.hasTag<SlimeTag>())
                    {
                        r.model = LoadModel("assets/models/Slime.obj");
                        Texture2D t0 = LoadTexture("assets/models/textures/entity_textures/Slime_uv.png");
                        Texture2D t = LoadTexture("assets/models/textures/entity_textures/Slime_texture.png");
                        r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
                        r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
                    }
                    else if (e.hasTag<SnowmanTag>())
                    {
                        r.model = LoadModel("assets/models/snowman.obj");
                        Texture2D t0 = LoadTexture("assets/models/textures/entity_textures/snowman_uv.png");
                        Texture2D t = LoadTexture("assets/models/textures/entity_textures/snowman_texture.png");
                        r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
                        r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
                    }
                    else if (e.hasTag<GolemTag>())
                    {
                        r.model = LoadModel("assets/models/Golem.obj");
                        Texture2D t0 = LoadTexture("assets/models/textures/entity_textures/Golem_uv.png");
                        Texture2D t = LoadTexture("assets/models/textures/entity_textures/Golem_texture.png");
                        r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
                        r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
                    }
                    else if (e.hasTag<SpiderTag>())
                    {
                        r.model = LoadModel("assets/models/Spider.obj");
                        Texture2D t0 = LoadTexture("assets/models/textures/entity_textures/Spider_UV.png");
                        Texture2D t = LoadTexture("assets/models/textures/entity_textures/Spider_texture.png");
                        r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
                        r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
                    }
                    else if (e.hasTag<BossFinalTag>())
                    {
                        r.model = LoadModel("assets/models/Boss.obj");
                        Texture2D t0 = LoadTexture("assets/models/textures/entity_textures/Boss_uv.png");
                        Texture2D t = LoadTexture("assets/models/textures/entity_textures/Boss_texture.png");
                        r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
                        r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
                    }
                    else if (e.hasTag<SubditoTag>())
                    {
                        r.model = LoadModel("assets/models/Boss_sub_1.obj");
                        Texture2D t0 = LoadTexture("assets/models/textures/entity_textures/Boss_sub_1_uv.png");
                        Texture2D t = LoadTexture("assets/models/textures/entity_textures/Boss_sub_1_texture.png");

                        r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
                        r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
                    }
                    else if (e.hasTag<GroundTag>())
                    {
                        r.model = LoadModel("assets/models/map_models/lvl_0-cnk0.obj");
                        // Texture2D t = LoadTexture("assets/models/textures/map_textures/lvl0_texture.png");
                        // Texture2D t2 = LoadTexture("assets/models/textures/map_textures/lvl_0-extras_texture.png");
                        // r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
                        // r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t2;
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
                    //scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 1.5);
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
                else if (e.hasTag<SpiderTag>())
                {
                    scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 0.7);
                }
                else if (e.hasTag<GolemTag>())
                {
                    scl = { 0.4, 0.4, 0.4 };
                    pos.setY(pos.y() - 1.1);
                }
                else if (e.hasTag<BossFinalTag>())
                {
                    scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 1.1);
                    colorEntidad = { 125, 125, 125, 255 };
                }
                else if (e.hasTag<SubditoTag>())
                {
                    scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 1.1);
                }


                float orientationInDegrees = static_cast<float>(r.orientation * (180.0f / M_PI));
                engine.drawModel(r.model, pos, r.rotationVec, orientationInDegrees, scl, colorEntidad);

                if (!e.hasTag<PlayerTag>() && !e.hasTag<SlimeTag>() && !e.hasTag<SnowmanTag>() && !e.hasTag<GolemTag>() && !e.hasTag<SpiderTag>() && !e.hasTag<BossFinalTag>() && !e.hasTag<SubditoTag>())
                {
                    int orientationInDegreesInt = static_cast<int>(orientationInDegrees);
                    if (orientationInDegreesInt % 90 == 0 && std::abs(orientationInDegreesInt) != 270 && std::abs(orientationInDegreesInt) != 90)
                        engine.drawCubeWires(r.position, static_cast<float>(r.scale.x()), static_cast<float>(r.scale.y()), static_cast<float>(r.scale.z()), BLACK);
                    else
                        engine.drawModelWires(r.model, pos, r.rotationVec, orientationInDegrees, scl, BLACK);
                }
            }
        }
    });
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
void RenderSystem::endFrame(ENGI::GameEngine& engine, EntityManager& em, double dt)
{
    engine.endMode3D();

    auto& inpi = em.getSingleton<InputInfo>();

    drawHUD(em, engine, inpi.debugPhy);
    // Si se pulsa F2 se activa editor  de parámetros In-game
    if (inpi.debugAI1) {
        drawEditorInGameIA(engine, em);
    }
    // Visual Debug AI
    if (inpi.debugAI2)
        drawDebuggerInGameIA(engine, em, dt);

    engine.endDrawing();
}

//Dibuja Slider en función de los parámetros
double SelectValue(double value, float posx, float posy, float height, float width, const char* text, float min_value, float max_value) {
    // pasamos a float el valor
    float floatvalue = static_cast<float>(value);
    // dibujamos el slider para modificar su valor
    int new_detect_radius = GuiSliderBar(Rectangle(posx, posy, height, width), text, NULL, &floatvalue, min_value, max_value);
    new_detect_radius = new_detect_radius + 1;
    DrawText(std::to_string(floatvalue).c_str(), 220, static_cast<int>(posy + 5.0f), 20, BLUE);
    // seteamos el nuevo valor
    return static_cast<double>(floatvalue);
}
//Debugger visual in-game
void RenderSystem::drawDebuggerInGameIA(ENGI::GameEngine& engine, EntityManager& em, double dt) {
    // engine.beginDrawing();
    Rectangle windowRect = { 470, 80, 330, 230 };
    DrawRectangleLinesEx(windowRect, 2, DARKGRAY);
    DrawRectangleRec(windowRect, Color{ 255, 255, 255, 128 });
    Vector2 textPositionInfo = { 480, 90 };
    DrawTextEx(GetFontDefault(), "INFO", textPositionInfo, 20, 1, RED);
    auto& debugsnglt = em.getSingleton<Debug_t>();

    using SYSCMPss = MP::TypeList<AIComponent, ColliderComponent, RenderComponent>;
    using SYSTAGss = MP::TypeList<EnemyTag>;

    // AQUI PONDRIA
    em.forEach<SYSCMPss, SYSTAGss>([&](Entity& e, AIComponent& aic, ColliderComponent& col, RenderComponent& ren)
    {
        RayCast ray = engine.getMouseRay(); ray = engine.getMouseRay();
        if (col.boundingBox.intersectsRay(ray.origin, ray.direction) && !(col.behaviorType & BehaviorType::STATIC || col.behaviorType & BehaviorType::ZONE)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isSelectedfordebug = !isSelectedfordebug;
                debugsnglt.IA_id_debug = e.getID();
            }
        }
        if (isSelectedfordebug && e.getID() == debugsnglt.IA_id_debug) {
            auto& bb = em.getSingleton<BlackBoard_t>();
            engine.beginMode3D();
            engine.drawCubeWires(ren.position, static_cast<float>(ren.scale.x()), static_cast<float>(ren.scale.y()), static_cast<float>(ren.scale.z()), PURPLE);
            engine.endMode3D();
            DrawText("Node active:", 480, 110, 20, BLACK);
            if (debugsnglt.elapsed >= debugsnglt.countdown) {
                debugsnglt.elapsed = 0;
                debugsnglt.text = aic.bh;
            }
            else {
                debugsnglt.plusdeltatime(dt, debugsnglt.elapsed);
            }
            DrawTextEx(GetFontDefault(), debugsnglt.text, Vector2{ 610,110 }, 20, 1, DARKGRAY);
            DrawText("TEID:", 480, 130, 20, BLACK);
            DrawTextEx(GetFontDefault(), std::to_string(aic.teid).c_str(), Vector2{ 550,130 }, 20, 1, DARKGRAY);
            DrawText("TX:", 480, 150, 20, BLACK);
            DrawTextEx(GetFontDefault(), std::to_string(aic.tx).c_str(), Vector2{ 520,150 }, 20, 1, DARKGRAY);
            DrawText("TZ:", 480, 170, 20, BLACK);
            DrawTextEx(GetFontDefault(), std::to_string(aic.tz).c_str(), Vector2{ 520,170 }, 20, 1, DARKGRAY);
            DrawText("Culldown:", 480, 190, 20, BLACK);
            DrawTextEx(GetFontDefault(), std::to_string(aic.elapsed_shoot).c_str(), Vector2{ 590,190 }, 20, 1, DARKGRAY);
            DrawText("Player Detected?:", 480, 210, 20, BLACK);
            DrawTextEx(GetFontDefault(), (aic.playerdetected == 0) ? "No" : "Sí", Vector2{ 680,210 }, 20, 1, RED);
            DrawText("Player hunted?:", 480, 230, 20, BLACK);
            DrawTextEx(GetFontDefault(), (bb.playerhunted == 0) ? "No" : "Sí", Vector2{ 680,230 }, 20, 1, RED);
            DrawText("Subditos alive:", 480, 250, 20, BLACK);
            DrawTextEx(GetFontDefault(), std::to_string(bb.subditosData.size()).c_str(), Vector2{ 680,250 }, 20, 1, RED);
            DrawText("Subditos id alive:", 480, 270, 20, BLACK);
            DrawTextEx(GetFontDefault(), std::to_string(bb.idsubditos.size()).c_str(), Vector2{ 680,270 }, 20, 1, RED);
        }
    });
    //  engine.endDrawing();
}
//Editor In-Game
void RenderSystem::drawEditorInGameIA(ENGI::GameEngine& engine, EntityManager& em) {
    // engine.beginDrawing();

    // Dibujar un rectángulo que simula una ventana
    Rectangle windowRect = { 0, 100, 340, 550 };
    DrawRectangleLinesEx(windowRect, 2, DARKGRAY);
    DrawRectangleRec(windowRect, Color{ 255, 255, 255, 128 });

    // Dibujar el texto "debugger IA" en el centro de la ventana
    Vector2 textSize = MeasureTextEx(GetFontDefault(), "Debugger IA", 20, 1);
    Vector2 textPosition = { windowRect.x + 20,
                             windowRect.y + 10 };

    DrawTextEx(GetFontDefault(), "Editor IA", textPosition, 20, 1, DARKBLUE);

    // Dibujar una línea recta debajo del texto
    float lineY = textPosition.y + textSize.y + 5;  // Ajusta la posición de la línea según tus necesidades
    DrawLine(static_cast<int>(windowRect.x), static_cast<int>(lineY), static_cast<int>(windowRect.x) + static_cast<int>(windowRect.width),
        static_cast<int>(lineY), DARKGRAY);
    // Dibujar el texto "INFO" debajo de la línea

    Vector2 textPositionParameters = { windowRect.x + 5, 150 };

    DrawTextEx(GetFontDefault(), "PARÁMETROS", textPositionParameters, 20, 1, RED);

    auto& debugsnglt = em.getSingleton<Debug_t>();

    using SYSCMPss = MP::TypeList<AIComponent, PhysicsComponent, ColliderComponent, RenderComponent>;
    using SYSTAGss = MP::TypeList<EnemyTag>;

    // AQUI PONDRIA
    em.forEach<SYSCMPss, SYSTAGss>([&](Entity& e, AIComponent& aic, PhysicsComponent& phy, ColliderComponent& col, RenderComponent& ren)
    {
        RayCast ray = engine.getMouseRay();
        // Comprobar si el rayo intersecta con el collider
        if (col.boundingBox.intersectsRay(ray.origin, ray.direction) && !(col.behaviorType & BehaviorType::STATIC || col.behaviorType & BehaviorType::ZONE)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isSelected = !isSelected;
                debugsnglt.IA_id = e.getID();
            }
            //     // si es seleccionada => wires morados
            //     // no es seleccionada => wires rojos
            engine.beginMode3D();
            engine.drawCubeWires(ren.position, static_cast<float>(ren.scale.x()), static_cast<float>(ren.scale.y()), static_cast<float>(ren.scale.z()), RED);
            engine.endMode3D();
        }
        if (isSelected && e.getID() == debugsnglt.IA_id) {
            engine.beginMode3D();
            engine.drawCubeWires(ren.position, static_cast<float>(ren.scale.x()), static_cast<float>(ren.scale.y()), static_cast<float>(ren.scale.z()), PURPLE);
            engine.endMode3D();
            // si se seleccionada una entidad se muestra el Editor de parámetros
            if (isSelected) {
                // ID DE LA ENTIDAD SELECCIONADA
                DrawText("EID:", 5, 170, 20, BLACK);
                DrawText(std::to_string(debugsnglt.IA_id).c_str(), 55, 170, 20, DARKGRAY);
                //Detect Radius
                aic.detect_radius = SelectValue(aic.detect_radius, 85.0, 200.0, 120.0, 30.0, "Detect Radius", 0.0, 100.0);
                // Attack Radius
                aic.attack_radius = SelectValue(aic.attack_radius, 85.0, 240.0, 120.0, 30.0, "Attack Radius", 0.0, 100.0);
                // Arrival Radius
                aic.arrival_radius = SelectValue(aic.arrival_radius, 85.0, 280.0, 120.0, 30.0, "Arrival Radius", 0.0, 100.0);
                // Max Speed
                phy.max_speed = SelectValue(phy.max_speed, 85.0, 320.0, 120.0, 30.0, "Max_Speed", 0.0, 10.0);
                //COuntdown Perception
                aic.countdown_perception = SelectValue(aic.countdown_perception, 85.0, 360.0, 120.0, 30.0, "Perception", 0.0, 10.0);
                //Countdown Shoot
                aic.countdown_shoot = SelectValue(aic.countdown_shoot, 85.0, 400.0, 120.0, 30.0, "Culldown Shoot", 0.0, 8.0);
                //Countdown stop
                aic.countdown_stop = SelectValue(aic.countdown_stop, 85.0, 440.0, 120.0, 30.0, "Culldown Stop", 0.0, 8.0);
            }
        }
    });
    // engine.endDrawing();
}
// Se dibuja el HUD
void RenderSystem::drawHUD(EntityManager& em, ENGI::GameEngine& engine, bool debugphy)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto* playerEn = em.getEntityByID(li.playerID);
    if (not playerEn) {
        drawDeath(engine);
        return;
    }
    else if (!playerEn->hasTag<PlayerTag>()) { drawDeath(engine); return; }

    // Visualizar las vidas del player
    for (auto const& e : em.getEntities())
    {

        if (e.hasTag<PlayerTag>())
        {
            // Dibujar background HUD
            // engine.drawRectangle(0, 0, 580, 60, WHITE);

            // Dibujar vidas restantes del player en el HUD
            if (e.hasComponent<LifeComponent>())
            {
                drawHealthBar(engine, em, e);
            }

            // Dibujamos la cantidad de mana restante del player en el HUD
            drawManaBar(engine, em);

            // Dibujamos el número de monedas en pantalla
            drawCoinBar(engine, em);

            // Dibujar el countdown restante del ataque del player en el HUD
            // if (e.hasComponent<AttackComponent>())
            // {
            //     auto const& a{ em.getComponent<AttackComponent>(e) };
            //     std::string countdown_ataque;
            //     if (a.elapsed > a.countdown)
            //         countdown_ataque = "Ataque listo (SPACE) - Bomba lista (B)";
            //     else
            //         countdown_ataque = "Ataque listo en: " + std::to_string(-1 * (a.elapsed - 1.0f)) + " segundos";

            //     engine.drawText(countdown_ataque.c_str(), 10, 35, 20, BLACK);
            // }

            // Dibujar el tipo de ataque que tiene equipado
            if (e.hasComponent<TypeComponent>())
            {
                // Rectangulo del tipo de magia
                engine.drawRectangle(7, 50, 100, 20, DARKGRAY);

                auto const& t{ em.getComponent<TypeComponent>(e) };

                if (t.type == ElementalType::Neutral)
                    engine.drawText("Neutral", 17, 50, 18, WHITE);
                else if (t.type == ElementalType::Agua)
                    engine.drawText("Agua", 17, 50, 18, BLUE);
                else if (t.type == ElementalType::Fuego)
                    engine.drawText("Fuego", 17, 50, 18, RED);
                else
                    engine.drawText("Hielo", 17, 50, 18, SKYBLUE);
            }
        }

        // Dibujar el precio d elos objetos de la tienda
        if (e.hasTag<ObjectTag>()) {
            if (e.hasComponent<ObjectComponent>() && e.hasComponent<RenderComponent>())
            {
                auto& ren{ em.getComponent<RenderComponent>(e) };
                auto& obj{ em.getComponent<ObjectComponent>(e) };
                if (obj.type == ObjectType::ShopItem_Bomb)
                    engine.drawText("20",
                        static_cast<int>(engine.getWorldToScreenX(ren.position) - 10),
                        static_cast<int>(engine.getWorldToScreenY(ren.position) + ren.scale.y() * 50),
                        20,
                        BLACK);
                else if (obj.type == ObjectType::ShopItem_Life)
                    engine.drawText("10",
                        static_cast<int>(engine.getWorldToScreenX(ren.position) - 10),
                        static_cast<int>(engine.getWorldToScreenY(ren.position) + ren.scale.y() * 50),
                        20,
                        BLACK);
                else if (obj.type == ObjectType::ShopItem_ExtraLife)
                    engine.drawText("30",
                        static_cast<int>(engine.getWorldToScreenX(ren.position) - 10),
                        static_cast<int>(engine.getWorldToScreenY(ren.position) + ren.scale.y() * 50),
                        20,
                        BLACK);
            }
        }

        // Vidas HUD
        if (e.hasTag<EnemyTag>() && e.hasComponent<LifeComponent>() && em.getComponent<RenderComponent>(e).visible)
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            auto& l{ em.getComponent<LifeComponent>(e) };

            // engine.drawText(std::to_string(l.life).c_str(),
            //     static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
            //     static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 50),
            //     20,
            //     BLACK);

            // Datos para la barra para la barra de vida
            int barWidth = 40;
            int barHeight = 4;
            int barX = static_cast<int>(engine.getWorldToScreenX(r.position)) - 18;
            int barY = static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 35);

            engine.drawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);

            // Normaliza la vida actual del personaje
            float normalizedLife = (static_cast<float>(l.life) / static_cast<float>(l.maxLife));

            // Calcula la anchura de la barra de vida
            int lifeWidth = static_cast<int>(static_cast<float>(barWidth) * normalizedLife);

            if (!l.vidaMax())
                lifeWidth = l.life_width + static_cast<int>((static_cast<float>(lifeWidth) - static_cast<float>(l.life_width)) * 0.25);

            // Dibujamos la barra de vida
            engine.drawRectangle(barX, barY, lifeWidth, barHeight, RED);

            l.life_width = lifeWidth;

            // if (e.hasTag<SpiderTag>())
            //     std::cout << newLifeWidth << std::endl;
        }

        if (e.hasComponent<ChestComponent>() && e.hasComponent<RenderComponent>())
        {
            auto& ren{ em.getComponent<RenderComponent>(e) };
            auto& chest{ em.getComponent<ChestComponent>(e) };
            if (chest.showButton)
            {
                engine.drawText("E",
                    static_cast<int>(engine.getWorldToScreenX(ren.position) - 5),
                    static_cast<int>(engine.getWorldToScreenY(ren.position) - ren.scale.y() * 50),
                    20,
                    BLACK);
            }
        }

        if (debugphy && e.hasComponent<LifeComponent>() && em.getComponent<RenderComponent>(e).visible)
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            auto const& l{ em.getComponent<LifeComponent>(e) };

            engine.drawText(std::to_string(l.life).c_str(),
                static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
                static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 50),
                20,
                BLACK);

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

        if (debugphy && e.hasComponent<ColliderComponent>())
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

        if (debugphy && e.hasComponent<PhysicsComponent>() && e.hasComponent<ColliderComponent>() && e.hasComponent<RenderComponent>())
        {
            auto& phy = em.getComponent<PhysicsComponent>(e);

            RayCast ray = engine.getMouseRay();

            auto& col = em.getComponent<ColliderComponent>(e);
            auto& ren = em.getComponent<RenderComponent>(e);
            // Comprobar si el rayo intersecta con el collider
            if (col.boundingBox.intersectsRay(ray.origin, ray.direction) && !(col.behaviorType & BehaviorType::ZONE))
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
        // Dibujar zona para mostrar ejemplo de uso del eventmanager
        // auto& linfo = em.getSingleton<LevelInfo>();
        // if (linfo.drawzone == true) {
        //     engine.drawText("CAMBIANDO DE \n ZONA...", 600, 10, 25, RED);
        //     if (linfo.segundos == 0) {
        //         linfo.drawzone = false;
        //         linfo.segundos = 1000;
        //     }
        //     linfo.segundos--;
        // }
        // else {
        //     engine.drawText(("ZONA " + std::to_string(linfo.num_zone)).c_str(), 600, 10, 50, RED);
        // }

        // Dibujar el ID de las entidades // DEBUG
        // if (debug)
        // {
        //     auto const& r{ em.getComponent<RenderComponent>(e) };
        //     engine.drawText(std::to_string(e.getID()).c_str(),
        //         static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
        //         static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 50),
        //         20,
        //         BLACK);
        // // }
    }

    std::size_t enemyID = li.max;
    Color color;

    if (li.lockedEnemy != li.max)
    {
        enemyID = li.lockedEnemy;
        color = WHITE;
    }
    else if (li.closestEnemy != li.max)
    {
        enemyID = li.closestEnemy;
        color = { 200, 200, 200, 100 };
    }

    if (enemyID != li.max)
    {
        auto& enemy = *em.getEntityByID(enemyID);
        if (enemy.hasComponent<RenderComponent>())
        {
            auto& r = em.getComponent<RenderComponent>(enemy);

            engine.drawCircle(static_cast<int>(engine.getWorldToScreenX(r.position)),
                static_cast<int>(engine.getWorldToScreenY(r.position)),
                5,
                color);
        }
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

void RenderSystem::drawHealthBar(ENGI::GameEngine& engine, EntityManager& em, const Entity& e)
{
    auto const& l{ em.getComponent<LifeComponent>(e) };

    // Datos de la barra de vida
    int barWidth = 40;
    int barHeight = 20;
    int barX = 10;
    int barY = 10;
    int spacing = 4;

    // Rectángulo de fondo para la barra de vida
    engine.drawRectangle(barX - 3, barY - 2, (barWidth + spacing) * l.maxLife + 2, barHeight + 4, DARKGRAY);

    // Dibujamos cada parte de la barra de vida
    for (int i = 0; i < l.life; ++i)
    {
        // Posición X de cada trozo
        int currentX = barX + i * (barWidth + spacing);

        // Dibujamos el rectángulo
        engine.drawRectangle(currentX, barY, barWidth, barHeight, RED);
    }
}

void RenderSystem::drawCoinBar(ENGI::GameEngine& engine, EntityManager& em)
{
    // Barra para las monedas monedas
    engine.drawRectangle(650, 540, 100, 20, DARKGRAY);

    auto const& plfi{ em.getSingleton<PlayerInfo>() };
    std::string info_text = std::to_string(plfi.coins);

    int posX = 735;

    // Sacamos el número de dígitos que tiene el número de monedas
    int num_digits = 0;
    int temp = plfi.coins;
    while (temp > 0)
    {
        temp /= 10;
        num_digits++;

        if (num_digits > 1)
            posX -= 10;
    }

    engine.drawText(info_text.c_str(), posX, 541, 18, YELLOW);
}

void RenderSystem::drawManaBar(ENGI::GameEngine& engine, EntityManager& em)
{
    auto& plfi{ em.getSingleton<PlayerInfo>() };

    if (plfi.mana > plfi.max_mana)
        plfi.mana = plfi.max_mana - 2;

    // Datos para la barra para el maná
    int barWidth = static_cast<int>(plfi.max_mana) * 2;
    int barHeight = 20;
    int barX = 7;
    int barY = 30;

    engine.drawRectangle(barX, barY, barWidth + 6, barHeight, DARKGRAY);

    int manaWidth = static_cast<int>(static_cast<float>(barWidth) * (static_cast<float>(plfi.mana) / static_cast<float>(plfi.max_mana)));

    // Interpolación
    if (plfi.mana != plfi.max_mana)
        manaWidth = plfi.mana_width + static_cast<int>((static_cast<float>(manaWidth) - static_cast<float>(plfi.mana_width)) * 0.175f);

    // Dibujamos la barra de maná
    engine.drawRectangle(barX + 3, barY + 3, manaWidth, barHeight - 6, SKYBLUE);

    plfi.mana_width = manaWidth;
}
