#include "render_system.hpp"
#include <iomanip>
//#define RAYGUI_IMPLEMENTATION
#include "../../libs/raygui.h"

void RenderSystem::init()
{
    // Tamaño de la fuente
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    // Alineamiento del texto
    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    // Color de la fuente de texto
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x000000ff);

    // Fondo de los botones
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0xD6A8E6FF);

    // Color de los bordes
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, 0x000000FF);

    // Hacemos que GuiDrawText() pueda tener más de una línea
    GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, 2);
}

void RenderSystem::update(EntityManager& em, GameEngine& engine)
{
    // Actualizamos la posicion de render del componente de fisicas
    em.forEach<SYSCMPs, SYSTAGs>([](Entity& e, PhysicsComponent& phy, RenderComponent& ren)
    {
        if (e.hasTag<SeparateModelTag>())
            return;
        ren.setPosition(phy.position);
        ren.setOrientation(phy.orientation);
        ren.setScale(phy.scale);
        // ren.updateBBox();
    });

    // Empezamos el frame
    beginFrame(engine, em);

    // Dibuja todas las entidades con componente de render
    drawEntities(em, engine);

    // Terminamos el frame
    endFrame(engine, em);
}

void RenderSystem::drawLogoGame(GameEngine& engine, EntityManager& em, SoundSystem& ss) {
    ss.ambient_stop();
    ss.ambient_started = false;

    engine.beginDrawing();
    engine.clearBackground(WHITE);
    // Logo del videojuego
    engine.textures["logo_twowonders"].width = static_cast<int>(engine.getScreenWidth() / 1.3);
    engine.textures["logo_twowonders"].height = static_cast<int>(engine.getScreenHeight() / 1.5);
    engine.drawTexture(engine.textures["logo_twowonders"],
        engine.getScreenWidth() / 2 - engine.textures["logo_twowonders"].width / 2,
        static_cast<int>(engine.getScreenHeight() / 2.0 - engine.textures["logo_twowonders"].height / 1.5),
        { 255, 255, 255, 255 });

    // Datos de los botones
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float posX = static_cast<float>(engine.getScreenWidth() / 2) - (buttonWidth / 2.f);
    float posY = static_cast<float>(engine.getScreenHeight() / 1.25) - (buttonHeight / 2.f);

    // Funcionalidad de botones
    Rectangle btn1Rec = { posX, posY, buttonWidth, buttonHeight };
    Rectangle btn2Rec = { posX, posY + 70, buttonWidth, buttonHeight };

    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();

    // Define the current button selection
    auto& currentButton = inpi.currentButton;

    // Get the gamepad input
    if (engine.isGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) { // A on Xbox, X on PlayStation
        // Press the currently selected button
        switch (currentButton) {
        case 0: // "JUGAR"
            li.currentScreen = GameScreen::STORY;
            ss.seleccion_menu();
            ss.music_stop();
            break;
        case 1: // "CONFIGURACION"
            li.currentScreen = GameScreen::OPTIONS;
            li.previousScreen = GameScreen::TITLE;
            ss.seleccion_menu();
            break;
        }
    }
    else if (engine.isGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) { // D-Pad Up
        // Move the selection up
        currentButton = (currentButton > 0) ? currentButton - 1 : 1;
        ss.sonido_mov();
    }
    else if (engine.isGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) { // D-Pad Down
        // Move the selection down
        currentButton = (currentButton < 1) ? currentButton + 1 : 0;
        ss.sonido_mov();
    }

    // Draw the buttons
    if (GuiButton(btn1Rec, (currentButton == 0) ? "[JUGAR]" : "JUGAR") || li.replay) {
        li.currentScreen = GameScreen::STORY;
        ss.seleccion_menu();
        ss.music_stop();
    }

    if (!li.replay)
    {
        auto& gami = em.getSingleton<GameData>();
        gami.updateFrameDown();
    }

    if (engine.checkCollisionPointRec(GetMousePosition(), btn1Rec) || engine.checkCollisionPointRec(GetMousePosition(), btn2Rec)) {
        if (ss.pushed == false)
            ss.sonido_mov();
        ss.pushed = true;
    }
    else
        ss.pushed = false;

    if (GuiButton(btn2Rec, (currentButton == 1) ? "[CONFIGURACION]" : "CONFIGURACION")) {
        li.currentScreen = GameScreen::OPTIONS;
        li.previousScreen = GameScreen::TITLE;
        ss.seleccion_menu();
    }

    engine.endDrawing();
}

void RenderSystem::drawChargeScreen(GameEngine& engine, EntityManager& em)
{
    engine.beginDrawing();
    engine.clearBackground({ 171, 159, 197, 255 });
    GameEngine::Gif& gif = engine.gifs.at("carga");
    Texture2D gifCopy = gif.texture;

    // Redimensionamos la copia
    gifCopy.width = static_cast<int>(gifCopy.width / 2.0);
    gifCopy.height = static_cast<int>(gifCopy.height / 2.0);

    int posX = static_cast<int>(engine.getScreenWidth() / 2 - gifCopy.width / 2);
    int posY = static_cast<int>(engine.getScreenHeight() / 2 - gifCopy.height / 2);

    displayGif(engine, gifCopy, gif, posX, posY);

    auto& li = em.getSingleton<LevelInfo>();
    li.loadingTime += timeStep;


    engine.endDrawing();
}

void RenderSystem::drawOptions(GameEngine& engine, EntityManager& em, SoundSystem& ss) {
    engine.beginDrawing();
    engine.clearBackground(WHITE);
    auto& li = em.getSingleton<LevelInfo>();

    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;

    // Slider del volumen
    Rectangle volumenSlider = { 100, 100, 200, 20 };
    float volumen = 50;
    float nuevoValor = static_cast<float>(GuiSliderBar(volumenSlider, "Volumen", NULL, &volumen, 0, 100));

    // Ahora asignamos el nuevo valor al puntero volumen
    volumen = nuevoValor;

    // Posición del botón de volver
    float posX = static_cast<float>(engine.getScreenWidth() / 2) - (buttonWidth / 2);
    float posY = static_cast<float>(engine.getScreenHeight() / 2) - (buttonHeight / .5f);

    // Botones de resolución
    float posResX = static_cast<float>(100) - (buttonWidth / 2);
    float posResY = static_cast<float>(200) - (buttonHeight / 2);

    // Boton de volver al inicio
    Rectangle btn1Rec = { posX, posY, buttonWidth, buttonHeight };

    // Botones de resolución
    Rectangle btn2Rec = { posResX, posResY, buttonWidth, buttonHeight };
    Rectangle btn3Rec = { posResX + 250, posResY, buttonWidth, buttonHeight };
    Rectangle btn4Rec = { posResX + 500, posResY, buttonWidth, buttonHeight };
    Rectangle btn5Rec = { posResX + 750, posResY, buttonWidth, buttonHeight };

    if (GuiButton(btn1Rec, "VOLVER")) {
        li.currentScreen = li.previousScreen;
        ss.seleccion_menu();
    }

    if (GuiButton(btn2Rec, "800x600"))
    {
        engine.setWindowSize(800, 600);
        ss.seleccion_menu();
    }

    if (GuiButton(btn3Rec, "1280x720"))
    {
        engine.setWindowSize(1280, 720);
        ss.seleccion_menu();
    }

    if (GuiButton(btn4Rec, "1920x1080"))
    {
        engine.setWindowSize(1920, 1080);
        ss.seleccion_menu();
    }

    if (fullScreen)
    {
        engine.setWindowFullScreen();
        fullScreen = false;
    }

    if (GuiButton(btn5Rec, "FULLSCREEN"))
    {
        engine.setWindowSize(1920, 1080);
        fullScreen = true;
        ss.seleccion_menu();
    }

    if (engine.checkCollisionPointRec(GetMousePosition(), btn1Rec) || engine.checkCollisionPointRec(GetMousePosition(), btn2Rec) || engine.checkCollisionPointRec(GetMousePosition(), btn3Rec) || engine.checkCollisionPointRec(GetMousePosition(), btn4Rec) || engine.checkCollisionPointRec(GetMousePosition(), btn5Rec)) {
        if (ss.pushed == false)
            ss.sonido_mov();
        ss.pushed = true;
    }
    else
        ss.pushed = false;

    engine.endDrawing();
}

void RenderSystem::drawPauseMenu(GameEngine& engine, EntityManager& em)
{
    auto& ss = em.getSingleton<SoundSystem>();

    float windowWidth = 330.0f;
    float windowHeight = 460.0f;
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;

    Rectangle windowRect = {
        static_cast<float>(engine.getScreenWidth()) / 2.0f - windowWidth / 2.0f,
        static_cast<float>(engine.getScreenHeight()) / 2.0f - windowHeight / 2.0f,
        windowWidth,
        windowHeight
    };
    engine.drawRectangleLinesEx(windowRect, 2, BLACK);
    engine.drawRectangleRec(windowRect, Color{ 255, 255, 255, 178 });
    engine.drawTextEx(engine.getFontDefault(), "PAUSA", vec2d{ windowRect.x + 100, windowRect.y + 40 }, 40, 1, BLACK);

    float posX = static_cast<float>(engine.getScreenWidth() / 2) - (buttonWidth / 2.0f);
    float posY = static_cast<float>(engine.getScreenHeight() / 2) - (buttonHeight / .5f);

    // Boton de volver al inicio
    Rectangle btn1Rec = { posX, posY, buttonWidth, buttonHeight };
    Rectangle btn2Rec = { posX, posY + 70, buttonWidth, buttonHeight };
    Rectangle btn3Rec = { posX, posY + 140, buttonWidth, buttonHeight };
    Rectangle btn4Rec = { posX, posY + 210, buttonWidth, buttonHeight };

    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();

    // Define the current button selection
    auto& currentButton = inpi.currentButton;

    // Get the gamepad input
    if (engine.isGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)
        || inpi.mouseClick) {
        // Press the currently selected button
        switch (currentButton) {
        case 0: // "CONTINUAR"
        {
            inpi.pause = false;
            ss.seleccion_menu();
            break;
        }
        case 1: // "OPCIONES"
        {
            li.currentScreen = GameScreen::OPTIONS;
            li.previousScreen = GameScreen::GAMEPLAY;
            ss.seleccion_menu();
            break;
        }
        case 2: // "VOLVER AL INICIO"
        {
            li.currentScreen = GameScreen::TITLE;
            ss.seleccion_menu();
            break;
        }
        case 3: // "SALIR"
        {
            ss.sonido_salir();
            li.gameShouldEnd = true;
            return;
        }
        }

        currentButton = 0;
        inpi.mouseClick = false;
    }
    else if (engine.isGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) { // D-Pad Up
        // Move the selection up
        currentButton = (currentButton > 0) ? currentButton - 1 : 3;
        ss.sonido_mov();
    }
    else if (engine.isGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) { // D-Pad Down
        // Move the selection down
        currentButton = (currentButton < 3) ? currentButton + 1 : 0;
        ss.sonido_mov();
    }

    // Draw the buttons
    if (GuiButton(btn1Rec, (currentButton == 0) ? "[CONTINUAR]" : "CONTINUAR"))
    {
        currentButton = 0;
        inpi.mouseClick = true;
    }
    if (GuiButton(btn2Rec, (currentButton == 1) ? "[OPCIONES]" : "OPCIONES"))
    {
        currentButton = 1;
        inpi.mouseClick = true;
    }
    if (GuiButton(btn3Rec, (currentButton == 2) ? "[VOLVER AL INICIO]" : "VOLVER AL INICIO"))
    {
        currentButton = 2;
        inpi.mouseClick = true;
    }
    if (GuiButton(btn4Rec, (currentButton == 3) ? "[SALIR]" : "SALIR"))
    {
        currentButton = 3;
        inpi.mouseClick = true;
    }

    if (engine.checkCollisionPointRec(GetMousePosition(), btn1Rec) || engine.checkCollisionPointRec(GetMousePosition(), btn2Rec) || engine.checkCollisionPointRec(GetMousePosition(), btn3Rec) || engine.checkCollisionPointRec(GetMousePosition(), btn4Rec)) {
        if (ss.pushed == false)
            ss.sonido_mov();
        ss.pushed = true;
    }
    else
        ss.pushed = false;
}

void RenderSystem::drawInventory(GameEngine& engine, EntityManager& em)
{
    float windowWidth = 450.0f;
    float windowHeight = 450.0f;
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float posButtonX = static_cast<float>(engine.getScreenWidth() / 2) - (buttonWidth / 2);
    float posButtonY = static_cast<float>(engine.getScreenHeight() / 2) - (buttonHeight / 2);
    float posX = static_cast<float>(engine.getScreenWidth() / 2) - (windowWidth / 2);
    float posY = static_cast<float>(engine.getScreenHeight() / 2) - (windowHeight / 2);
    float augment = 55.f;

    Rectangle windowRect = {
        posX,
        posY,
        windowWidth,
        windowHeight
    };
    engine.drawRectangleLinesEx(windowRect, 2, BLACK);
    engine.drawRectangleRec(windowRect, Color{ 255, 255, 255, 178 });
    engine.drawTextEx(engine.getFontDefault(), "INVENTARIO", vec2d{ windowRect.x + 110, windowRect.y + 20 }, 40, 1, BLACK);

    auto& plfi = em.getSingleton<PlayerInfo>();
    auto& inpi = em.getSingleton<InputInfo>();

    auto& currentButton = inpi.currentButton;

    if (plfi.selectedItem == plfi.max)
    {
        // Get the gamepad input
        if (engine.isGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
            // Press the currently selected button
            if (static_cast<std::size_t>(currentButton) < plfi.inventory.size()) {
                // Select an item from the inventory
                plfi.selectedItem = plfi.inventory[currentButton]->getID();
                currentButton = 0;
                return;
            }
            else {
                // "VOLVER" button
                auto& inpi = em.getSingleton<InputInfo>();
                inpi.inventory = false;
            }
        }
        else if (engine.isGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
            // Move the selection up
            currentButton = (currentButton > 0) ? currentButton - 1 : static_cast<int>(plfi.inventory.size());
        }
        else if (engine.isGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
            // Move the selection down
            currentButton = (currentButton < static_cast<int>(plfi.inventory.size())) ? currentButton + 1 : 0;
        }
        int buttonIndex = 0;
        for (auto& item : plfi.inventory)
        {
            Rectangle btnRec = { posButtonX, posButtonY + 50, buttonWidth, buttonHeight };
            if (GuiButton(btnRec, (buttonIndex == currentButton) ? ("[" + item->name + "]").c_str() : item->name.c_str())) {
                plfi.selectedItem = item->getID();
                currentButton = 0;
                return;
            }
            posButtonY += augment;
            buttonIndex++;
        }

        // Botón de volver al juego
        Rectangle btn2Rec = { posButtonX, posButtonY + 70, buttonWidth, buttonHeight };
        if (GuiButton(btn2Rec, (currentButton == static_cast<int>(plfi.inventory.size())) ? "[VOLVER]" : "VOLVER")) {
            auto& inpi = em.getSingleton<InputInfo>();
            inpi.inventory = false;
        }
    }
    else
    {
        // Dibujamos la descripción del objeto seleccionado
        auto& item = *plfi.getItem(plfi.selectedItem);
        auto text = const_cast<char*>(item.description.c_str());

        GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

        float descWidth = 300.f, descHeight = 150.f;
        float posDescX = static_cast<float>(engine.getScreenWidth() / 2) - (descWidth / 2.0f);
        float posDescY = static_cast<float>(engine.getScreenHeight() / 2) - (descHeight / 1.25f);
        GuiTextBox({ posDescX, posDescY, descWidth, descHeight }, text, static_cast<int>(item.description.size()), false);

        if (engine.isGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
            // Press the currently selected button
            switch (currentButton) {
            case 0: // "USAR"
                if (dynamic_cast<Potion*>(&item) != nullptr) {
                    auto& potion = static_cast<Potion&>(item);
                    plfi.usePotion(potion);
                    plfi.selectedItem = plfi.max;
                    auto& inpi = em.getSingleton<InputInfo>();
                    inpi.inventory = false;
                    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
                    return;
                }
                break;
            case 1: // "VOLVER"
                plfi.selectedItem = plfi.max;
                GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
                break;
            }
        }
        else if (engine.isGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
            // Move the selection up
            currentButton = (currentButton > 0) ? currentButton - 1 : 1;
        }
        else if (engine.isGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
            // Move the selection down
            currentButton = (currentButton < 1) ? currentButton + 1 : 0;
        }

        if (dynamic_cast<Potion*>(&item) != nullptr) {
            Rectangle btn1Rec = { posButtonX, posButtonY + 100, buttonWidth, buttonHeight };
            if (GuiButton(btn1Rec, (currentButton == 0) ? "[USAR]" : "USAR")) {
                auto& potion = static_cast<Potion&>(item);
                plfi.usePotion(potion);
                plfi.selectedItem = plfi.max;
                auto& inpi = em.getSingleton<InputInfo>();
                inpi.inventory = false;
                GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
            }
        }

        // Boton de volver al inventario
        Rectangle btn2Rec = { posButtonX, posButtonY + 170, buttonWidth, buttonHeight };
        if (GuiButton(btn2Rec, (currentButton == 1) ? "[VOLVER]" : "VOLVER")) {
            plfi.selectedItem = plfi.max;
            GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        }
    }
}

void RenderSystem::drawLogoKaiwa(GameEngine& engine) {
    engine.beginDrawing();
    engine.clearBackground(Color({ 136, 219, 152, 255 }));
    engine.textures["logo_kaiwagames"].width = engine.getScreenWidth();
    engine.textures["logo_kaiwagames"].height = static_cast<int>(engine.getScreenHeight() / 1.6);
    engine.drawTexture(engine.textures["logo_kaiwagames"],
        engine.getScreenWidth() / 2 - engine.textures["logo_kaiwagames"].width / 2,
        engine.getScreenHeight() / 2 - engine.textures["logo_kaiwagames"].height / 2,
        WHITE);
    engine.endDrawing();
}

void RenderSystem::drawEnding(GameEngine& engine) {
    engine.beginDrawing();
    engine.clearBackground(WHITE);

    // Valores de la caja de texto
    float boxWidth = 600.f;
    float boxHeight = 100.f;
    float posX = static_cast<float>(engine.getScreenWidth() / 2) - (boxWidth / 2.f);
    float posY = static_cast<float>(engine.getScreenHeight() / 2) - (boxHeight / 2.f);

    std::string text = "[ENTER] PARA VOLVER AL TÍTULO";

    if (engine.isGamepadAvailable(0))
        text = "[X] PARA VOLVER AL TÍTULO";

    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    GuiLabelButton(Rectangle{ posX, posY, boxWidth, boxHeight },
        "¡Gracias por jugar a nuestra demo!");

    GuiLabelButton(Rectangle{ posX, posY + 50, boxWidth + 100, boxHeight },
        text.c_str());

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    engine.endDrawing();
}

void RenderSystem::drawStory(GameEngine& engine) {
    engine.beginDrawing();
    engine.clearBackground(WHITE);
    float boxWidth = 700.f;
    float boxHeight = 400.f;
    float posX = static_cast<float>(engine.getScreenWidth() / 2) - (boxWidth / 2);
    float posY = static_cast<float>(engine.getScreenHeight() / 2.5) - (boxHeight / 2);

    Rectangle boxRect = { posX, posY, boxWidth, boxHeight };

    // Tamaño de la fuente
    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);

    // Alineamiento del texto
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    GuiLabel(boxRect, "¡Bienvenido a la aventura!");
    GuiLabel({ posX, posY + 50, boxWidth, boxHeight }, "Estas perdido por el bosque y");
    GuiLabel({ posX, posY + 100, boxWidth, boxHeight }, "debes encontrar a tu maestro.");
    GuiLabel({ posX, posY + 150, boxWidth, boxHeight }, "¡Mucha suerte!");

    std::string text = "PRESS [ENTER] TO PLAY";
    if (engine.isGamepadAvailable(0))
        text = "PRESS [X] TO PLAY";
    GuiLabel({ posX, posY + 250, boxWidth, boxHeight }, text.c_str());

    init();
    engine.endDrawing();
}

void RenderSystem::drawEntities(EntityManager& em, GameEngine& engine)
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

                case ElementalType::Water:
                    colorEntidad = BLUE;
                    break;

                case ElementalType::Fire:
                    colorEntidad = RED;
                    break;

                case ElementalType::Ice:
                    colorEntidad = SKYBLUE;
                    break;

                default:
                    break;
                }
            }

            if (e.hasTag<SubjectTag>() && e.hasComponent<SubjectComponent>() && em.getComponent<SubjectComponent>(e).activeShield)
                colorEntidad = GREEN;

            if (e.hasComponent<LifeComponent>()) {
                auto& l{ em.getComponent<LifeComponent>(e) };
                if (l.elapsed < l.countdown)
                {
                    colorEntidad = MAROON;

                    if (e.hasTag<DestructibleTag>())
                        r.position.setZ(shakeDouble(r.position.z()));
                }
            }
            if (!e.hasTag<ZoneTag>())
            {
                vec3d scl = { 1.0, 1.0, 1.0 };
                vec3d pos = r.position;
                // Solo generamos la malla si no existe
                if (!r.meshLoaded)
                    loadModels(e, engine, em, r);

                auto& li = em.getSingleton<LevelInfo>();
                if (li.isCharging())
                    return;

                bool in{ false };
                if (e.hasTag<PlayerTag>())
                {
                    // scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 1.8);
                    in = true;
                }
                else if (e.hasTag<SlimeTag>())
                {
                    //scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - .6);
                    in = true;
                }
                else if (e.hasTag<SnowmanTag>())
                {
                    // scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 1.1);
                    in = true;
                }
                else if (e.hasTag<SpiderTag>())
                {
                    // scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 0.7);
                    in = true;
                }
                else if (e.hasTag<GolemTag>())
                {
                    // scl = { 0.4, 0.4, 0.4 };
                    pos.setY(pos.y() - 4.0);
                    in = true;
                }
                else if (e.hasTag<DummyTag>())
                {
                    // scl = { 0.4, 0.4, 0.4 };
                    pos.setY(pos.y() - 4.0);
                    in = true;
                }
                else if (e.hasTag<BossFinalTag>())
                {
                    scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 1.1);
                    colorEntidad = { 125, 125, 125, 255 };
                    in = true;
                }
                else if (e.hasTag<SubjectTag>())
                {
                    scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 1.1);
                    in = true;
                }
                else if (e.hasTag<CrusherTag>())
                {
                    // scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 4.8);
                    in = true;
                }
                else if (e.hasTag<AngryBushTag>())
                {
                    // scl = { 0.33, 0.33, 0.33 };
                    // pos.setY(pos.y() - 0.5);
                    in = true;
                }
                else if (e.hasTag<AngryBushTag2>())
                {
                    //pos.setY(pos.y() - 0.5);
                    in = true;
                }
                else if (e.hasTag<ChestTag>() || e.hasTag<SpawnTag>() || e.hasTag<LavaTag>() || e.hasTag<SignTag>() || e.hasTag<TableTag>() || e.hasTag<MissionObjTag>())
                {
                    pos.setY(pos.y() - r.offset);
                    in = true;
                }
                else if (e.hasTag<DestructibleTag>())
                {
                    pos.setY(pos.y() - r.offset / 1.5);
                    in = true;
                }
                else if (e.hasTag<NomadTag>() || e.hasTag<InvestigatorTag>())
                {
                    pos.setY(pos.y() - 3.54);
                    in = true;
                }
                else if (e.hasTag<DoorTag>() || e.hasTag<LeverTag>()
                    || e.hasTag<CoinTag>() || e.hasTag<WaterBombTag>() || e.hasTag<BoatTag>())
                {
                    in = true;
                }

                if (r.rotationVec == vec3d::zero())
                    r.rotationVec = { 0.0, -1.0, 0.0 };

                float orientationInDegrees = static_cast<float>(r.orientation * (180.0f / K_PI));
                engine.drawModel(r.model, pos, r.rotationVec, orientationInDegrees, scl, colorEntidad);

                if (!in)
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

void RenderSystem::loadModels(Entity& e, GameEngine& engine, EntityManager& em, RenderComponent& r)
{
    auto& li = em.getSingleton<LevelInfo>();

    if (e.hasTag<PlayerTag>())
    {
        r.model = engine.loadModel("assets/models/main_character.obj");
        Texture2D t0 = engine.loadTexture("assets/models/textures/entity_textures/main_character_uv_V2.png");
        Texture2D t = engine.loadTexture("assets/models/textures/entity_textures/main_character_texture_V2.png");
        r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
        r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
    }
    else if (e.hasTag<SlimeTag>())
    {
        r.model = engine.loadModel("assets/models/Slime.obj");
        // Texture2D t0 = engine.loadTexture("assets/models/textures/entity_textures/Slime_uv.png");
        // Texture2D t = engine.loadTexture("assets/models/textures/entity_textures/Slime_texture.png");
        // r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
        // r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
    }
    else if (e.hasTag<SnowmanTag>())
    {
        r.model = engine.loadModel("assets/models/snowman.obj");
        Texture2D t0 = engine.loadTexture("assets/models/textures/entity_textures/snowman_uv.png");
        Texture2D t = engine.loadTexture("assets/models/textures/entity_textures/snowman_texture.png");
        r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
        r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
    }
    else if (e.hasTag<GolemTag>())
    {
        r.model = engine.loadModel("assets/models/Golem.obj");
        loadShaders(r.model);
    }
    else if (e.hasTag<SpiderTag>())
    {
        r.model = engine.loadModel("assets/models/Spider.obj");
        Texture2D t0 = engine.loadTexture("assets/models/textures/entity_textures/Spider_UV.png");
        Texture2D t = engine.loadTexture("assets/models/textures/entity_textures/Spider_texture.png");
        r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
        r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
    }
    else if (e.hasTag<BossFinalTag>())
    {
        r.model = engine.loadModel("assets/models/Boss.obj");
        Texture2D t0 = engine.loadTexture("assets/models/textures/entity_textures/Boss_uv.png");
        Texture2D t = engine.loadTexture("assets/models/textures/entity_textures/Boss_texture.png");
        r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
        r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
    }
    else if (e.hasTag<SubjectTag>())
    {
        r.model = engine.loadModel("assets/models/Boss_sub_1.obj");
        Texture2D t0 = engine.loadTexture("assets/models/textures/entity_textures/Boss_sub_1_uv.png");
        Texture2D t = engine.loadTexture("assets/models/textures/entity_textures/Boss_sub_1_texture.png");

        r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;
        r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
    }
    else if (e.hasTag<Chunk0Tag>())
    {
        switch (li.mapID)
        {
        case 0:
            r.model = engine.loadModel("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_0.obj");
            break;

        case 1:
            r.model = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_0.obj");
            break;
        case 2:
            r.model = engine.loadModel("assets/levels/Zona_2-Volcan/objs/lvl_2-cnk_0.obj");
            break;
        }

        loadShaders(r.model);
    }
    else if (e.hasTag<Chunk1Tag>())
    {
        switch (li.mapID)
        {
        case 0:
            r.model = engine.loadModel("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_1.obj");
            break;

        case 1:
            r.model = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_1.obj");
            break;
        case 2:
            r.model = engine.loadModel("assets/levels/Zona_2-Volcan/objs/lvl_2-cnk_1.obj");
            break;
        }
        loadShaders(r.model);
    }
    else if (e.hasTag<Chunk2Tag>())
    {
        switch (li.mapID)
        {
        case 0:
            r.model = engine.loadModel("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_2.obj");
            break;

        case 1:
            r.model = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_2.obj");
            break;
        case 2:
            r.model = engine.loadModel("assets/levels/Zona_2-Volcan/objs/lvl_2-cnk_2.obj");
            break;
        }
        loadShaders(r.model);
    }
    else if (e.hasTag<Chunk3Tag>())
    {
        switch (li.mapID)
        {
        case 0:
            r.model = engine.loadModel("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_3.obj");
            break;

        case 1:
            r.model = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_3.obj");
            break;
        case 2:
            r.model = engine.loadModel("assets/levels/Zona_2-Volcan/objs/lvl_2-cnk_3.obj");
            break;
        }
        loadShaders(r.model);
    }
    else if (e.hasTag<Chunk4Tag>())
    {
        switch (li.mapID)
        {
        case 0:
            r.model = engine.loadModel("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_4.obj");
            break;

        case 1:
            r.model = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_4.obj");
            break;
        }
        loadShaders(r.model);
    }
    else if (e.hasTag<Chunk5Tag>())
    {
        switch (li.mapID)
        {
        case 0:
            r.model = engine.loadModel("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_5.obj");
            break;

        case 1:
            r.model = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_5.obj");
            break;
        }
        loadShaders(r.model);
    }
    else if (e.hasTag<Chunk6Tag>())
    {
        switch (li.mapID)
        {
        case 0:
            r.model = engine.loadModel("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_6.obj");
            break;

        case 1:
            r.model = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_6.obj");
            break;
        }
        loadShaders(r.model);
    }
    else if (e.hasTag<ChestTag>())
    {
        r.model = engine.loadModel("assets/models/Cofre.obj");
        loadShaders(r.model);
    }
    else if (e.hasTag<DestructibleTag>())
    {
        switch (li.mapID)
        {
        case 0:
            r.model = engine.loadModel("assets/models/Troncos.obj");
            break;

        case 1:
            r.model = engine.loadModel("assets/models/Puerta_prision_agua.obj");
            break;
        }
        loadShaders(r.model);
    }
    else if (e.hasTag<DoorTag>())
    {
        switch (li.mapID)
        {
        case 0:
            r.model = engine.loadModel("assets/levels/Zona_0-Bosque/objs/Barricada_cambio_lvl.obj");
            break;

        case 1:
            r.model = engine.loadModel("assets/models/Puerta_prision_base.obj");
            break;
        }

        loadShaders(r.model);
    }
    else if (e.hasTag<AngryBushTag>())
    {
        r.model = engine.loadModel("assets/models/Piedra.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<AngryBushTag2>())
    {
        r.model = engine.loadModel("assets/models/PiedraV2.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<CrusherTag>())
    {
        r.model = engine.loadModel("assets/models/Apisonadora.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<DummyTag>())
    {
        r.model = engine.loadModel("assets/models/Dummy.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<BarricadeTag>())
    {
        r.model = engine.loadModel("assets/Assets/Barricada_arboles/Barricada_arboles.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<SpawnTag>())
    {
        r.model = engine.loadModel("assets/Assets/Checkpoint/Checkpoint.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<LevelChangeTag>())
    {
        r.model = engine.loadModel("assets/Assets/Tp/Tp.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<LeverTag>())
    {
        r.model = engine.loadModel("assets/Assets/Palanca/Palanca-prision.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<CoinTag>())
    {
        r.model = engine.loadModel("assets/Assets/Props/Destellos.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<WaterBombTag>())
    {
        r.model = engine.loadModel("assets/Assets/Props/Hechizos/Agua_1.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<NomadTag>())
    {
        r.model = engine.loadModel("assets/Personajes/NPCs/Nomada/Nomada.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<InvestigatorTag>())
    {
        r.model = engine.loadModel("assets/Personajes/NPCs/Investigador/Investigador.obj");
        loadShaders(r.model);
    }
    else if (e.hasTag<LavaTag>())
    {
        r.model = engine.loadModel("assets/Assets/Charco_lava/Charco_lava.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<SignTag>())
    {
        r.model = engine.loadModel("assets/Assets/Cartel/Cartel.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<TableTag>())
    {
        r.model = engine.loadModel("assets/Assets/Mesa_investigador/Mesa-investigador.obj");

        loadShaders(r.model);
    }
    else if (e.hasTag<MissionObjTag>())
    {
        switch (li.mapID)
        {
        case 2:
        {
            if (e.hasComponent<BoatComponent>())
            {
                auto& bc = em.getComponent<BoatComponent>(e);

                switch (bc.part)
                {
                case BoatParts::Base:
                {
                    r.model = engine.loadModel("assets/Assets/Barca/Barca_base.obj");
                    break;
                }
                case BoatParts::Motor:
                {
                    r.model = engine.loadModel("assets/Assets/Barca/Barca_motor.obj");
                    break;
                }
                case BoatParts::SteeringWheel:
                {
                    r.model = engine.loadModel("assets/Assets/Barca/Barca_volante.obj");
                    break;
                }
                case BoatParts::Propeller:
                {
                    r.model = engine.loadModel("assets/Assets/Barca/Barca_helice.obj");
                    break;
                }
                }
            }
        }

        default:
            break;
        }

        loadShaders(r.model);
    }
    else if (e.hasTag<BoatTag>())
    {
        r.model = engine.loadModel("assets/Assets/Barca/Barca_completa.obj");

        loadShaders(r.model);
    }
    else
    {
        r.mesh = engine.genMeshCube(static_cast<float>(r.scale.x()), static_cast<float>(r.scale.y()), static_cast<float>(r.scale.z()));
        r.model = engine.loadModelFromMesh(r.mesh);
    }
    r.meshLoaded = true;
}

void RenderSystem::loadShaders(Model& model)
{
    for (int i = 0; i < model.materialCount; i++)
    {
        model.materials[i].shader = *shaderPtr;
    }

}

// Empieza el dibujado y se limpia la pantalla
void RenderSystem::beginFrame(GameEngine& engine, EntityManager& em)
{
    engine.beginDrawing();

    auto& li = em.getSingleton<LevelInfo>();

    Color bgColor = RAYWHITE;
    switch (li.mapID)
    {
    case 0:
        bgColor = RAYWHITE;
        break;
    case 1:
        bgColor = { 103, 49, 71, 255 };
        break;
    }

    engine.clearBackground(bgColor);

    engine.beginMode3D();
    //engine.drawGrid(50, 1.f);
}
//dibujar rayo 3d
void RenderSystem::drawRay(vec3d origin, vec3d dir) {
    BeginDrawing();
    DrawLine3D(origin.toRaylib(), (origin + dir * 100).toRaylib(), RED);
    EndDrawing();
}
// Se termina el dibujado
void RenderSystem::endFrame(GameEngine& engine, EntityManager& em)
{
    engine.endMode3D();

    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();
    auto& txti = em.getSingleton<TextInfo>();

    if (li.isCharging())
        return;

    drawHUD(em, engine, inpi.debugPhy);
    drawAlerts_IA(em, engine);

    if (txti.hasText())
        drawTextBox(engine, em);

    if (inpi.pause)
        drawPauseMenu(engine, em);
    else if (inpi.pause)
        inpi.pause = false;

    else if (inpi.inventory)
        drawInventory(engine, em);

    // Si se pulsa F2 se activa editor  de parámetros In-game
    else if (inpi.debugAI1)
        drawEditorInGameIA(engine, em);

    // Visual Debug AI
    else if (inpi.debugAI2)
        drawDebuggerInGameIA(engine, em);

    else if (inpi.pathfind)
        drawTestPathfindinf(engine, em);

    engine.endDrawing();
}

//Dibuja Slider en función de los parámetros
double SelectValue(GameEngine& engine, double value, float posx, float posy, float height, float width, const char* text, float min_value, float max_value) {
    // pasamos a float el valor
    float floatvalue = static_cast<float>(value);
    // dibujamos el slider para modificar su valor
    int new_detect_radius = GuiSliderBar(Rectangle(posx, posy, height, width), text, NULL, &floatvalue, min_value, max_value);
    new_detect_radius = new_detect_radius + 1;
    engine.drawText(std::to_string(floatvalue).c_str(), 300, static_cast<int>(posy + 5.0f), 20, BLUE);
    // seteamos el nuevo valor
    return static_cast<double>(floatvalue);
}

uint16_t findNearestNode(EntityManager& em, const vec3d& position, const std::set<std::pair<uint16_t, vec3d>>& nodes) {
    uint16_t nearestNodeId = 0; // Suponemos que el primer nodo es el más cercano inicialmente
    double minDistance = std::numeric_limits<double>::max(); // Inicializamos la distancia mínima con un valor muy grande
    vec3d nearestpos{};
    for (const auto& node : nodes) {
        double dist = position.distance(node.second); // Calculamos la distancia entre la posición y el nodo actual
        if (dist < minDistance) { // Si encontramos un nodo más cercano
            minDistance = dist;
            nearestNodeId = node.first;
            nearestpos = node.second;
        }
    }
    auto& debug = em.getSingleton<Debug_t>();
    debug.nodes.push_back(nearestpos);
    return nearestNodeId;
}
//Interfaz para probar el pathfinding
void RenderSystem::drawTestPathfindinf(GameEngine& engine, EntityManager& em) {
    auto& debug = em.getSingleton<Debug_t>();
    auto& navs = em.getSingleton<NavmeshInfo>();
    auto& li = em.getSingleton<LevelInfo>();
    //Dibujado de titulo y ventana
    Rectangle windowRect = { static_cast<float>(engine.getScreenWidth() - 400), 300, 330, 430 };
    engine.drawRectangleLinesEx(windowRect, 2, DARKGRAY);
    engine.drawRectangleRec(windowRect, Color{ 255, 255, 255, 128 });
    vec2d textPositionInfo = { static_cast<float>(engine.getScreenWidth() - 370), 320 };
    engine.drawTextEx(GetFontDefault(), "PATHFINDING", textPositionInfo, 20, 1, RED);

    // Datos de los botones
    float buttonWidth = 150.0f;
    float buttonHeight = 30.0f;
    float posX = static_cast<float>(engine.getScreenWidth() - 370);
    float posY = 350.0f;

    // Slider para startnode
    // float startMinValue = 1.0f;
    // float startMaxValue = 100.0f;
    // const char* startNodeText = "Start Node";
    // posX = 600.0f; // Reseteamos la posición X
    // posY = 355.0f; // Posición Y para el slider de startnode
    // int startnodenew = GuiSliderBar(Rectangle(posX, posY, buttonWidth, buttonHeight), startNodeText, NULL, &debug.startnode, startMinValue, startMaxValue);
    // engine.drawText(std::to_string(static_cast<int>(debug.startnode)).c_str(), static_cast<int>(posX + 160), static_cast<int>(posY), 20, BLUE);
    // startnodenew += 1;
    // // Slider para goalnode
    // float goalMinValue = 1.0f;
    // float goalMaxValue = 100.0f;
    // const char* goalNodeText = "Goal Node";
    // int goalnodenew = GuiSliderBar(Rectangle(posX, posY + 40, buttonWidth, buttonHeight), goalNodeText, NULL, &debug.goalnode, goalMinValue, goalMaxValue);
    // engine.drawText(std::to_string(static_cast<int>(debug.goalnode)).c_str(), static_cast<int>(posX + 160), static_cast<int>(posY + 40), 20, BLUE);
    // goalnodenew += 1;

    Rectangle btn1Rec = { posX - 10, posY + 80, buttonWidth, buttonHeight };
    Rectangle btn2Rec = { posX + 140, posY + 80, buttonWidth, buttonHeight };
    Rectangle btn3Rec = { posX + 140, posY  , buttonWidth, buttonHeight };
    Rectangle btn4Rec = { posX - 10, posY  , buttonWidth, buttonHeight };
    Rectangle btn5Rec = { posX + 140, posY + 40, buttonWidth, buttonHeight };
    Rectangle btn6Rec = { posX - 10, posY + 40, buttonWidth, buttonHeight };
    // Botón
    if (GuiButton(btn1Rec, "CALCULATE")) {
        //std::size_t idcenter{};
        // if(em.getEntityByID(li.playerID)->hasComponent<ColliderComponent>()){
        //     auto& playerbbox = em.getComponent<ColliderComponent>(*em.getEntityByID(li.playerID)).boundingBox;
        //     for (auto& navmesh : navs.NavMeshes){
        //         //auto center = it->centerpoint.second;
        //         auto& currentbbox = navmesh.box;
        //         if(currentbbox.intersects(playerbbox)){
        //             vec3d center = {navmesh.centerpoint.second.x(),navmesh.centerpoint.second.y(),navmesh.centerpoint.second.z()};
        //             idcenter = navmesh.centerpoint.first;
        //             break;
        //             // DrawCube(Vector3{static_cast<float>(center.x()),
        //             // static_cast<float>(center.y()),
        //             // static_cast<float>(center.z())},500,500,500,RED);
        //         }
        //     }
        // }
        //Recorre navs.nodes //    std::set<std::pair<uint16_t, vec3d>> nodes;
        // recorrelos y devuelve
        // Función para encontrar el nodo más cercano a una posición dada
        vec3d posplayer = em.getComponent<PhysicsComponent>(*em.getEntityByID(li.playerID)).position;
        uint16_t startnode = findNearestNode(em, posplayer, navs.nodes);
        uint16_t targetnode = findNearestNode(em, vec3d{ -12.33, 40.0, 22.41 }, navs.nodes);
        //Creamos Grafo
        Graph graph{};
        graph.createGraph(navs.conexiones, navs.nodes);
        std::vector<vec3d> path = graph.PathFindAStar(debug, startnode, targetnode);

        std::size_t lengthpath = path.size();
        std::cout << lengthpath;
        //calcular camnino desde el centro hasta un punto

    //     std::vector<vec3d> nodes;
    //     nodes.push_back({ -106.9, 4.0, 116.0 });
    //     nodes.push_back({ -119.0, 4.0, 114.0 });
    //     nodes.push_back({ -131.0, 4.0, 105.1 });
    //     nodes.push_back({ -105.0, 4.0, 97.3 });
    //     nodes.push_back({ -118.0, 4.0, 92.0 });
    //     nodes.push_back({ -132.0, 4.0, 87.0 });
    //     nodes.push_back({ -117.0, 4.0, 78.0 });
    //     nodes.push_back({ -127.4, 4.0, 69.6 });
    //     //Creamos puntos y conexiones
    //     std::vector<Conection> conexiones;
    //     Conection cone12(1, 1, 2);
    //     conexiones.push_back(cone12);
    //     Conection cone14(1, 1, 4);
    //     conexiones.push_back(cone14);
    //     Conection cone15(1, 1, 5);
    //     conexiones.push_back(cone15);
    //     Conection cone25(1, 2, 5);
    //     conexiones.push_back(cone25);
    //     Conection cone23(1, 2, 3);
    //     conexiones.push_back(cone23);
    //     Conection cone36(1, 3, 6);
    //     conexiones.push_back(cone36);
    //     Conection cone45(1, 4, 5);
    //     conexiones.push_back(cone45);
    //     Conection cone56(1, 5, 6);
    //     conexiones.push_back(cone56);
    //     Conection cone57(1, 5, 7);
    //     conexiones.push_back(cone57);
    //     Conection cone58(1, 5, 8);
    //     conexiones.push_back(cone58);
    //     Conection cone68(1, 6, 8);
    //     conexiones.push_back(cone68);
    //     Conection cone78(1, 7, 8);
    //     conexiones.push_back(cone78);
    //     Lammamos a creargrafo
    //     Creamos el grafo
    //     Graph graph{};
    //     graph.createGraph(navs.conexiones, navs.nodes);
    //     graph.createGraph(conexiones,nodes);
    //     Calcular pathfinding
    //     std::cout << static_cast<uint16_t>(debug.startnode) << static_cast<uint16_t>(debug.goalnode) << "\n";
    //     std::vector<vec3d> path = graph.PathFindAStar(static_cast<uint16_t>(debug.startnode), static_cast<uint16_t>(debug.goalnode));
        if (path.size() == 0) {
            std::cout << "CAGUEEEEEEEE \n";
        }
        else {
            //     // Copiar el path devuelto por PathFindAStar() a debug.path
            debug.path.resize(path.size());
            //     //Rellenamos
            std::copy(path.begin(), path.end(), debug.path.begin());
            //     Mostrar el camino copiado
                //    std::cout << "Camino en debug.path:" << std::endl;
            // for (const auto& node : debug.path) {
            //     std::cout << "(" << node.x() << ", " << node.y() << ", " << node.z() << ")" << std::endl;
            //     debug.nodes.push_back(node);
            // }
        }
        //    debug.path.resize(3); // Cambiar el tamaño del vector a 3 elementos
        //    std::fill(debug.path.begin(), debug.path.end(), vec3d(1.0, 2.0, 3.0)); // Rellenar el vector con vec3d con los valores dados
    }
    if (GuiButton(btn2Rec, "CLEAR")) {
        debug.path.clear();
        debug.nodes.clear();
        debug.closedlist.clear();
    }
    if (GuiButton(btn3Rec, "CORNERS")) {
        debug.seecorners = !debug.seecorners;
    }
    if (GuiButton(btn4Rec, "CENTERS")) {
        debug.seecenters = !debug.seecenters;
    }
    if (GuiButton(btn5Rec, "MIDPOINTS")) {
        debug.seemidpoint = !debug.seemidpoint;
    }
    if (GuiButton(btn6Rec, "CONEXIONES")) {
        debug.seeconex = !debug.seeconex;
    }
    //resultado
    vec2d textPositionInfo2 = { static_cast<double>(engine.getScreenWidth() - 370), 480 };
    engine.drawTextEx(GetFontDefault(), "PATH RESULT", textPositionInfo2, 20, 1, RED);
    //Dibujar path
    float posyt = 510.0f;
    for (auto pos : debug.path) {
        std::string text = std::to_string(pos.x()) + " " + std::to_string(pos.y()) + " " + std::to_string(pos.z());
        engine.drawTextEx(GetFontDefault(), text.c_str(), vec2d{ static_cast<double>(engine.getScreenWidth() - 370),posyt }, 20, 1, RED);
        posyt += 20.0f;
    }
    engine.beginMode3D();
    //DIbujar nodos de la lista cerrada y nodos del path resultado
    for (auto& closenode : debug.closedlist) {
        engine.drawCube(closenode, 2, 2, 2, YELLOW);
    }
    for (auto& node : debug.nodes) {
        engine.drawCube(node, 2, 2, 2, GREEN);
    }

    // for (auto& node : navs.nodes) {
    //     engine.drawCube(node.second, 2, 2, 2, RED);
    // }
//Dibujar corners
    if (debug.seecorners) {
        for (auto& node : navs.corners) {
            engine.drawCube(node, 2, 2, 2, RED);
        }
    }
    if (debug.seecenters) {
        for (auto it = navs.centers.begin(); it != std::prev(navs.centers.end()); ++it) {
            engine.drawCube(it->second, 2, 2, 2, BLUE);
        }
    }
    if (debug.seemidpoint) {
        for (auto& node : navs.midpoints) {
            engine.drawCube(node, 2, 2, 2, PURPLE);
        }
    }
    if (debug.seeconex) {
        for (auto& conex : navs.conexpos) {
            engine.drawLine3D(conex.first, conex.second, GREEN);
        }
        for (auto& bbox : navs.boundingnavmesh) {
            auto boxSize = bbox.max - bbox.min;
            vec3d boxPosition = (bbox.min + bbox.max) / 2;
            //boxPosition.setY(boxPosition.y + 20.0);
            engine.drawCubeWires(boxPosition,
                static_cast<float>(boxSize.x()),
                static_cast<float>(boxSize.y()),
                static_cast<float>(boxSize.z()),
                PURPLE);
        }
    }

    engine.endMode3D();
    engine.beginDrawing();
    for (auto& node : debug.nodes) {
        std::string text = std::to_string(node.x()) + " " + std::to_string(node.y()) + " " + std::to_string(node.z());
        float posx = engine.getWorldToScreenX(node);
        float posy = engine.getWorldToScreenY(node);
        engine.drawTextEx(GetFontDefault(), text.c_str(), vec2d{ static_cast<double>(posx),static_cast<double>(posy) }, 15, 1, RED);
    }

    //engine.endDrawing();

}
//Debugger visual in-game
void RenderSystem::drawDebuggerInGameIA(GameEngine& engine, EntityManager& em)
{
    // engine.beginDrawing();
    float posX = static_cast<float>(engine.getScreenWidth() - 330);
    int posText = static_cast<int>(posX + 10);
    Rectangle windowRect = { posX, 80, 330, 230 };
    engine.drawRectangleLinesEx(windowRect, 2, DARKGRAY);
    engine.drawRectangleRec(windowRect, Color{ 255, 255, 255, 128 });
    vec2d textPositionInfo = { static_cast<double>(posText), 90 };
    engine.drawTextEx(engine.getFontDefault(), "INFO", textPositionInfo, 20, 1, RED);
    auto& debugsnglt = em.getSingleton<Debug_t>();

    using SYSCMPss = MP::TypeList<AIComponent, ColliderComponent, RenderComponent>;
    using SYSTAGss = MP::TypeList<EnemyTag>;

    // AQUI PONDRIA
    em.forEach<SYSCMPss, SYSTAGss>([&](Entity& e, AIComponent& aic, ColliderComponent& col, RenderComponent& ren)
    {
        RayCast ray = engine.getMouseRay();
        if (col.boundingBox.intersectsRay(ray.origin, ray.direction) && !(col.behaviorType & BehaviorType::STATIC || col.behaviorType & BehaviorType::ZONE)) {
            if (engine.isMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isSelectedfordebug = !isSelectedfordebug;
                debugsnglt.IA_id_debug = e.getID();
            }
        }
        if (isSelectedfordebug && e.getID() == debugsnglt.IA_id_debug) {
            auto& bb = em.getSingleton<BlackBoard_t>();
            engine.beginMode3D();
            engine.drawCubeWires(ren.position, static_cast<float>(ren.scale.x()), static_cast<float>(ren.scale.y()), static_cast<float>(ren.scale.z()), PURPLE);
            engine.endMode3D();
            engine.drawText("ID:", posText, 110, 20, BLACK);
            engine.drawTextEx(engine.getFontDefault(), std::to_string(e.getID()).c_str(), vec2d{ static_cast<double>(posText) + 90.0,110 }, 20, 1, DARKGRAY);
            engine.drawText("Node active:", posText, 130, 20, BLACK);
            // std::cout << debugsnglt.elapsed << "\n";
             // std::cout << debugsnglt.countdown << "\n";
            if (debugsnglt.elapsed >= debugsnglt.countdown) {
                debugsnglt.elapsed = 0;
                debugsnglt.text = aic.bh;
            }
            else {
                debugsnglt.plusDeltatime(timeStep, debugsnglt.elapsed);
            }
            engine.drawTextEx(engine.getFontDefault(), debugsnglt.text, vec2d{ static_cast<double>(posText) + 130.0,130 }, 20, 1, DARKGRAY);
            engine.drawText("TEID:", posText, 150, 20, BLACK);
            engine.drawTextEx(engine.getFontDefault(), std::to_string(aic.teid).c_str(), vec2d{ static_cast<double>(posText) + 90.0,150 }, 20, 1, DARKGRAY);
            engine.drawText("TX:", posText, 170, 20, BLACK);
            engine.drawTextEx(engine.getFontDefault(), std::to_string(aic.tx).c_str(), vec2d{ static_cast<double>(posText) + 80.0,170 }, 20, 1, DARKGRAY);
            engine.drawText("TZ:", posText, 190, 20, BLACK);
            engine.drawTextEx(engine.getFontDefault(), std::to_string(aic.tz).c_str(), vec2d{ static_cast<double>(posText) + 80.0,190 }, 20, 1, DARKGRAY);
            engine.drawText("Culldown:", posText, 210, 20, BLACK);
            engine.drawTextEx(engine.getFontDefault(), std::to_string(aic.elapsed_shoot).c_str(), vec2d{ static_cast<double>(posText) + 90.0,210 }, 20, 1, DARKGRAY);
            engine.drawText("Player Detected?:", posText, 230, 20, BLACK);
            engine.drawTextEx(engine.getFontDefault(), (aic.playerdetected == 0) ? "No" : "Sí", vec2d{ static_cast<double>(posText) + 180.0,230 }, 20, 1, RED);
            engine.drawText("Player hunted?:", posText, 250, 20, BLACK);
            engine.drawTextEx(engine.getFontDefault(), (bb.playerhunted == 0) ? "No" : "Sí", vec2d{ static_cast<double>(posText) + 180.0,250 }, 20, 1, RED);
            engine.drawText("Subditos alive:", posText, 270, 20, BLACK);
            engine.drawTextEx(engine.getFontDefault(), std::to_string(bb.subditosData.size()).c_str(), vec2d{ static_cast<double>(posText) + 180.0,270 }, 20, 1, RED);
            engine.drawText("Subditos id alive:", posText, 290, 20, BLACK);
            engine.drawTextEx(engine.getFontDefault(), std::to_string(bb.idsubditos.size()).c_str(), vec2d{ static_cast<double>(posText) + 180.0,290 }, 20, 1, RED);
            engine.drawText("Alert state:", posText, 310, 20, BLACK);
            engine.drawTextEx(engine.getFontDefault(), (aic.alert_state == 0) ? "No" : "Sí", vec2d{ static_cast<double>(posText) + 180.0,310 }, 20, 1, RED);

            engine.beginMode3D();
            //raycast
            if (bb.launched) {
                // engine.beginMode3D();

                auto dir = bb.direction * 100;
                DrawLine3D(bb.position_origin.toRaylib(), dir.toRaylib(), BLUE);
                // engine.endMode3D();
                bb.launched = false;
            }
            //Cone
            //if(e.hasTag<SnowmanTag>())
            auto& phy = em.getComponent<PhysicsComponent>(e);
            drawVisionCone(phy.position, phy.orientation, bb.horizontalFOV);
            // if(e.hasTag<GolemTag>())
            //     drawVisionCone(bb.conegolem.first, bb.conegolem.second, bb.horizontalFOV);
            engine.endMode3D();
        }
    });
    //  engine.endDrawing();
}


// Dentro de tu clase BTDecisionPlayerDetected, podrías tener un método para dibujar el cono de visión
void RenderSystem::drawVisionCone(vec3d pos_enemy, double orientation, double horizontalFOV) {
    // Calcula las direcciones de las líneas del cono
    Vector3 direction1 = { static_cast<float>(std::sin(orientation - horizontalFOV / 2.0)), 0.0f, static_cast<float>(std::cos(orientation - horizontalFOV / 2.0)) };
    Vector3 direction2 = { static_cast<float>(std::sin(orientation + horizontalFOV / 2.0)), 0.0f, static_cast<float>(std::cos(orientation + horizontalFOV / 2.0)) };

    // Calcula los puntos de inicio de las líneas
    Vector3 start1 = pos_enemy.toRaylib();
    Vector3 start2 = pos_enemy.toRaylib();

    // Calcula los puntos finales de las líneas (multiplica por una distancia adecuada para hacerlas visibles)
    Vector3 end1 = { start1.x + direction1.x * 10.0f, start1.y + direction1.y * 10.0f, start1.z + direction1.z * 10.0f };
    Vector3 end2 = { start2.x + direction2.x * 10.0f, start2.y + direction2.y * 10.0f, start2.z + direction2.z * 10.0f };

    // Dibuja las líneas
    DrawLine3D(start1, end1, PURPLE);
    DrawLine3D(start2, end2, PURPLE);
}

//Editor In-Game
void RenderSystem::drawEditorInGameIA(GameEngine& engine, EntityManager& em) {
    // engine.beginDrawing();

    // Dibujar un rectángulo que simula una ventana
    Rectangle windowRect = { 0, 100, 390, 550 };
    engine.drawRectangleLinesEx(windowRect, 2, DARKGRAY);
    engine.drawRectangleRec(windowRect, Color{ 255, 255, 255, 128 });

    // Dibujar el texto "debugger IA" en el centro de la ventana
    vec2d textSize = engine.measureTextEx(engine.getFontDefault(), "Debugger IA", 20, 1);
    vec2d textPosition = { windowRect.x + 20,
                             windowRect.y + 10 };

    engine.drawTextEx(engine.getFontDefault(), "Editor IA", textPosition, 20, 1, DARKBLUE);

    // Dibujar una línea recta debajo del texto
    float lineY = static_cast<float>(textPosition.y + textSize.y + 5);  // Ajusta la posición de la línea según tus necesidades
    engine.drawLine(static_cast<int>(windowRect.x), static_cast<int>(lineY), static_cast<int>(windowRect.x) + static_cast<int>(windowRect.width),
        static_cast<int>(lineY), DARKGRAY);
    // Dibujar el texto "INFO" debajo de la línea

    vec2d textPositionParameters = { windowRect.x + 5, 150 };

    engine.drawTextEx(engine.getFontDefault(), "PARÁMETROS", textPositionParameters, 20, 1, RED);

    auto& debugsnglt = em.getSingleton<Debug_t>();

    using SYSCMPss = MP::TypeList<AIComponent, PhysicsComponent, ColliderComponent, RenderComponent>;
    using SYSTAGss = MP::TypeList<EnemyTag>;

    // AQUI PONDRIA
    em.forEach<SYSCMPss, SYSTAGss>([&](Entity& e, AIComponent& aic, PhysicsComponent& phy, ColliderComponent& col, RenderComponent& ren)
    {
        RayCast ray = engine.getMouseRay();
        // Comprobar si el rayo intersecta con el collider
        if (col.boundingBox.intersectsRay(ray.origin, ray.direction) && !(col.behaviorType & BehaviorType::STATIC || col.behaviorType & BehaviorType::ZONE)) {
            if (engine.isMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
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
                engine.drawText("EID:", 15, 170, 20, BLACK);
                engine.drawText(std::to_string(debugsnglt.IA_id).c_str(), 55, 170, 20, DARKGRAY);
                //Detect Radius
                aic.detect_radius = SelectValue(engine, aic.detect_radius, 145.0, 200.0, 120.0, 30.0, "Detect Radius", 0.0, 100.0);
                // Attack Radius
                aic.attack_radius = SelectValue(engine, aic.attack_radius, 145.0, 240.0, 120.0, 30.0, "Attack Radius", 0.0, 100.0);
                // Arrival Radius
                aic.arrival_radius = SelectValue(engine, aic.arrival_radius, 145.0, 280.0, 120.0, 30.0, "Arrival Radius", 0.0, 100.0);
                // Max Speed
                phy.max_speed = SelectValue(engine, phy.max_speed, 145.0, 320.0, 120.0, 30.0, "Max_Speed", 0.0, 10.0);
                //COuntdown Perception
                aic.countdown_perception = SelectValue(engine, aic.countdown_perception, 145.0, 360.0, 120.0, 30.0, "Perception", 0.0, 10.0);
                //Countdown Shoot
                aic.countdown_shoot = SelectValue(engine, aic.countdown_shoot, 145.0, 400.0, 120.0, 30.0, "Culldown Shoot", 0.0, 8.0);
                //Countdown stop
                aic.countdown_stop = SelectValue(engine, aic.countdown_stop, 145.0, 440.0, 120.0, 30.0, "Culldown Stop", 0.0, 8.0);
            }
        }
    });
    // engine.endDrawing();
}
//Dibujado alertas de detección de enemigos
void RenderSystem::drawAlerts_IA(EntityManager& em, GameEngine& engine) {
    for (auto const& e : em.getEntities())
    {
        //Alert state
        if (e.hasTag<EnemyTag>() && e.hasComponent<RenderComponent>() && e.hasComponent<AIComponent>()) {
            auto& aic = em.getComponent<AIComponent>(e);
            auto& r = em.getComponent<RenderComponent>(e);

            float barX = engine.getWorldToScreenX(r.position);
            float barY = engine.getWorldToScreenY(r.position);

            if (!aic.playerdetected) {
                aic.show_icon = true;
            }

            if (aic.playerdetected && aic.show_icon) {
                // vec2d point1 = { barX, barY - 120.0f };
                // vec2d point2 = { barX - 30.0f, barY - 50.0f };
                // vec2d point3 = { barX + 30.0f, barY - 50.0f };
                // //dibujar icono alerta
                // // Dibuja el triángulo
                // engine.drawTriangle(point1, point2, point3, BLACK);
                // // Dibuja el signo de exclamación dentro del triángulo
                // engine.drawText("!", static_cast<int>(barX - 2), static_cast<int>(barY - 100), 50, YELLOW);
                auto& icon = engine.textures["detectionicon"];
                engine.drawTexture(icon, static_cast<int>(barX - 15.0f), static_cast<int>(barY - 135.0f), WHITE);

                //emepezar contador para borrar
                if (aic.elapsed_show_icon >= aic.countdown_show_icon) {
                    aic.elapsed_show_icon = 0.0;
                    aic.show_icon = false;
                }
                else {
                    aic.plusDeltatime(timeStep, aic.elapsed_show_icon);
                }
            }

            vec2d center = { barX, barY - 120.0f };
            if (aic.alert_state) {
                //Se escuahn pasos
                if (aic.listen_steps) {
                    aic.endangle -= aic.increase_angle;
                }
                else {
                    //No se escuchan los pasos
                    if (aic.endangle != 0.0f) {
                        aic.endangle += aic.increase_angle;
                    }
                }
                //std::cout << endangle << "\n";
                engine.drawCircleSector(center, 30.0f, 0.0f, aic.endangle, 30, RED);
            }
            else {
                if (aic.endangle != 0.0f) {
                    aic.endangle += aic.increase_angle;
                }
            }
        }
    }
}
// Se dibuja el HUD
void RenderSystem::drawHUD(EntityManager& em, GameEngine& engine, bool debugphy)
{
    auto& li = em.getSingleton<LevelInfo>();
    if (li.isDead)
    {
        em.getComponent<RenderComponent>(*em.getEntityByID(li.playerID)).visible = false;
        drawDeath(engine);
        return;
    }

    if (debugphy)
        pointedEntity = std::numeric_limits<std::size_t>::max();

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

            drawFPSCounter(engine);

            // Dibujar el bastón
            drawStaff(engine, em);

            // Dibujar espacios de hechizos
            drawSpellSlots(engine, em);

            drawAnimatedTextures(engine);

            if (li.mapID == 2 && li.volcanoMission)
                drawBoatParts(engine, em);

            // Dibujar el tipo de ataque que tiene equipado
            if (e.hasComponent<TypeComponent>())
            {
                // Rectangulo del tipo de magia
                engine.drawRectangle(7, 50, 100, 20, DARKGRAY);

                auto const& t{ em.getComponent<TypeComponent>(e) };

                if (t.type == ElementalType::Neutral)
                    engine.drawText("Neutral", 17, 50, 18, WHITE);
                else if (t.type == ElementalType::Water)
                    engine.drawText("Agua", 17, 50, 18, BLUE);
                else if (t.type == ElementalType::Fire)
                    engine.drawText("Fuego", 17, 50, 18, RED);
                else
                    engine.drawText("Hielo", 17, 50, 18, SKYBLUE);
            }

            if ((li.mapID == 0 || li.mapID == 1) && e.hasComponent<AttackComponent>())
            {
                if (!li.tutorialEnemies.empty())
                {
                    for (auto& enemy : li.tutorialEnemies)
                    {
                        auto& ene = *em.getEntityByID(enemy);
                        if (ene.hasComponent<RenderComponent>())
                        {
                            auto& ren{ em.getComponent<RenderComponent>(ene) };
                            auto& phy{ em.getComponent<PhysicsComponent>(ene) };
                            if (ren.visible && (ene.hasTag<DummyTag>() || ene.hasTag<DestructibleTag>()))
                            {
                                double multiplier = 28.0;

                                GameEngine::Gif* gif;
                                Texture2D gifCopy;
                                if (li.lockedEnemy == li.max)
                                {
                                    if (engine.isGamepadAvailable(0))
                                        gif = &engine.gifs.at("l2");
                                    else
                                        gif = &engine.gifs.at("q");
                                }
                                else
                                {
                                    if (engine.isGamepadAvailable(0))
                                        gif = &engine.gifs.at("r2");
                                    else
                                        gif = &engine.gifs.at("espacio");
                                }

                                gifCopy = gif->texture;

                                // Redimensionamos la copia
                                gifCopy.width = static_cast<int>(gifCopy.width / 2.0);
                                gifCopy.height = static_cast<int>(gifCopy.height / 2.0);

                                multiplier = 55.0;

                                if (ene.hasTag<DestructibleTag>())
                                    multiplier = 8.0;
                                else if (li.mapID == 1)
                                    multiplier = 25.0;

                                int posX = static_cast<int>(engine.getWorldToScreenX(phy.position)) - gifCopy.width / 2;
                                int posY = static_cast<int>(engine.getWorldToScreenY(phy.position) - phy.scale.y() * multiplier);

                                displayGif(engine, gifCopy, *gif, posX, posY);
                            }
                        }
                    }
                }
            }

            if (li.mapID == 0 && li.num_zone == 1 && elapsed_WASD < elapsed_limit_WASD)
            {
                auto& phy{ em.getComponent<PhysicsComponent>(e) };

                GameEngine::Gif* gif;
                Texture2D gifCopy;
                if (engine.isGamepadAvailable(0))
                    gif = &engine.gifs.at("joystick_izq");
                else
                    gif = &engine.gifs.at("wasd");

                gifCopy = gif->texture;

                // Redimensionamos la copia
                gifCopy.width = static_cast<int>(gifCopy.width / 2.0);
                gifCopy.height = static_cast<int>(gifCopy.height / 2.0);

                int posX = static_cast<int>(engine.getWorldToScreenX(phy.position)) - gifCopy.width / 2;
                int posY = static_cast<int>(engine.getWorldToScreenY(phy.position) - phy.scale.y() * 37);

                displayGif(engine, gifCopy, *gif, posX, posY);

                elapsed_WASD += 1.0f / 60.0f;
            }

            if (li.mapID == 2 && e.hasComponent<AttackComponent>() && !li.volcanoLava.empty())
            {
                auto& plfi = em.getSingleton<PlayerInfo>();
                std::size_t spellID{ 5 };

                // Mapear los IDs de los hechizos a los nombres de los gifs
                static std::map<std::size_t, std::pair<std::string, std::string>> spellToGif = {
                    {0, {"cuadrado", "j"}},
                    {1, {"circulo", "k"}},
                    {2, {"triangulo", "l"}}
                };

                for (std::size_t i = 0; i < plfi.spellSlots.size(); ++i)
                {
                    if (plfi.spellSlots[i].spell == Spells::WaterDash)
                    {
                        spellID = i;
                        break;
                    }
                }

                if (spellID != 5)
                {

                    // Ponemos un gif del botón del dash encima de la lava
                    for (auto& lava : li.volcanoLava)
                    {
                        auto& lav = *em.getEntityByID(lava);
                        auto& phy{ em.getComponent<PhysicsComponent>(lav) };

                        GameEngine::Gif* gif;
                        Texture2D gifCopy;

                        // Usar el mapa para obtener el nombre del gif
                        if (engine.isGamepadAvailable(0))
                            gif = &engine.gifs.at(spellToGif[spellID].first);
                        else
                            gif = &engine.gifs.at(spellToGif[spellID].second);

                        gifCopy = gif->texture;

                        // Redimensionamos la copia
                        gifCopy.width = static_cast<int>(gifCopy.width / 2.0);
                        gifCopy.height = static_cast<int>(gifCopy.height / 2.0);

                        int posX = static_cast<int>(engine.getWorldToScreenX(phy.position)) - gifCopy.width / 2;
                        int posY = static_cast<int>(engine.getWorldToScreenY(phy.position) - phy.scale.y() * 10);

                        displayGif(engine, gifCopy, *gif, posX, posY);
                    }
                }
            }
        }

        if (e.hasTag<CrusherTag>()) {
            auto& phy = em.getComponent<PhysicsComponent>(e);
            auto& ai = em.getComponent<AIComponent>(e);
            int posx = static_cast<int>(engine.getWorldToScreenX(phy.position) + 30);
            int posz = static_cast<int>(engine.getWorldToScreenY(phy.position) - 70);
            if (ai.playerdetected) {
                engine.drawRectangle(posx, posz, 10, 100, BLACK);
                int barHeight = static_cast<int>((ai.elapsed_shoot / ai.countdown_shoot) * 100);
                engine.drawRectangle(posx, posz, 10, barHeight, BLUE);
            }
        }

        // Dibujar el precio d elos objetos de la tienda
        // if (e.hasTag<ObjectTag>()) {
        //     if (e.hasComponent<ObjectComponent>() && e.hasComponent<RenderComponent>())
        //     {
        //         auto& ren{ em.getComponent<RenderComponent>(e) };
        //         auto& obj{ em.getComponent<ObjectComponent>(e) };
        //         if (obj.type == ObjectType::ShopItem_Bomb)
        //             engine.drawText("20",
        //                 static_cast<int>(engine.getWorldToScreenX(ren.position) - 10),
        //                 static_cast<int>(engine.getWorldToScreenY(ren.position) + ren.scale.y() * 50),
        //                 20,
        //                 BLACK);
        //         else if (obj.type == ObjectType::ShopItem_Life)
        //             engine.drawText("10",
        //                 static_cast<int>(engine.getWorldToScreenX(ren.position) - 10),
        //                 static_cast<int>(engine.getWorldToScreenY(ren.position) + ren.scale.y() * 50),
        //                 20,
        //                 BLACK);
        //         else if (obj.type == ObjectType::ShopItem_ExtraLife)
        //             engine.drawText("30",
        //                 static_cast<int>(engine.getWorldToScreenX(ren.position) - 10),
        //                 static_cast<int>(engine.getWorldToScreenY(ren.position) + ren.scale.y() * 50),
        //                 20,
        //                 BLACK);
        //     }
        // }

        // Vidas HUD
        if (e.hasTag<EnemyTag>() && e.hasComponent<LifeComponent>() && em.getComponent<RenderComponent>(e).visible &&
            !(e.hasTag<AngryBushTag>() || e.hasTag<AngryBushTag2>()))
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
            int barX = static_cast<int>(engine.getWorldToScreenX(r.position) - 18);
            int barY = static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 15);

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

            if (e.hasTag<SubjectTag>() && e.hasComponent<SubjectComponent>())
            {
                auto& sub{ em.getComponent<SubjectComponent>(e) };

                // Dibujamos una barra de vida para el escudo si es que tiene uno activo
                if (sub.activeShield)
                {
                    engine.drawRectangle(barX, barY - 10, barWidth, barHeight, DARKGRAY);
                    float normalizedShieldLife = (static_cast<float>(sub.shieldLife) / static_cast<float>(sub.maxShieldLife));

                    // Calcula la anchura de la barra de vida
                    int shieldWidth = static_cast<int>(static_cast<float>(barWidth) * normalizedShieldLife);

                    if (sub.shieldLife != sub.maxShieldLife)
                        shieldWidth = sub.shieldLifeWidth + static_cast<int>((shieldWidth - sub.shieldLifeWidth) * 0.25);

                    // Dibujamos la barra de vida
                    engine.drawRectangle(barX, barY - 10, shieldWidth, barHeight, GREEN);

                    sub.shieldLifeWidth = shieldWidth;
                }
            }
        }

        if (e.hasComponent<InteractiveComponent>() && (e.hasComponent<RenderComponent>() || e.hasComponent<PhysicsComponent>()))
        {
            auto& inter{ em.getComponent<InteractiveComponent>(e) };
            vec3d pos{};
            double sclY{};
            if (e.hasTag<SeparateModelTag>())
            {
                auto phy = em.getComponent<PhysicsComponent>(e);
                pos = phy.position;
                sclY = phy.scale.y();
            }
            else
            {
                auto& ren = em.getComponent<RenderComponent>(e);
                pos = ren.position;
                sclY = ren.scale.y();
            }

            if (inter.showButton)
            {
                GameEngine::Gif* gif;
                Texture2D gifCopy;
                int sum = 0;
                if (engine.isGamepadAvailable(0))
                    gif = &engine.gifs.at("x");
                else
                {
                    gif = &engine.gifs.at("e");
                    sum = 13;
                }

                gifCopy = gif->texture;

                // Redimensionamos la copia
                gifCopy.width = static_cast<int>(gifCopy.width / 2.0);
                gifCopy.height = static_cast<int>(gifCopy.height / 2.0);

                int offSetX = gifCopy.width / 2;
                if (e.hasTag<DoorTag>())
                    offSetX = -(gifCopy.width / 2 + sum);

                int posX = static_cast<int>(engine.getWorldToScreenX(pos)) - offSetX;
                int posY = static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 11);

                displayGif(engine, gifCopy, *gif, posX, posY);

                if (e.hasTag<DoorTag>())
                {
                    auto& lock = engine.textures["candado_abierto"];
                    engine.drawTexture(lock,
                        static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(lock.width / 2)),
                        static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 13),
                        { 255, 255, 255, 255 });
                }
            }
            else if (inter.showLock)
            {
                auto& lock = engine.textures["candado_cerrado"];
                engine.drawTexture(lock,
                    static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(lock.width / 2)),
                    static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 13),
                    { 255, 255, 255, 255 });

                if (e.hasTag<ChestTag>())
                {
                    auto& swordText = engine.textures["batalla"];
                    engine.drawTexture(swordText,
                        static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(swordText.width / 2)),
                        static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 13),
                        { 255, 255, 255, 255 });

                    auto& ch = em.getComponent<ChestComponent>(e);
                    if (ch.closeEnemies > 0)
                    {
                        // Dibujamos el número de partes de barco encontradas
                        auto& textureNum = engine.textures.at(std::to_string(ch.closeEnemies));
                        auto& textureMax = engine.textures.at(std::to_string(ch.maxEnemies));
                        auto& textureBar = engine.textures.at("barra");

                        auto posX = static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(textureNum.width / 2) - 100);
                        auto posY = static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 12);

                        // Dibujamos el num / 4
                        engine.drawTexture(textureNum, posX, posY, { 255, 255, 255, 255 });
                        engine.drawTexture(textureBar, posX + textureNum.width / 3, posY, { 255, 255, 255, 255 });
                        engine.drawTexture(textureMax, posX + textureNum.width / 2 + textureBar.width / 2, posY + 20, { 255, 255, 255, 255 });
                    }
                }
            }

            if (e.hasTag<ChestTag>() && li.enemyToChestPos != vec3d::zero() && !li.playerDetected)
            {
                auto& chest = em.getComponent<ChestComponent>(e);
                if (!chest.isOpen)
                {
                    auto& phy = em.getComponent<PhysicsComponent>(e);
                    if (phy.position.distance(li.enemyToChestPos) < 50.0)
                    {
                        if (elapsed_Lock < elapsed_limit_Lock)
                        {
                            auto& openLock = engine.textures["candado_abierto"];
                            engine.drawTexture(openLock,
                                static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(openLock.width / 2)),
                                static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 9),
                                { 255, 255, 255, 255 });

                            elapsed_Lock += timeStep;
                        }
                        else
                        {
                            li.enemyToChestPos = vec3d::zero();
                            elapsed_Lock = 0.0;
                        }
                    }
                }
            }
        }

        if (debugphy && e.hasComponent<LifeComponent>() && em.getComponent<RenderComponent>(e).visible)
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            auto const& l{ em.getComponent<LifeComponent>(e) };

            engine.drawText(std::to_string(l.life).c_str(),
                static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
                static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 8.0),
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
                else if (t.type == ElementalType::Water)
                {
                    tipo = "Agua";
                    color = BLUE;
                }
                else if (t.type == ElementalType::Fire)
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

        if (debugphy && e.hasComponent<ZoneComponent>())
        {
            auto& ren{ em.getComponent<RenderComponent>(e) };
            auto& z{ em.getComponent<ZoneComponent>(e) };

            engine.drawText(std::to_string(z.zone).c_str(),
                static_cast<int>(engine.getWorldToScreenX(ren.position) - 5),
                static_cast<int>(engine.getWorldToScreenY(ren.position) - ren.scale.y() * 5),
                20,
                RED);
        }

        if (debugphy && e.hasComponent<RampComponent>() && e.hasComponent<PhysicsComponent>())
        {
            // Dibujamos el rectángulo de la rampa
            auto& phy = em.getComponent<PhysicsComponent>(e);
            auto& rc = em.getComponent<RampComponent>(e);
            // La rampa solo tiene vec2d mínimos y máximos, vamos a dibujar el rectángulo que los une
            engine.drawText(std::to_string(rc.offset.y()).c_str(),
                static_cast<int>(engine.getWorldToScreenX(phy.position) - 5),
                static_cast<int>(engine.getWorldToScreenY(phy.position) - phy.scale.y() * 5),
                20, RED);
            engine.beginMode3D();
            engine.drawCubeWires(phy.position, static_cast<float>(phy.scale.x()), static_cast<float>(phy.scale.y()), static_cast<float>(phy.scale.z()), RED);
            engine.endMode3D();
        }

        if (debugphy && e.hasComponent<PhysicsComponent>() && e.hasComponent<ColliderComponent>() && e.hasComponent<RenderComponent>())
        {
            auto& col{ em.getComponent<ColliderComponent>(e) };

            // Calcular la posición y el tamaño de la bounding box
            vec3d boxPosition = (col.boundingBox.min + col.boundingBox.max) / 2;
            vec3d boxSize = col.boundingBox.max - col.boundingBox.min;

            Color color = BLUE;
            if (col.behaviorType & BehaviorType::ZONE)
                color = GREEN;

            // Dibujar la bounding box
            engine.beginMode3D();
            engine.drawCubeWires(boxPosition,
                static_cast<float>(boxSize.x()),
                static_cast<float>(boxSize.y()),
                static_cast<float>(boxSize.z()),
                color);
            engine.endMode3D();

            auto& phy = em.getComponent<PhysicsComponent>(e);

            RayCast ray = engine.getMouseRay();

            //std::cout << ray.origin << " " << ray.direction << std::endl;

            auto& ren = em.getComponent<RenderComponent>(e);
            bool notStatic = !(col.behaviorType & BehaviorType::ZONE);
            // Comprobar si el rayo intersecta con el collider

            if (col.boundingBox.intersectsRay(ray.origin, ray.direction) && notStatic && pointedEntity != li.playerID)
            {
                pointedEntity = e.getID();

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
                    PURPLE);
                engine.endMode3D();

                engine.beginMode3D();
                engine.drawCubeWires(ren.position, static_cast<float>(ren.scale.x()), static_cast<float>(ren.scale.y()), static_cast<float>(ren.scale.z()), RED);
                engine.endMode3D();

                // Dibujar el HUD de debug
                engine.drawRectangle(0, 65, 150, 360, WHITE);
                engine.drawText("Posición", 10, 70, 20, BLACK);
                std::string posX = "X: " + std::to_string(phy.position.x());
                engine.drawText(posX.c_str(), 10, 95, 20, BLACK);
                std::string posY = "Y: " + std::to_string(phy.position.y());
                engine.drawText(posY.c_str(), 10, 120, 20, BLACK);
                std::string posZ = "Z: " + std::to_string(phy.position.z());
                engine.drawText(posZ.c_str(), 10, 145, 20, BLACK);

                engine.drawText("Escala", 10, 175, 20, BLACK);
                std::string sclX = "X: " + std::to_string(phy.scale.x());
                engine.drawText(sclX.c_str(), 10, 200, 20, BLACK);
                std::string sclY = "Y: " + std::to_string(phy.scale.y());
                engine.drawText(sclY.c_str(), 10, 225, 20, BLACK);
                std::string sclZ = "Z: " + std::to_string(phy.scale.z());
                engine.drawText(sclZ.c_str(), 10, 250, 20, BLACK);

                engine.drawText("Velocidad", 10, 280, 20, BLACK);
                std::string velX = "X: " + std::to_string(phy.velocity.x());
                engine.drawText(velX.c_str(), 10, 305, 20, BLACK);
                std::string velY = "Y: " + std::to_string(phy.velocity.y());
                engine.drawText(velY.c_str(), 10, 330, 20, BLACK);
                std::string velZ = "Z: " + std::to_string(phy.velocity.z());
                engine.drawText(velZ.c_str(), 10, 355, 20, BLACK);

                std::string id = "ID: " + std::to_string(e.getID());
                engine.drawText(id.c_str(), 10, 385, 20, BLACK);
            }
        }
        // Dibujar zona para mostrar ejemplo de uso del eventmanager

        // auto& linfo = em.getSingleton<LevelInfo>();
        // auto& bb = em.getSingleton<BlackBoard_t>();
        // if (linfo.num_zone == 11) {
        //     if (bb.boss_fase == 1) {
        //         engine.drawText("FASE 1", 500, 10, 70, RED);
        //     }
        //     else {
        //         engine.drawText("FASE 2", 500, 10, 70, ORANGE);
        //     }

        //     // if (linfo.segundos == 0) {
        //     //     linfo.drawzone = false;
        //     //     linfo.segundos = 1000;
        //     // }
        //     // linfo.segundos--;
        // }
        // else {
        //     engine.drawText(("ZONA " + std::to_string(linfo.num_zone)).c_str(), 600, 10, 50, RED);
        // }

        // Dibujar el ID de las entidades // DEBUG
        if (debugphy && e.hasComponent<RenderComponent>())
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            engine.drawText(std::to_string(e.getID()).c_str(),
                static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
                static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 50),
                20,
                BLACK);
        }
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
            if (color.a == 100)
            {
                auto& destellin = engine.textures["destellin"];
                engine.drawTexture(destellin,
                    static_cast<int>(engine.getWorldToScreenX(r.position)) - destellin.width / 2,
                    static_cast<int>(engine.getWorldToScreenY(r.position)) - destellin.height / 2,
                    { 255, 255, 255, 255 });
            }
            else
            {
                auto& fijado = engine.gifs.at("fijado");
                auto copy = fijado.texture;

                // Redimensionamos la copia
                copy.width = static_cast<int>(copy.width / fijado.reScaleX);
                copy.height = static_cast<int>(copy.height / fijado.reScaleY);

                int posX = static_cast<int>(engine.getWorldToScreenX(r.position)) - copy.width / 2;
                int posY = static_cast<int>(engine.getWorldToScreenY(r.position)) - copy.height / 2;

                displayGif(engine, copy, fijado, posX, posY);
            }
        }
    }
}

void RenderSystem::drawDeath(GameEngine& engine)
{
    engine.drawRectangle(0, 0, engine.getScreenWidth(), engine.getScreenHeight(), Fade(BLACK, 0.5f));

    // Valores de la caja de texto
    float boxWidth = 300.f;
    float boxWidth2 = 500.f;
    float boxHeight = 100.f;
    float posX = static_cast<float>(engine.getScreenWidth() / 2) - (boxWidth / 2.f);
    float posX2 = static_cast<float>(engine.getScreenWidth() / 2) - (boxWidth2 / 2.f);
    float posY = static_cast<float>(engine.getScreenHeight() / 2) - (boxHeight / 2.f);

    // Tamaño de la fuente
    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);

    // Color de la fuente de texto
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFF0000ff);

    GuiLabelButton(Rectangle{ posX, posY, boxWidth, boxHeight }, "HAS MUERTO");

    std::string text = "[ENTER] para volver a jugar";
    if (engine.isGamepadAvailable(0))
        text = "Pulsa [X] para volver a jugar";
    GuiLabelButton(Rectangle{ posX2, posY + 50, boxWidth2, boxHeight }, text.c_str());
    init();
}

void RenderSystem::unloadModels(EntityManager& em, GameEngine& engine)
{
    using SYSCMPs = MP::TypeList<RenderComponent>;
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity&, RenderComponent& ren)
    {
        // engine.unloadMesh(ren.mesh);
        engine.unloadModel(ren.model);
        ren.meshLoaded = false;
    });
}

void RenderSystem::drawHealthBar(GameEngine& engine, EntityManager& em, const Entity& e)
{
    auto const& l{ em.getComponent<LifeComponent>(e) };
    auto& plfi = em.getSingleton<PlayerInfo>();

    // Datos de la barra de vida
    int barWidth = 40;
    // int barHeight = 20;
    int barX = 155;
    int barY = 30;
    int spacing = 10;

    // Rectángulo de fondo para la barra de vida
    // engine.drawRectangle(barX - 3, barY - 2, (barWidth + spacing) * (l.maxLife + plfi.armor) + 2, barHeight + 4, DARKGRAY);

    // Dibujamos cara del maguito
    if (l.life > l.maxLife / 2)
        engine.drawTexture(engine.textures["mago_happy"], 25, 20, { 255, 255, 255, 255 });
    else if (l.life > 2)
        engine.drawTexture(engine.textures["mago_meh"], 25, 20, { 255, 255, 255, 255 });
    else
        engine.drawTexture(engine.textures["mago_sos"], 25, 20, { 255, 255, 255, 255 });

    // Dibujamos cada parte de la barra de vida
    int i{};
    for (; i < l.life; ++i)
    {
        // Posición X de cada trozo
        int currentX = barX + i * (barWidth + spacing);

        // Dibujamos el corazón
        // engine.drawRectangle(currentX, barY, barWidth, barHeight, RED);
        engine.drawTexture(engine.textures["heart"], currentX, barY, { 255, 255, 255, 255 });
    }

    for (; i < l.maxLife; ++i)
    {
        // Posición X de cada trozo
        int currentX = barX + i * (barWidth + spacing);

        // Dibujamos el corazón vacío
        // engine.drawRectangle(currentX, barY, barWidth, barHeight, RED);
        engine.drawTexture(engine.textures["empty_heart"], currentX, barY, { 255, 255, 255, 255 });
    }

    // Dibujamos la armadura
    if (plfi.armor > 0)
        for (; i < l.maxLife + plfi.armor; ++i)
        {
            // Posición X de cada trozo
            int currentX = barX + i * (barWidth + spacing);

            // Dibujamos el corazón
            engine.drawTexture(engine.textures["ice_heart"], currentX, barY, SKYBLUE);
        }
}

void RenderSystem::drawCoinBar(GameEngine& engine, EntityManager& em)
{
    auto& plfi{ em.getSingleton<PlayerInfo>() };
    if (plfi.coins == 0)
        return;

    const float multip = 3.5f;
    if (plfi.elapsed_coins < plfi.elapsed_limit_coins)
    {
        elapsed_CoinBar += timeStep * multip;
        if (elapsed_CoinBar > elapsed_limit_CoinBar) elapsed_CoinBar = elapsed_limit_CoinBar;

        plfi.elapsed_coins += timeStep;
    }
    else
    {
        elapsed_CoinBar -= timeStep * multip;
        if (elapsed_CoinBar < 0) elapsed_CoinBar = 0;
    }

    std::vector<int> digits{}, digits2{};
    auto coinsCopy = plfi.coins;
    auto coinsCopy2 = plfi.coinsAdded;
    // Sacamos cada dígito individual
    while (coinsCopy > 0)
    {
        digits.push_back(coinsCopy % 10);
        coinsCopy /= 10;
    }

    // Sacamos cada dígito individual
    while (coinsCopy2 > 0)
    {
        digits2.push_back(coinsCopy2 % 10);
        coinsCopy2 /= 10;
    }

    double div = elapsed_CoinBar / elapsed_limit_CoinBar;

    // Posición de la barra de destellos
    auto sum = static_cast<double>(digits.size()) * 16.5; // 16.5 es la mitad del ancho de la textura de la moneda
    int offSetX = static_cast<int>(120 + sum);

    // Interpolación
    coinBarX = static_cast<int>((1.f - div) * static_cast<float>(engine.getScreenWidth()) + div * static_cast<float>(engine.getScreenWidth() - offSetX));

    // Barra para los destellos
    engine.drawTexture(engine.textures["destellos"], coinBarX, engine.getScreenHeight() - 130, { 255, 255, 255, 255 });

    // Interpolación de la posición de los números
    int offSetCoinNum = static_cast<int>(40 + sum);
    coinNumberX = static_cast<int>((1.f - div) * (static_cast<float>(engine.getScreenWidth() + (offSetX - offSetCoinNum))) + div * static_cast<float>(engine.getScreenWidth() - offSetCoinNum));
    int posY = engine.getScreenHeight() - 117;
    auto coinNumberX2 = coinNumberX;
    std::string plusMinus = "+";

    if (plfi.minusCoins)
        plusMinus = "-";

    // Dibujamos el número de destellos
    if (elapsed_CoinBar > 0 && plfi.coins > 0)
    {
        engine.drawText(plusMinus.c_str(), coinNumberX - 10, posY - 25, 25, { 255, 255, 255, 255 });
        for (std::size_t i = digits.size(); i-- > 0; )
        {
            auto& texture = engine.textures.at(std::to_string(digits[i]));
            engine.drawTexture(texture, coinNumberX, posY, { 255, 255, 255, 255 });
            coinNumberX += static_cast<int>(texture.width / 1.7);
        }

        // Dibujamos el número de monedas
        for (std::size_t i = digits2.size(); i-- > 0; )
        {
            auto& texture = engine.textures.at(std::to_string(digits2[i]));
            engine.drawTexture(texture, coinNumberX2, posY - 30, { 255, 255, 255, 255 });
            coinNumberX2 += static_cast<int>(texture.width / 1.7);
        }
    }
    else if (plfi.minusCoins)
        plfi.minusCoins = false;
}

void RenderSystem::drawManaBar(GameEngine& engine, EntityManager& em)
{
    auto& plfi{ em.getSingleton<PlayerInfo>() };

    if (plfi.mana > plfi.max_mana)
        plfi.mana = plfi.max_mana - 2;

    // Datos para la barra para el maná
    int barWidth = static_cast<int>(plfi.max_mana * 1.8);
    // int barHeight = 20;
    int barX = 155;
    int barY = 85;

    int manaWidth = static_cast<int>(static_cast<float>(barWidth) * (static_cast<float>(plfi.mana) / static_cast<float>(plfi.max_mana)));

    // Interpolación
    if (plfi.mana != plfi.max_mana)
        manaWidth = plfi.mana_width + static_cast<int>((static_cast<float>(manaWidth) - static_cast<float>(plfi.mana_width)) * 0.175f);

    // Idea para el movimiento de la barra
    // Dos capas, la barra primero y luego el borde
    // Entre medias algo que tape la barra dependiendo de la cantidad de maná en la barra.

    // Dibujamos la barra de maná
    engine.drawRectangle(barX + 14, barY + 9, manaWidth, 25, { 154, 222, 235, 255 });

    // Ponemos la textura de la barra de maná
    engine.drawTexture(engine.textures["mana"], barX, barY, { 255, 255, 255, 255 });


    plfi.mana_width = manaWidth;
}

void RenderSystem::handleAnimatedTexture(const std::string& name, const std::string& textureName, int x, int y, const Texture2D& texture, float scaleFactor)
{
    if (animatedTextures.find(name) == animatedTextures.end())
    {
        animatedTextures[name] = { textureName, x, y, texture.width, texture.height, scaleFactor };
    }
    else
    {
        animatedTextures[name].targetPosX = x;
        animatedTextures[name].targetPosY = y;
    }
}

void RenderSystem::drawSpellSlots(GameEngine& engine, EntityManager& em)
{
    auto& plfi{ em.getSingleton<PlayerInfo>() };

    if (!plfi.spells.empty())
    {
        static std::map<std::size_t, std::pair<int, int>> spellPositions = {
            {0, {engine.getScreenWidth() - 270, 20}},
            {1, {engine.getScreenWidth() - 210, 110}},
            {2, {engine.getScreenWidth() - 110, 150}}
        };

        static std::map<Spells, std::tuple<std::string, std::string, int, int, float>> spellToTexture = {
            {Spells::WaterBomb, {"pompas", "exp_pompa", 10, 15, 2.5f}},
            {Spells::WaterDash, {"dash", "exp_pompa", 15, 17, 2.75f}},
            {Spells::FireBall, {"bola_fuego", "exp_pompa", 17, 22, 2.75f}},
            {Spells::FireMeteorites, {"meteoritos", "exp_pompa", 15, 17, 2.75f}},
            {Spells::IceShards, {"estacas", "exp_pompa", 15, 17, 2.75f}},
            {Spells::IceShield, {"escudo", "exp_pompa", 15, 17, 2.75f}},
        };

        for (std::size_t i = 0; i < plfi.spellSlots.size(); i++)
        {
            auto& spell = plfi.spellSlots[i];
            if (spell.spell != plfi.noSpell)
            {
                if (!plfi.showBook)
                {
                    std::string spellName = "hechizo" + std::to_string(i + 1);
                    handleAnimatedTexture(std::to_string(i + 1) + "_pl", "placeholder", spellPositions[i].first, spellPositions[i].second, engine.textures["placeholder"], 2.5f);

                    // Usar el mapa para obtener el nombre de la textura, posiciones y factor de escala
                    auto textureDetails = spellToTexture[spell.spell];
                    handleAnimatedTexture(spellName, std::get<0>(textureDetails), spellPositions[i].first + std::get<2>(textureDetails), spellPositions[i].second + std::get<3>(textureDetails), engine.textures[std::get<0>(textureDetails)], std::get<4>(textureDetails));
                }
                else
                {
                    // Usar el mapa para obtener el nombre de la textura de explosión
                    auto textureDetails = spellToTexture[spell.spell];
                    drawSpellExp(engine, std::get<1>(textureDetails));

                    auto& inpi = em.getSingleton<InputInfo>();
                    auto& txti = em.getSingleton<TextInfo>();

                    if (inpi.interact && elapsed_book >= 1.0f)
                    {
                        txti.notPass = false;
                        plfi.showBook = false;
                        inpi.interact = false;
                        elapsed_book = 0.f;
                    }
                    else
                        txti.notPass = true;
                }
            }
        }
    }
}

void RenderSystem::drawSpellExp(GameEngine& engine, std::string name)
{
    // Dibujamos textura del libro
    auto& libroText = engine.textures["libro"];

    // Calculamos la posición inicial fuera de la pantalla
    int initialPosY = -libroText.height;
    int posX = engine.getScreenWidth() / 2 - libroText.width / 2;

    // Calculamos la posición final en el centro de la pantalla
    int finalPosY = engine.getScreenHeight() / 2 - libroText.height / 2;

    // Animamos la posición Y
    int posY = initialPosY + static_cast<int>(static_cast<float>(finalPosY - initialPosY) * elapsed_book);

    engine.drawTexture(libroText, posX, posY, { 255, 255, 255, 255 });

    // Dibujamos el gif de la explicación por encima
    auto& gif = engine.gifs.at(name);
    auto copy = gif.texture;

    // Redimensionamos la copia
    copy.width = static_cast<int>(copy.width / gif.reScaleX);
    copy.height = static_cast<int>(copy.height / gif.reScaleY);

    // Calculamos la posición inicial y final para el gif
    initialPosY = -copy.height;
    finalPosY = engine.getScreenHeight() / 2 - copy.height / 2;

    // Animamos la posición Y del gif
    posY = initialPosY + static_cast<int>(static_cast<float>(finalPosY - initialPosY) * elapsed_book);

    posX = engine.getScreenWidth() / 2 - copy.width / 2;

    displayGif(engine, copy, gif, posX, posY);

    // Incremento
    elapsed_book += timeStep * 0.5f;

    if (elapsed_book > 1.0f)
        elapsed_book = 1.0f;
}

void RenderSystem::drawStaff(GameEngine& engine, EntityManager& em)
{
    auto& plfi{ em.getSingleton<PlayerInfo>() };

    if (plfi.hasStaff)
    {
        handleAnimatedTexture("4_pl", "placeholder", engine.getScreenWidth() - 140, 45, engine.textures["placeholder"], 2.75f);
        handleAnimatedTexture("palo", "palo", engine.getScreenWidth() - 125, 60, engine.textures["palo"], 2.85f);
    }
}

void RenderSystem::drawAnimatedTextures(GameEngine& engine)
{
    for (auto& [_, textureInfo] : animatedTextures)
    {
        // Calculamos el factor de escala
        textureInfo.scaleFactor = 3.5f - textureInfo.scaleChange * textureInfo.lerpFactor;

        // Calcula la posición del centro de la pantalla
        int centerX = static_cast<int>(static_cast<float>(engine.getScreenWidth() / 2) - static_cast<float>(textureInfo.width) * textureInfo.scaleFactor / 2);
        int centerY = static_cast<int>(static_cast<float>(engine.getScreenHeight() / 2 - (50)) - static_cast<float>(textureInfo.height) * textureInfo.scaleFactor / 2);

        // Interpola entre el centro de la pantalla y la posición objetivo
        int posX = static_cast<int>(static_cast<float>(centerX) + textureInfo.lerpFactor * static_cast<float>(textureInfo.targetPosX - centerX));
        int posY = static_cast<int>(static_cast<float>(centerY) + textureInfo.lerpFactor * static_cast<float>(textureInfo.targetPosY - centerY));

        // Si el tiempo transcurrido es menor que 1.5 segundos, no hagas nada
        if (textureInfo.elapsed < 2.5f)
        {
            // Incrementamos el tiempo transcurrido
            textureInfo.elapsed += timeStep;
        }
        else
        {
            // Incrementamos el factor de interpolación
            textureInfo.lerpFactor += textureInfo.lerpSpeed;
            if (textureInfo.lerpFactor > 1.0f)
                textureInfo.lerpFactor = 1.0f;
        }

        engine.drawTexture(engine.textures[textureInfo.textureName], posX, posY, { 255, 255, 255, 255 }, textureInfo.scaleFactor);
    }
}

void RenderSystem::drawTextBox(GameEngine& engine, EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& txti = em.getSingleton<TextInfo>();
    auto& textQueue = txti.getTextQueue();

    float boxWidth = 600;
    float boxHeight = 100;

    // Centramos la posición del cuadro de texto
    float posX = static_cast<float>(engine.getScreenWidth() / 2) - boxWidth / 2;
    float posY = static_cast<float>(engine.getScreenHeight() / 1.25) - boxHeight / 2;

    // Sacamos el texto en formatos para GuiTextBox
    auto& str = textQueue.front();
    auto text = const_cast<char*>(str.c_str());

    GuiSetStyle(DEFAULT, TEXT_LINE_SPACING, 20);

    // Dibujamos el cuadro de diálogo con RayGui
    engine.drawRectangle(static_cast<int>(posX), static_cast<int>(posY), static_cast<int>(boxWidth), static_cast<int>(boxHeight), WHITE);
    GuiTextBox({ posX, posY, boxWidth, boxHeight }, text, static_cast<int>(str.size()), false);

    auto& inpi = em.getSingleton<InputInfo>();
    if (inpi.interact && !txti.notPass)
    {
        txti.popText();
        inpi.interact = false;

        if (textQueue.empty() && li.openChest)
        {
            em.getSingleton<SoundSystem>().sonido_cerrar_cofre();
            li.openChest = false;
        }
    }

    int posButtonX = static_cast<int>(posX + boxWidth - 8);
    int posButtonY = static_cast<int>(posY + boxHeight - 8);
    if (textQueue.size() > 1)
    {
        auto& sig = engine.textures["sig"];
        auto copy = sig;
        copy.width /= 2;
        copy.height /= 2;

        posButtonX -= copy.width;
        posButtonY -= copy.height;

        engine.drawTexture(copy,
            posButtonX,
            posButtonY,
            { 255, 255, 255, 255 });

        posButtonY += copy.height;
    }

    GameEngine::Gif* gif;
    Texture2D gifCopy;
    int rest = 5;
    if (engine.isGamepadAvailable(0))
    {
        gif = &engine.gifs.at("x");
    }
    else
    {
        gif = &engine.gifs.at("e");
        rest = 0;
    }

    gifCopy = gif->texture;

    // Redimensionamos la copia
    gifCopy.width = static_cast<int>(gifCopy.width / 3.5);
    gifCopy.height = static_cast<int>(gifCopy.height / 3.5);

    posButtonX -= gifCopy.width;
    posButtonY -= gifCopy.height - rest;

    displayGif(engine, gifCopy, *gif, posButtonX, posButtonY);
}

void RenderSystem::displayGif(GameEngine& engine, Texture2D& copy, GameEngine::Gif& gif, int& posX, int& posY)
{
    engine.drawTexture(copy, posX, posY, WHITE);
    engine.updateGif(gif);
}

double RenderSystem::shakeDouble(double value)
{
    return value += static_cast<double>(std::rand() % 3 - 1) / 5.0;
}

void RenderSystem::drawFPSCounter(GameEngine& engine)
{
    static double lastTime = 0.0;
    static int framesCounter = 0;
    static double fps = 0.0;

    double currentTime = engine.getTime(); // Suponiendo que tienes una función que devuelve el tiempo actual
    framesCounter++;

    if (currentTime - lastTime >= 1.0) // Si ha pasado más de un segundo
    {
        fps = framesCounter; // El número de frames dibujados en el último segundo es el FPS
        framesCounter = 0; // Reinicia el contador de frames
        lastTime = currentTime; // Actualiza el último tiempo
    }

    // Dibuja el FPS
    std::string fpsStr = "FPS: " + std::to_string(fps);
    engine.drawText(fpsStr.c_str(), engine.getScreenWidth() - 100, 10, 20, RED);
}

void RenderSystem::drawBoatParts(GameEngine& ge, EntityManager& em)
{
    auto& plfi = em.getSingleton<PlayerInfo>();

    if (elapsed_boat < elapsed_limit_boat)
    {
        elapsed_boat += timeStep;
        if (elapsed_boat > elapsed_limit_boat) elapsed_boat = elapsed_limit_boat;
    }

    // Dibujamos la textura de la barra que sale desde la derecha
    auto& barca = ge.textures["barco"];

    // Calculamos la posición inicial y final de la barra
    int initialPosX = 0 - barca.width;
    int finalPosX = 0;

    // Interpolamos la posición X
    int posX = initialPosX - static_cast<int>(static_cast<float>(initialPosX - finalPosX) * elapsed_boat);
    int posY = ge.getScreenHeight() / 4;

    // Dibujamos la barra
    ge.drawTexture(barca, posX, posY, { 255, 255, 255, 255 });

    // Dibujamos el número de partes de barco encontradas
    auto& textureNum = ge.textures.at(std::to_string(plfi.boatParts.size()));
    auto& texture4 = ge.textures.at("4");
    auto& textureBar = ge.textures.at("barra");

    // Dibujamos el num / 4
    ge.drawTexture(textureNum, posX + 35, posY + 2, { 255, 255, 255, 255 });
    ge.drawTexture(textureBar, posX + 20 + textureNum.width / 2, posY + 2, { 255, 255, 255, 255 });
    ge.drawTexture(texture4, posX + 25 + textureNum.width / 2 + textureBar.width / 2, posY + 20, { 255, 255, 255, 255 });
}

// void RenderSystem::drawSpellSlots(GameEngine& engine, EntityManager& em)
// {
//     auto& plfi{ em.getSingleton<PlayerInfo>() };

//     if (!plfi.spells.empty())
//     {
//         std::string exp_name = "";
//         std::map<int, std::pair<int, int>> spellPositions = {
//             {0, {engine.getScreenWidth() - 260, 35}},
//             {1, {engine.getScreenWidth() - 200, 125}},
//             {2, {engine.getScreenWidth() - 100, 165}}
//         };

//         for (std::size_t i = 0; i < plfi.spellSlots.size(); i++)
//         {
//             auto& spell = plfi.spellSlots[i];
//             if (spell.spell != plfi.noSpell)
//             {
//                 int spellX, spellY;
//                 std::string spellName = "hechizo" + std::to_string(i + 1);
//                 switch (i)
//                 {
//                 case 0:
//                 {
//                     if (animatedTextures.find("1_pl") == animatedTextures.end())
//                     {
//                         animatedTextures["1_pl"] = { "placeholder", engine.getScreenWidth() - 270, 20,
//                         engine.textures["placeholder"].width, engine.textures["placeholder"].height, 2.5f };
//                     }
//                     else
//                     {
//                         animatedTextures["1_pl"].targetPosX = engine.getScreenWidth() - 270;
//                     }
//                     spellX = engine.getScreenWidth() - 260;
//                     spellY = 35;
//                     break;
//                 }
//                 case 1:
//                 {
//                     if (animatedTextures.find("2_pl") == animatedTextures.end())
//                     {
//                         animatedTextures["2_pl"] = { "placeholder", engine.getScreenWidth() - 210, 110,
//                         engine.textures["placeholder"].width, engine.textures["placeholder"].height, 2.5f };
//                     }
//                     else
//                     {
//                         animatedTextures["2_pl"].targetPosX = engine.getScreenWidth() - 210;
//                     }
//                     spellX = engine.getScreenWidth() - 200;
//                     spellY = 125;
//                     break;
//                 }
//                 case 2:
//                 {
//                     if (animatedTextures.find("3_pl") == animatedTextures.end())
//                     {
//                         animatedTextures["3_pl"] = { "placeholder", engine.getScreenWidth() - 110, 150,
//                         engine.textures["placeholder"].width, engine.textures["placeholder"].height, 2.5f };
//                     }
//                     else
//                     {
//                         animatedTextures["3_pl"].targetPosX = engine.getScreenWidth() - 110;
//                     }
//                     spellX = engine.getScreenWidth() - 100;
//                     spellY = 165;
//                     break;
//                 }
//                 default:
//                     break;
//                 }

//                 switch (spell.spell)
//                 {
//                 case Spells::WaterBomb:
//                 {
//                     if (animatedTextures.find(spellName) == animatedTextures.end())
//                     {
//                         if (!plfi.showBook)
//                         {
//                             animatedTextures[spellName] = { "pompas", spellX, spellY,
//                             engine.textures["pompas"].width, engine.textures["pompas"].height, 2.5f };
//                         }
//                         else
//                             exp_name = "exp_pompa";
//                     }
//                     else
//                     {
//                         animatedTextures[spellName].targetPosX = spellX;
//                         animatedTextures[spellName].targetPosY = spellY;
//                     }
//                     break;
//                 }
//                 case Spells::WaterDash:
//                 {
//                     if (animatedTextures.find(spellName) == animatedTextures.end())
//                     {
//                         if (!plfi.showBook)
//                         {
//                             animatedTextures[spellName] = { "dash", spellX + 10, spellY,
//                             engine.textures["dash"].width, engine.textures["dash"].height, 2.75f };
//                         }
//                         else
//                             exp_name = "exp_pompa";
//                     }
//                     else
//                     {
//                         animatedTextures[spellName].targetPosX = spellX + 10;
//                         animatedTextures[spellName].targetPosY = spellY + 5;
//                     }
//                     break;
//                 }
//                 case Spells::FireBall:
//                 {
//                     if (animatedTextures.find(spellName) == animatedTextures.end())
//                     {
//                         if (!plfi.showBook)
//                         {
//                             animatedTextures[spellName] = { "bola_fuego", spellX + 10, spellY + 5,
//                             engine.textures["bola_fuego"].width, engine.textures["bola_fuego"].height, 2.75f };
//                         }
//                         else
//                             exp_name = "exp_pompa";
//                     }
//                     else
//                     {
//                         animatedTextures[spellName].targetPosX = spellX + 10;
//                         animatedTextures[spellName].targetPosY = spellY + 5;
//                     }
//                     break;
//                 }
//                 default:
//                     break;
//                 }

//                 if (plfi.showBook)
//                 {
//                     drawSpellExp(engine, exp_name);

//                     auto& inpi = em.getSingleton<InputInfo>();
//                     auto& txti = em.getSingleton<TextInfo>();

//                     if (inpi.interact && elapsed_book >= 1.0f)
//                     {

//                         txti.notPass = false;
//                         plfi.showBook = false;
//                         inpi.interact = false;
//                         elapsed_book = 0.f;
//                     }
//                     else
//                         txti.notPass = true;
//                 }
//             }
//         }
//     }
// }