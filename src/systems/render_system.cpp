#include "render_system.hpp"
#include "../motor/src/darkmoon.hpp"
#include <iomanip>

float ENGI::GameEngine::widthRate = 1.0;
float ENGI::GameEngine::heightRate = 1.0f;

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
    drawParticles(em, engine);

    // Terminamos el frame
    endFrame(engine, em);
}

void RenderSystem::restartScene(GameEngine& engine)
{
    auto* tresde = getNode(engine, "3D");
    auto* particles = getNode(engine, "Particles");
    auto* dosde = getNode(engine, "2D");
    auto* menu = getNode(engine, "Menu");
    auto* copyNode = getNode(engine, "Copy");
    auto* textCopy = getNode(engine, "TextCopy");

    particles->clearChildren();
    copyNode->clearChildren();
    textCopy->clearChildren();

    tresde->setVisible(false);
    dosde->setVisible(false);
    menu->setVisible(false);
    dosde->setVisibleOne(true);
}

void RenderSystem::drawLogoGame(GameEngine& engine, EntityManager& em, SoundSystem& ss) {

    auto& li = em.getSingleton<LevelInfo>();

    ss.ambient_stop();
    ss.music_stop();
    ss.ambient_started = false;

    // restartScene(engine);
    engine.beginDrawing();
    engine.clearBackground(D_WHITE);
    getNode(engine, "MenuOpciones")->setVisible(false);
    auto* menuNode = getNode(engine, "MenuPrincipal");

    auto wRate = engine.getWidthRate();
    auto hRate = engine.getHeightRate();

    // Logo del videojuego
    auto* fondoTwoWonders = getNode(engine, "fondo_inicio");
    auto* logoTwoWonders = getNode(engine, "logo_twowonders");

    auto* fondoText = dynamic_cast<Texture2D*>(fondoTwoWonders->getEntity())->texture;
    auto* logoText = dynamic_cast<Texture2D*>(logoTwoWonders->getEntity())->texture;

    auto fondoWidth = fondoText->getWidth();
    auto fondoHeight = fondoText->getHeight();
    auto logoWidth = logoText->getWidth();
    auto logoHeight = logoText->getHeight();

    int middleX = engine.getScreenWidth() / 2;
    int middleY = engine.getScreenHeight() / 2;

    int posBackX = static_cast<int>(static_cast<float>(middleX) - static_cast<float>(fondoWidth) * wRate / 2);
    int posBackY = static_cast<int>(static_cast<float>(middleY) - static_cast<float>(fondoHeight) * hRate / 2);

    int posX = static_cast<int>(static_cast<float>(middleX) - static_cast<float>(logoWidth) * wRate / 2);
    int posY = static_cast<int>(static_cast<float>(middleY) - static_cast<float>(logoHeight) * hRate / 1.5f);

    engine.drawNode(fondoTwoWonders, { posBackX, posBackY });
    engine.drawNode(logoTwoWonders, { posX, posY });

    if (!li.anyButtonPressed)
    {
        engine.createText({ engine.getScreenWidth() / 2, static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 1.2f) },
            "[Pulsa cualquier tecla]", engine.getDefaultFont(), 40, D_LAVENDER_LIGHT, "Texto inicio", menuNode, Aligned::CENTER);

        if (engine.isAnyKeyPressed())
        {
            auto& inpi = em.getSingleton<InputInfo>();
            inpi.interact = false;
            li.anyButtonPressed = true;
            ss.seleccion_menu();
        }
    }
    else
    {
        getNode(engine, "Texto inicio")->getEntity<Text>()->text = L" ";
        drawPauseMenu(engine, em, li, ss);
    }

    // Dibujar
    engine.traverseRoot();
    engine.endDrawing();
}

void RenderSystem::drawChargeScreen(GameEngine& engine, EntityManager& em)
{
    engine.clearBackground({ 171, 159, 197, 255 });
    auto* chargeGif = getNode(engine, "carga");

    // Redimensionamos
    auto& gifInfo = *chargeGif->getEntity<Gif>();
    auto& frames = gifInfo.frames;
    auto& currentFrame = gifInfo.currentFrame;

    auto wRate = engine.getWidthRate();
    auto hRate = engine.getHeightRate();

    float width = static_cast<float>(frames[currentFrame]->getWidth());
    float height = static_cast<float>(frames[currentFrame]->getHeight());

    int posX = static_cast<int>(engine.getScreenWidth() / 2 - static_cast<int>(width * wRate / 2));
    int posY = static_cast<int>(engine.getScreenHeight() / 2 - static_cast<int>(height * hRate / 2));

    engine.drawNode(chargeGif, { posX, posY });

    auto& li = em.getSingleton<LevelInfo>();
    li.loadingTime += timeStep;
}

void RenderSystem::drawControls(EntityManager& em, GameEngine& engine)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();

    if ((engine.isKeyDown(D_KEY_E) && inpi.interact) || engine.isKeyDown(D_KEY_ESCAPE))
    {
        li.currentScreen = li.previousScreen;
        li.previousScreen = li.evenMorePreviousScreen;
        inpi.interact = false;
    }

    auto text = "mando_explicacion";
    if (li.keyboardControls)
        text = "teclado_explicacion";

    engine.beginDrawing();
    engine.clearBackground(D_GRAY);

    restartScene(engine);

    // Controles
    auto controles = getNode(engine, text);
    auto cont = dynamic_cast<Texture2D*>(controles->getEntity());

    int aux_width = cont->texture->getWidth();
    int aux_height = cont->texture->getHeight();

    float reScaleX = engine.getWidthRate();
    float reScaleY = engine.getHeightRate();

    int posX = static_cast<int>(static_cast<float>(engine.getScreenWidth()) / 2 - (static_cast<float>(aux_width) * reScaleX) / 2);
    int posY = static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 2 - (static_cast<float>(aux_height) * reScaleY) / 2);

    auto* txtBox = engine.createText({ engine.getScreenWidth() / 2 - 100, engine.getScreenHeight() - 50 },
        "DALE A [E] PARA SALIR", engine.getDefaultFont(), 20, D_WHITE,
        "Texto controles", getNode(engine, "Menu"));

    engine.drawNode(controles, { posX, posY });
    engine.drawNode(txtBox);

    engine.traverseRoot();
    engine.endDrawing();
}

void RenderSystem::drawOptions(GameEngine& engine, EntityManager& em, SoundSystem& ss) {

    restartScene(engine);
    engine.beginDrawing();
    engine.clearBackground(D_WHITE);

    // Fondo Opciones
    auto wRate = engine.getWidthRate();
    auto hRate = engine.getHeightRate();
    auto* fondoTwoWonders = getNode(engine, "fondo_inicio");

    auto* fondoText = fondoTwoWonders->getEntity<Texture2D>()->texture;

    auto fondoWidth = fondoText->getWidth();
    auto fondoHeight = fondoText->getHeight();

    int middleX = engine.getScreenWidth() / 2;
    int middleY = engine.getScreenHeight() / 2;

    int posBackX = static_cast<int>(static_cast<float>(middleX) - static_cast<float>(fondoWidth) * wRate / 2);
    int posBackY = static_cast<int>(static_cast<float>(middleY) - static_cast<float>(fondoHeight) * hRate / 2);

    engine.drawNode(fondoTwoWonders, { posBackX, posBackY });

    auto* menuNode = getNode(engine, "MenuOpciones");
    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();
    auto& currentButton = inpi.currentButton;

    // Datos de los botones
    int middleScreen = engine.getScreenWidth() / 2;
    int buttonWidth = 300;
    int buttonHeight = 50;

    // Título de Opciones
    int posTitleY = engine.getScreenHeight() / 9;
    engine.createText({ middleScreen,  posTitleY }, "Opciones", engine.getFontDefault(), 80, D_WHITE, "Titulo opciones", menuNode, Aligned::CENTER);

    // Posición del slider
    int posX = middleScreen + buttonWidth / 3;
    int posY = static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 2.f);
    int posYVol = static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 1.5f);

    std::string firstOpt = "";
    switch (engine.getScreenWidth())
    {
    case 800:
        firstOpt = "800x600";
        break;
    case 1280:
        firstOpt = "1280x720";
        break;
    case 1920:
        firstOpt = "1920x1080";
        break;
    default:
        firstOpt = "FULLSCREEN";
        break;
    }
    auto* sliderRes = engine.createOptionSlider({ posX, posY }, { buttonWidth, buttonHeight }, D_AQUA, "",
        engine.getFontDefault(), 35, 45, D_AQUA, Aligned::CENTER, Aligned::CENTER, D_AQUA, D_AQUA_LIGHT, D_AQUA_DARK,
        { "800x600", "1280x720", "1920x1080", "FULLSCREEN" }, firstOpt, "Resolucion", menuNode);


    auto* sliderVol = engine.createFloatSlider({ posX, posYVol }, { buttonWidth, buttonHeight }, D_AQUA, "",
        engine.getFontDefault(), 35, 45, D_AQUA, Aligned::CENTER, Aligned::CENTER, D_AQUA, D_AQUA_LIGHT, D_AQUA_DARK, ss.getVolumeMaster(), "Volumen", menuNode);

    std::map<std::string, std::function<void()>> SliderData =
    {
        {"800x600", [&]() { engine.setWindowSize(800, 600); }},
        {"1280x720", [&]() { engine.setWindowSize(1280, 720); }},
        {"1920x1080", [&]() { engine.setWindowSize(1920, 1080); }},
        {"FULLSCREEN", [&]() { engine.setWindowFullScreen(); }}
    };

    // Botones
    std::map<std::string, std::tuple<Node*, std::string, std::function<void()>, vec2i>> buttonData = {
    { "1_volver", { nullptr, "Volver", [&]() {
        li.currentScreen = li.previousScreen;
        ss.seleccion_menu();
        return;
    }, {middleScreen - buttonWidth, static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 1.2f)} } },
    { "2_aceptar", { nullptr, "Aceptar", [&]() {
        auto& sliderInfo = *sliderRes->getEntity<OptionSlider>();
        auto& sliderInfoVol = *sliderVol->getEntity<FloatSlider>();

        auto& action = SliderData[sliderInfo.options[sliderInfo.currentOption]];
        action();

        ss.setVolumeMaster(sliderInfoVol.currentValue);
        ss.seleccion_menu();
        return;
    }, {middleScreen + buttonWidth / 3, static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 1.2f)} } },
    { "3_controles", { nullptr, "Controles", [&]() {
        li.currentScreen = GameScreen::CONTROLS;
        li.previousScreen = GameScreen::OPTIONS;
        ss.seleccion_menu();
    }, {middleScreen - buttonWidth / 3, static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 4.5f)} } },
    { "4_sliderRes", { nullptr, "Resolución", [&]() {
        auto& sliderInfo = *sliderRes->getEntity<OptionSlider>();
        if (inpi.right)
        {
            sliderInfo.nextOption();
            inpi.right = false;
        }
        else if (inpi.left)
        {
            sliderInfo.prevOption();
            inpi.left = false;
        }

    }, {middleScreen - buttonWidth, static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 2.f)} } },
    { "5_sliderVol", { nullptr, "Volumen", [&]() {
        auto& sliderInfo = *sliderVol->getEntity<FloatSlider>();
        if (engine.isKeyDown(D_KEY_RIGHT))
        {
            sliderInfo.nextOption();
            ss.setVolumeMaster(sliderInfo.currentValue);
            inpi.right = false;
        }
        else if (engine.isKeyDown(D_KEY_LEFT))
        {
            sliderInfo.prevOption();
            ss.setVolumeMaster(sliderInfo.currentValue);
            inpi.left = false;
        }
    }, {middleScreen - buttonWidth, posYVol} } }
    };

    int i{ 0 };
    for (auto& [name, data] : buttonData)
    {
        auto& [button, buttonText, action, position] = data;

        // Asignamos el botón
        button = engine.createButton(position, { buttonWidth, buttonHeight },
            buttonText,
            engine.getFontDefault(), 40, D_BLACK,
            Aligned::CENTER, Aligned::CENTER,
            D_LAVENDER, D_LAVENDER_DARK, D_LAVENDER_LIGHT,
            name.c_str(), menuNode);

        // Sacamos la información del botón
        auto& but = *button->getEntity<Button>();
        but.textBox.drawBox = false;

        // Sacamos la información del slider

        if (currentButton == static_cast<std::size_t>(i) && but.state != ButtonState::CLICK)
            but.isCurrent = true;

        // Comprobar estado del boton
        if ((but.state == ButtonState::CLICK || (but.isCurrent && inpi.interact)) && i < 3) {
            action();
            inpi.interact = false;
        }
        else if (i >= 3 && but.isCurrent)
        {
            action();
        }

        i += 1;
    }

    if (inpi.up || inpi.left) {
        currentButton = (currentButton > 0) ? currentButton - 1 : buttonData.size() - 1;
        ss.sonido_mov();
    }
    if (inpi.down || inpi.right) {
        currentButton = (currentButton < buttonData.size() - 1) ? currentButton + 1 : 0;
        ss.sonido_mov();
    }

    engine.traverseRoot();
    engine.endDrawing();
}

void RenderSystem::drawPauseMenu(GameEngine& engine, EntityManager& em, LevelInfo& li, SoundSystem& ss)
{
    auto& inpi = em.getSingleton<InputInfo>();
    // Nodo de los botones
    if (inpi.pause)
        getNode(engine, "2D")->setVisible(false);

    auto* menuNode = getNode(engine, "MenuPrincipal");

    // Datos de los botones
    auto wRate = engine.getWidthRate();
    auto hRate = engine.getHeightRate();

    int buttonWidth = 400;
    int buttonHeight = 75;
    int downRate = static_cast<int>(150.f * hRate);

    int posX = engine.getScreenWidth() / 7 - static_cast<int>(static_cast<float>(buttonWidth) * wRate / 2.f);
    int posY = static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 3.8f) - static_cast<int>(static_cast<float>(buttonHeight) * hRate / 2.f);

    // Fondo de los botones
    int initX = -static_cast<int>(static_cast<float>(engine.getScreenWidth()) / 2.5f);
    int initButtonX = posX + initX;
    int finalX = 0;
    float multiplier = 750.f;
    if (!inpi.pause)
        multiplier = 500.f;

    int movement = static_cast<int>(multiplier * li.elapsedPause);

    if (initX + movement > finalX)
    {
        movement = finalX - initX;
    }
    else
        li.elapsedPause += engine.getFrameTime();

    initX += movement;
    initButtonX += movement;

    engine.createRectangle({ initX, 0 },
        { static_cast<int>(static_cast<float>(engine.getScreenWidth()) / 2.5f), static_cast<int>(static_cast<float>(engine.getScreenHeight()) / hRate) },
        { 0, 0, 0, 120 }, "Fondo_botones", menuNode);

    // Botones
    auto& currentButton = inpi.currentButton;
    engine.clearBackground(D_WHITE);
    std::map<std::string, std::tuple<Node*, std::string, std::function<void()>>> buttonData = {
        { "1_jugar", { nullptr, inpi.pause ? "Reanudar" : "Jugar" , [&]() {
            if (!inpi.pause)
            {
                li.currentScreen = GameScreen::STORY;
                li.anyButtonPressed = false;
            }
            else
                inpi.pause = false;

            li.elapsedPause = 0.f;

            ss.seleccion_menu();
            ss.music_stop();
        } } },
        { "2_opciones", { nullptr, "Opciones", [&]() {
            li.previousScreen = li.currentScreen;
            li.currentScreen = GameScreen::OPTIONS;
            ss.seleccion_menu();
        } } },
        { "3_creditos", { nullptr, "Créditos", [&]() {
        // Por definir
        } } },
        { "4_salir", { nullptr, inpi.pause ? "Ir al menú" : "Salir", [&]() {
            if (!inpi.pause)
            {
                li.gameShouldEnd = true;
                ss.sonido_salir();
            }
            else
            {
                li.currentScreen = GameScreen::TITLE;
                inpi.pause = false;
                engine.nodeClear(menuNode);
                engine.clearBackground(D_WHITE);
            }
        } } }
    };

    int i{ 0 };
    for (auto& [name, data] : buttonData)
    {
        auto& [button, buttonText, action] = data;

        // Asignamos el botón
        button = engine.createButton({ initButtonX, posY + i * downRate }, { buttonWidth, buttonHeight },
            buttonText,
            engine.getFontDefault(), 50, D_BLACK,
            Aligned::CENTER, Aligned::RIGHT,
            D_LAVENDER, D_LAVENDER_DARK, D_LAVENDER_LIGHT,
            name.c_str(), menuNode);

        // Sacamos la información del botón
        auto& but = *button->getEntity<Button>();
        but.textBox.drawBox = false;

        if (currentButton == static_cast<std::size_t>(i) && but.state != ButtonState::CLICK)
            but.isCurrent = true;

        // Comprobar estado del boton
        if (but.state == ButtonState::CLICK || (but.isCurrent && inpi.interact)) {
            action();
            inpi.interact = false;
        }

        i += 1;
    }

    if (inpi.up || inpi.left) {
        currentButton = (currentButton > 0) ? currentButton - 1 : buttonData.size() - 1;
        ss.sonido_mov();
    }
    if (inpi.down || inpi.right) {
        currentButton = (currentButton < buttonData.size() - 1) ? currentButton + 1 : 0;
        ss.sonido_mov();
    }
}

void RenderSystem::drawInventory(GameEngine&, EntityManager&)
{
    // float windowWidth = 450.0f;
    // float windowHeight = 450.0f;
    // float buttonWidth = 200.0f;
    // float buttonHeight = 50.0f;
    // float posButtonX = static_cast<float>(engine.getScreenWidth() / 2) - (buttonWidth / 2);
    // float posButtonY = static_cast<float>(engine.getScreenHeight() / 3) - (buttonHeight / 2);
    // float posX = static_cast<float>(engine.getScreenWidth() / 2) - (windowWidth / 2);
    // float posY = static_cast<float>(engine.getScreenHeight() / 2) - (windowHeight / 2);
    // float augment = 55.f;

    // Rectangle windowRect = {
    //     posX,
    //     posY,
    //     windowWidth,
    //     windowHeight
    // };
    // engine.drawRectangleLinesEx(windowRect, 2, D_BLACK);
    // engine.drawRectangleRec(windowRect, Color{ 255, 255, 255, 178 });
    // engine.drawTextEx(engine.getFontDefault(), "INVENTARIO", vec2d{ windowRect.x + 110, windowRect.y + 20 }, 40, 1, D_BLACK);

    // auto& plfi = em.getSingleton<PlayerInfo>();
    // // auto& inpi = em.getSingleton<InputInfo>();

    // // auto& currentButton = inpi.currentButton;

    // // Definimos los botones
    // auto size = plfi.inventory.size() + 1;
    // std::vector<ButtonRect> buttons(size);
    // for (std::size_t i = 0; i < plfi.inventory.size(); i++) {
    //     buttons[i] = { { posButtonX, posButtonY + 55 * static_cast<float>(i), buttonWidth, buttonHeight }, plfi.inventory[i]->name.c_str(), static_cast<int>(i) };
    // }
    // buttons[plfi.inventory.size()] = { { posButtonX, posButtonY + 55 * static_cast<float>(plfi.inventory.size()), buttonWidth, buttonHeight }, "VOLVER", static_cast<int>(plfi.inventory.size()) };

    // if (plfi.selectedItem == plfi.max)
    // {
    //     // Control de botones de mando para cambiar el botón seleccionado
    //     if (inpi.up || inpi.left) {
    //         currentButton = (currentButton > 0) ? currentButton - 1 : plfi.inventory.size();
    //     }
    //     else if (inpi.down || inpi.right) {
    //         currentButton = (currentButton < plfi.inventory.size()) ? currentButton + 1 : 0;
    //     }

    //     for (std::size_t i = 0; i < buttons.size(); i++) {
    //         ButtonRect& button = buttons[i];
    //         bool isCurrent = (currentButton == i);
    //         if (GuiButton(button.rect, isCurrent ? ("[" + std::string(button.text) + "]").c_str() : button.text) ||
    //             (isCurrent && inpi.interact)) {
    //             currentButton = i;
    //             // Aquí puedes manejar la acción del botón
    //             if (i < plfi.inventory.size()) {
    //                 // Select an item from the inventory
    //                 plfi.selectedItem = plfi.inventory[i]->getID();
    //                 currentButton = 0;
    //                 return;
    //             }
    //             else {
    //                 // "VOLVER" button
    //                 auto& inpi = em.getSingleton<InputInfo>();
    //                 inpi.inventory = false;
    //             }

    //             inpi.interact = false;
    //         }
    //     }
    // }
    // else
    // {
    //     // Dibujamos la descripción del objeto seleccionado
    //     auto& item = *plfi.getItem(plfi.selectedItem);
    //     auto text = const_cast<char*>(item.description.c_str());

    //     GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

    //     float descWidth = 300.f, descHeight = 150.f;
    //     float posDescX = static_cast<float>(engine.getScreenWidth() / 2) - (descWidth / 2.0f);
    //     float posDescY = static_cast<float>(engine.getScreenHeight() / 2) - (descHeight / 1.25f);
    //     GuiTextBox({ posDescX, posDescY, descWidth, descHeight }, text, static_cast<int>(item.description.size()), false);

    //     if (inpi.interact) {
    //         // Press the currently selected button
    //         switch (currentButton) {
    //         case 0: // "USAR"
    //             if (dynamic_cast<Potion*>(&item) != nullptr) {
    //                 auto& potion = static_cast<Potion&>(item);
    //                 plfi.usePotion(potion);
    //                 plfi.selectedItem = plfi.max;
    //                 auto& inpi = em.getSingleton<InputInfo>();
    //                 inpi.inventory = false;
    //                 GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    //                 return;
    //             }
    //             break;
    //         case 1: // "VOLVER"
    //             plfi.selectedItem = plfi.max;
    //             GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    //             break;
    //         }
    //     }
    //     else if (inpi.up || inpi.left) {
    //         // Move the selection up
    //         currentButton = (currentButton > 0) ? currentButton - 1 : 1;
    //     }
    //     else if (inpi.down || inpi.right) {
    //         // Move the selection down
    //         currentButton = (currentButton < 1) ? currentButton + 1 : 0;
    //     }

    //     if (dynamic_cast<Potion*>(&item) != nullptr) {
    //         Rectangle btn1Rec = { posButtonX, posButtonY + 100, buttonWidth, buttonHeight };
    //         if (GuiButton(btn1Rec, (currentButton == 0) ? "[USAR]" : "USAR")) {
    //             auto& potion = static_cast<Potion&>(item);
    //             plfi.usePotion(potion);
    //             plfi.selectedItem = plfi.max;
    //             auto& inpi = em.getSingleton<InputInfo>();
    //             inpi.inventory = false;
    //             GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    //         }
    //     }

    //     // Boton de volver al inventario
    //     Rectangle btn2Rec = { posButtonX, posButtonY + 170, buttonWidth, buttonHeight };
    //     if (GuiButton(btn2Rec, (currentButton == 1) ? "[VOLVER]" : "VOLVER")) {
    //         plfi.selectedItem = plfi.max;
    //         GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    //     }
    // }
}

void RenderSystem::drawLogoKaiwa(GameEngine& engine) {

    engine.beginDrawing();
    engine.clearBackground({ 136, 219, 152, 255 });

    restartScene(engine);

    // DrawLogoKaiwa
    auto* logoKaiwa = getNode(engine, "logo_kaiwa");
    auto* logoText = dynamic_cast<Texture2D*>(logoKaiwa->getEntity())->texture;

    auto width = logoText->getWidth();
    auto height = logoText->getHeight();

    auto wRate = 0.11f / engine.getWidthRate();
    auto hRate = 0.11f / engine.getHeightRate();

    int posX = engine.getScreenWidth() / 2 - static_cast<int>(static_cast<float>(width) * 0.11f / 2);
    int posY = engine.getScreenHeight() / 2 - static_cast<int>(static_cast<float>(height) * 0.11f / 2);

    engine.drawNode(logoKaiwa, { posX, posY }, { wRate, hRate });

    // Dibujar arbol
    engine.traverseRoot();

    engine.endDrawing();
}

void RenderSystem::drawEnding(GameEngine&) {
    // engine.beginDrawing();
    // engine.clearBackground(D_WHITE);

    // // Valores de la caja de texto
    // float boxWidth = 600.f;
    // float boxHeight = 100.f;
    // float posX = static_cast<float>(engine.getScreenWidth() / 2) - (boxWidth / 2.f);
    // float posY = static_cast<float>(engine.getScreenHeight() / 2) - (boxHeight / 2.f);

    // std::string text = "[ENTER] PARA VOLVER AL TÍTULO";

    // if (engine.isGamepadAvailable(0))
    //     text = "[X] PARA VOLVER AL TÍTULO";

    // GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    // GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    // GuiLabelButton(Rectangle{ posX, posY, boxWidth, boxHeight },
    //     "¡Gracias por jugar a nuestra demo!");

    // GuiLabelButton(Rectangle{ posX, posY + 50, boxWidth + 100, boxHeight },
    //     text.c_str());

    // GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    // engine.endDrawing();
}

void RenderSystem::drawStory(GameEngine& engine) {
    engine.beginDrawing();
    engine.clearBackground(D_WHITE);

    restartScene(engine);

    int boxWidth = 700;
    int boxHeight = 400;
    int posX = static_cast<int>(engine.getScreenWidth() / 2 - (boxWidth / 2));
    int posY = static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 2.5f - static_cast<float>(boxHeight / 2));

    if (!nodeExists(engine, "historia")) {
        auto* hist = engine.createNode("historia", getNode(engine, "Dialog"));

        engine.createTextBox({ posX, posY }, { boxWidth, boxHeight }, D_WHITE,
            "¡Bienvenido a la aventura!", engine.getDefaultFont(), 40,
            D_BLACK, Aligned::CENTER, Aligned::CENTER,
            "Texto 1", hist);
        engine.createTextBox({ posX, posY + 50 }, { boxWidth, boxHeight }, D_WHITE,
            "Estas perdido por el bosque y", engine.getDefaultFont(), 40,
            D_BLACK, Aligned::CENTER, Aligned::CENTER,
            "Texto 2", hist);
        engine.createTextBox({ posX, posY + 100 }, { boxWidth, boxHeight }, D_WHITE,
            "debes encontrar a tu maestro.", engine.getDefaultFont(), 40,
            D_BLACK, Aligned::CENTER, Aligned::CENTER,
            "Texto 3", hist);
        engine.createTextBox({ posX, posY + 150 }, { boxWidth, boxHeight }, D_WHITE,
            "¡Mucha suerte!", engine.getDefaultFont(), 40,
            D_BLACK, Aligned::CENTER, Aligned::CENTER,
            "Texto 4", hist);

        engine.createTextBox({ posX, posY + 250 }, { boxWidth, boxHeight }, D_WHITE,
            "", engine.getDefaultFont(), 40,
            D_BLACK, Aligned::CENTER, Aligned::CENTER,
            "Texto 5", hist);

        for (auto& txtEl : hist->getChildren())
            dynamic_cast<TextBox*>(txtEl->getEntity())->drawBox = false;
    }

    auto* hist = getNode(engine, "historia");
    hist->setVisible(true);

    auto auxText = dynamic_cast<TextBox*>(hist->getChildren().back()->getEntity());
    std::string textGame = "DALE A [E] PARA JUGAR";

    if (engine.isGamepadAvailable(0))
        textGame = "DALE A [X] PARA JUGAR";

    auxText->text.setText(textGame);
    engine.traverseRoot();

    engine.endDrawing();
}

void RenderSystem::drawEntities(EntityManager& em, GameEngine& engine)
{
    using SYSCMPs = MP::TypeList<RenderComponent>;
    auto& frti = em.getSingleton<FrustumInfo>();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, RenderComponent& r)
    {
        if ((!e.hasTags(FrustOut{}) && r.meshLoaded && e.hasComponent<ColliderComponent>()))
        {
            auto& col = em.getComponent<ColliderComponent>(e);
            if (frti.bboxIn(col.bbox) == FrustPos::OUTSIDE)
                return;
        }

        if (e.hasTag<EnemyDeathTag>())
            return;

        if (r.visible) {
            Color colorEntidad = r.color;

            // Comprobar el tipo y si es enemigo cambiarle el color
            if (!e.hasTag<PlayerTag>() && e.hasComponent<TypeComponent>()) {
                auto& t{ em.getComponent<TypeComponent>(e) };

                switch (t.type)
                {
                case ElementalType::Neutral:
                    colorEntidad = D_GRAY;
                    break;

                case ElementalType::Water:
                    colorEntidad = D_BLUE;
                    break;

                case ElementalType::Fire:
                    colorEntidad = D_RED;
                    break;

                case ElementalType::Ice:
                    colorEntidad = D_BLUE_LIGHT;
                    break;

                default:
                    break;
                }
            }

            if (e.hasTag<SubjectTag>() && e.hasComponent<SubjectComponent>() && em.getComponent<SubjectComponent>(e).activeShield)
                colorEntidad = D_GREEN;

            if (e.hasComponent<LifeComponent>()) {
                auto& l{ em.getComponent<LifeComponent>(e) };
                if (l.elapsed < l.countdown)
                {
                    colorEntidad = D_CORAL_PINK;

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

                if (e.hasTag<PlayerTag>())
                {
                    // scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 2.4);
                }
                else if (e.hasTag<SlimeTag>())
                {
                    //scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - .6);
                }
                else if (e.hasTag<SnowmanTag>())
                {
                    // scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 3.0);
                }
                else if (e.hasTag<SpiderTag>())
                {
                    // scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 0.7);
                }
                else if (e.hasTag<GolemTag>())
                {
                    // scl = { 0.4, 0.4, 0.4 };
                    pos.setY(pos.y() - 4.0);
                }
                else if (e.hasTag<DummyTag>())
                {
                    // scl = { 0.4, 0.4, 0.4 };
                    pos.setY(pos.y() - 5.7);
                }
                else if (e.hasTag<BossFinalTag>())
                {
                    scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 1.1);
                    colorEntidad = { 125, 125, 125, 255 };
                }
                else if (e.hasTag<SubjectTag>())
                {
                    scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 1.1);
                }
                else if (e.hasTag<CrusherTag>())
                {
                    // scl = { 0.33, 0.33, 0.33 };
                    pos.setY(pos.y() - 8.6);
                }
                else if (e.hasTag<AngryBushTag>())
                {
                    // scl = { 0.33, 0.33, 0.33 };
                    // pos.setY(pos.y() - 0.5);
                }
                else if (e.hasTag<AngryBushTag2>())
                {
                    //pos.setY(pos.y() - 0.5);
                }
                else if (e.hasTag<ChestTag>() || e.hasTag<SpawnTag>() || e.hasTag<LavaTag>() || e.hasTag<SignTag>() || e.hasTag<TableTag>() || e.hasTag<MissionObjTag>())
                {
                    pos.setY(pos.y() - r.offset);
                }
                else if (e.hasTag<DestructibleTag>())
                {
                    pos.setY(pos.y() - r.offset / 1.5);
                }
                else if (e.hasTag<NomadTag>() || e.hasTag<InvestigatorTag>())
                {
                    pos.setY(pos.y() - 3.54);
                }
                else if (e.hasTag<DoorTag>() || e.hasTag<LeverTag>() || e.hasTag<FireBallTag>()
                    || e.hasTag<CoinTag>() || e.hasTag<WaterBombTag>() || e.hasTag<BoatTag>())
                {
                }

                if (r.rotationVec == vec3d::zero())
                    r.rotationVec = { 0.0, -1.0, 0.0 };

                float orientationInDegrees = static_cast<float>(r.orientation * (180.0f / K_PI));

                if (r.node) {
                    r.node->setTranslation({ pos.x(), pos.y(), pos.z() });
                    r.node->setScale({ scl.x(), scl.y(), scl.z() });
                    r.node->setRotation({ r.rotationVec.x(), r.rotationVec.y(), r.rotationVec.z() }, orientationInDegrees);
                    r.node->setVisibleOne(true);
                    /*
                    if (!in)
                    {
                        int orientationInDegreesInt = static_cast<int>(orientationInDegrees);
                        if (orientationInDegreesInt % 90 == 0 && std::abs(orientationInDegreesInt) != 270 && std::abs(orientationInDegreesInt) != 90)
                        {
                            // engine.drawCubeWires(r.position, static_cast<float>(r.scale.x()), static_cast<float>(r.scale.y()), static_cast<float>(r.scale.z()), D_BLACK);
                            //std::cout << "Dibujando cubo\n";
                        }
                        else
                        {
                            // engine.drawModelWires(r.model, pos, r.rotationVec, orientationInDegrees, scl, D_BLACK);
                            //auto eModel = dynamic_cast<ModelType*>(r.node->getEntity());
                            //eModel->drawModel = true;
                            //eModel->drawWires = true;
                        }
                    }
                    */
                }
            }
        }
    });
}

void RenderSystem::loadModels(Entity& e, GameEngine& engine, EntityManager& em, RenderComponent& r)
{
    auto& li = em.getSingleton<LevelInfo>();

    if (e.hasTag<PlayerTag>())
        r.node = engine.loadModel("assets/models/main_character.obj");
    else if (e.hasTag<SlimeTag>())
        r.node = engine.loadModel("assets/models/Slime.obj");
    else if (e.hasTag<SnowmanTag>())
    {
        r.node = engine.loadModel("assets/Personajes/Enemigos/Snowman/Snowman.obj");
        // r.model = engine.loadModel("assets/Personajes/Enemigos/Snowman/Snowman.obj");

        // Texture t{};
        // if (li.mapID != 2)
        //     t = engine.loadTexture("assets/Personajes/Enemigos/Snowman/Snowman_texture.png");
        // else
        //     t = engine.loadTexture("assets/Personajes/Enemigos/Snowman/snowman_fuego_texture.png");

        // Texture2D t0 = engine.loadTexture("assets/models/textures/entity_textures/snowman_uv.png");
        // r.model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = t0;

        // r.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = t;
    }
    else if (e.hasTag<GolemTag>())
        r.node = engine.loadModel("assets/Personajes/Enemigos/Golem/Golem.obj");
    else if (e.hasTag<SpiderTag>())
        r.node = engine.loadModel("assets/models/Spider.obj");
    else if (e.hasTag<BossFinalTag>())
        r.node = engine.loadModel("assets/models/Boss.obj");
    else if (e.hasTag<SubjectTag>())
        r.node = engine.loadModel("assets/models/Boss_sub_1.obj");
    else if (e.hasTag<Chunk0Tag>())
        switch (li.mapID)
        {
        case 0:
            r.node = engine.loadModel("assets/Niveles/Lvl_0/Objs/lvl_0-cnk_0.obj");
            break;

        case 1:
            r.node = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_0.obj");
            break;
        case 2:
            r.node = engine.loadModel("assets/Niveles/Lvl_2/Objs/lvl_2-cnk_0.obj");
            break;
        }
    else if (e.hasTag<Chunk1Tag>())
        switch (li.mapID)
        {
        case 0:
            r.node = engine.loadModel("assets/Niveles/Lvl_0/Objs/lvl_0-cnk_1.obj");
            break;

        case 1:
            r.node = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_1.obj");
            break;
        case 2:
            r.node = engine.loadModel("assets/Niveles/Lvl_2/Objs/lvl_2-cnk_1.obj");
            break;
        }
    else if (e.hasTag<Chunk2Tag>())
        switch (li.mapID)
        {
        case 0:
            r.node = engine.loadModel("assets/Niveles/Lvl_0/Objs/lvl_0-cnk_2.obj");
            break;

        case 1:
            r.node = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_2.obj");
            break;
        case 2:
            r.node = engine.loadModel("assets/Niveles/Lvl_2/Objs/lvl_2-cnk_2.obj");
            break;
        }
    else if (e.hasTag<Chunk3Tag>())
    {
        switch (li.mapID)
        {
        case 0:
            // r.model = engine.loadModelRaylib("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_3.obj");
            r.node = engine.loadModel("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_3.obj");
            break;

        case 1:
            // r.model = engine.loadModelRaylib("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_3.obj");
            r.node = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_3.obj");
            break;
        case 2:
            r.node = engine.loadModel("assets/Niveles/Lvl_2/Objs/lvl_2-cnk_3.obj");
            break;
        }
        // loadShaders(r.model);
    }
    else if (e.hasTag<Chunk4Tag>())
    {
        switch (li.mapID)
        {
        case 0:
            // r.model = engine.loadModelRaylib("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_4.obj");
            r.node = engine.loadModel("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_4.obj");
            break;

        case 1:
            // r.model = engine.loadModelRaylib("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_4.obj");
            r.node = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_4.obj");
            break;
        }
        // loadShaders(r.model);
    }
    else if (e.hasTag<Chunk5Tag>())
    {
        switch (li.mapID)
        {
        case 0:
            // r.model = engine.loadModelRaylib("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_5.obj");
            r.node = engine.loadModel("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_5.obj");
            break;

        case 1:
            // r.model = engine.loadModelRaylib("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_5.obj");
            r.node = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_5.obj");
            break;
        }
        // loadShaders(r.model);
    }
    else if (e.hasTag<Chunk6Tag>())
    {
        switch (li.mapID)
        {
        case 0:
            // r.model = engine.loadModelRaylib("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_6.obj");
            r.node = engine.loadModel("assets/levels/Zona_0-Bosque/objs/lvl_0-cnk_6.obj");
            break;

        case 1:
            // r.model = engine.loadModelRaylib("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_6.obj");
            r.node = engine.loadModel("assets/levels/Zona_1-Mazmorra/objs/versionDevcom/lvl_1-cnk_6.obj");
            break;
        }
        // loadShaders(r.model);
    }
    else if (e.hasTag<ChestTag>())
    {
        // r.model = engine.loadModelRaylib("assets/models/Cofre.obj");
        r.node = engine.loadModel("assets/models/Cofre.obj");
        // loadShaders(r.model);
    }
    else if (e.hasTag<DestructibleTag>())
    {
        switch (li.mapID)
        {
        case 0:
            // r.model = engine.loadModelRaylib("assets/models/Troncos.obj");
            r.node = engine.loadModel("assets/models/Troncos.obj");
            break;

        case 1:
            // r.model = engine.loadModelRaylib("assets/models/Puerta_prision_agua.obj");
            r.node = engine.loadModel("assets/models/Puerta_prision_agua.obj");
            break;
        }
        // loadShaders(r.model);
    }
    else if (e.hasTag<DoorTag>())
    {
        switch (li.mapID)
        {
        case 0:
            // r.model = engine.loadModelRaylib("assets/levels/Zona_0-Bosque/objs/Barricada_cambio_lvl.obj");
            r.node = engine.loadModel("assets/levels/Zona_0-Bosque/objs/Barricada_cambio_lvl.obj");
            break;

        case 1:
            // r.model = engine.loadModelRaylib("assets/models/Puerta_prision_base.obj");
            r.node = engine.loadModel("assets/models/Puerta_prision_base.obj");
            break;
        }

        // loadShaders(r.model);
    }
    else if (e.hasTag<AngryBushTag>())
    {
        // r.model = engine.loadModelRaylib("assets/models/Piedra.obj");
        if (li.mapID == 0)
            r.node = engine.loadModel("assets/Personajes/Enemigos/Piedra/Piedra_2.obj");
        else
            r.node = engine.loadModel("assets/Personajes/Enemigos/Piedra/Piedra_3.obj");

        // loadShaders(r.model);
    }
    else if (e.hasTag<AngryBushTag2>())
    {
        r.node = engine.loadModel("assets/Personajes/Enemigos/Piedra/Piedra_1.obj");
    }
    else if (e.hasTag<CrusherTag>())
    {
        // r.model = engine.loadModelRaylib("assets/models/Apisonadora.obj");
        r.node = engine.loadModel("assets/models/Apisonadora.obj");

        // loadShaders(r.model);
    }
    else if (e.hasTag<DummyTag>())
    {
        // r.model = engine.loadModelRaylib("assets/models/Dummy.obj");
        r.node = engine.loadModel("assets/models/Dummy.obj");

        // if (li.mapID == 2)
        // {
        //     for (int j = 0; j < r.model.materialCount; j++)
        //     {
        //         for (int k = 0; k < 11; k++)
        //             r.model.materials[j].maps[k].texture = engine.loadTexture("assets/Personajes/Enemigos/Dummy/Dummy_fire-texture.png");
        //     }
        // }
    }
    else if (e.hasTag<BarricadeTag>())
    {
        r.node = engine.loadModel("assets/Assets/Barricada_arboles/Barricada_arboles.obj");
    }
    else if (e.hasTag<SpawnTag>())
    {
        r.node = engine.loadModel("assets/Assets/Checkpoint/Checkpoint.obj");
    }
    else if (e.hasTag<LevelChangeTag>())
    {
        r.node = engine.loadModel("assets/Assets/Tp/Tp.obj");
    }
    else if (e.hasTag<LeverTag>())
    {
        r.node = engine.loadModel("assets/Assets/Palanca/Palanca-prision.obj");
    }
    else if (e.hasTag<CoinTag>())
    {
        r.node = engine.loadModel("assets/Assets/Props/Destellos.obj");
    }
    else if (e.hasTag<WaterBombTag>())
    {
        r.node = engine.loadModel("assets/Assets/Props/Hechizos/Agua_1.obj");
    }
    else if (e.hasTag<NomadTag>())
    {
        r.node = engine.loadModel("assets/Personajes/NPCs/Nomada/Nomada.obj");
    }
    else if (e.hasTag<InvestigatorTag>())
    {
        r.node = engine.loadModel("assets/Personajes/NPCs/Investigador/Investigador.obj");
        // loadShaders(r.model);
    }
    else if (e.hasTag<LavaTag>())
    {
        r.node = engine.loadModel("assets/Assets/Charco_lava/Charco_lava.obj");

        // loadShaders(r.model);
    }
    else if (e.hasTag<SignTag>())
    {
        r.node = engine.loadModel("assets/Assets/Cartel/Cartel.obj");

        // loadShaders(r.model);
    }
    else if (e.hasTag<TableTag>())
    {
        r.node = engine.loadModel("assets/Assets/Mesa_investigador/Mesa-investigador.obj");
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
                    r.node = engine.loadModel("assets/Assets/Barca/Barca_base.obj");
                    break;
                }
                case BoatParts::Motor:
                {
                    r.node = engine.loadModel("assets/Assets/Barca/Barca_motor.obj");
                    break;
                }
                case BoatParts::SteeringWheel:
                {
                    r.node = engine.loadModel("assets/Assets/Barca/Barca_volante.obj");
                    break;
                }
                case BoatParts::Propeller:
                {
                    r.node = engine.loadModel("assets/Assets/Barca/Barca_helice.obj");
                    break;
                }
                }
            }
        }

        default:
            break;
        }
    }
    else if (e.hasTag<BoatTag>())
    {
        r.node = engine.loadModel("assets/Assets/Barca/Barca_completa.obj");
    }
    else if (e.hasTag<SnowBallTag>())
    {
        r.node = engine.loadModel("assets/Personajes/Enemigos/Snowman/Snow_ball.obj");
    }
    else if (e.hasTag<MagmaBallTag>())
    {
        r.node = engine.loadModel("assets/Personajes/Enemigos/Snowman/Magma_ball.obj");

        // loadShaders(r.model);
    }
    else
    {
        r.node = engine.createCube({ 0.0f, 0.0f, 0.0f },
            { static_cast<float>(r.scale.x()), static_cast<float>(r.scale.y()), static_cast<float>(r.scale.z()) },
            D_GRAY, "cubo 3D", getNode(engine, "3D"));
    }
    r.meshLoaded = true;
}


void RenderSystem::drawParticles(EntityManager& em, GameEngine& engine)
{
    using partCMPs = MP::TypeList<ColliderComponent, ParticleMakerComponent>;
    using noTAGs = MP::TypeList<>;

    auto& frti = em.getSingleton<FrustumInfo>();
    em.forEach<partCMPs, noTAGs>([&](Entity&, ColliderComponent& col, ParticleMakerComponent& pmc)
    {
        if (frti.bboxIn(col.bbox) == FrustPos::OUTSIDE)
            return;

        if (pmc.active)
        {
            for (auto& p : pmc.particles)
            {
                if (p.type == Particle::ParticleType::Pixel)
                {
                    // Dibujamos 4 partćulas arriba, abajo, izquierda y derecha de la posición
                    engine.drawPoint3D(p.position.to_other<double>(), 3.f, { p.r, p.g, p.b, p.a });
                    // engine.createLine3D(p.position.toGlm(), (p.position + vec3f{ 0.0f, .3f, 0.0f }).toGlm(), 1.5f, { p.r, p.g, p.b, p.a }, "particula", getNode(engine, "Particles"));
                    // engine.drawPoint3D(p.position.to_other<double>(), { p.r, p.g, p.b, p.a });
                    // engine.drawPoint3D((p.position + vec3f{ 0.0f, 0.1f, 0.0f }).to_other<double>(), { p.r, p.g, p.b, p.a });
                    // engine.drawPoint3D((p.position + vec3f{ 0.1f, 0.0f, 0.0f }).to_other<double>(), { p.r, p.g, p.b, p.a });
                    // engine.drawPoint3D((p.position + vec3f{ 0.0f, -0.1f, 0.0f }).to_other<double>(), { p.r, p.g, p.b, p.a });
                    // engine.drawPoint3D((p.position + vec3f{ -0.1f, 0.0f, 0.0f }).to_other<double>(), { p.r, p.g, p.b, p.a });
                }
            }
        }
    });

}

// Empieza el dibujado y se limpia la pantalla
void RenderSystem::beginFrame(GameEngine& engine, EntityManager& em)
{
    restartScene(engine);
    engine.beginDrawing();

    auto& li = em.getSingleton<LevelInfo>();

    Color bgColor = D_WHITE;
    switch (li.mapID)
    {
    case 0:
        bgColor = D_WHITE;
        break;
    case 1:
        bgColor = { 103, 49, 71, 255 };
        break;
    }

    engine.clearBackground(bgColor);
}

//dibujar rayo 3d 
void RenderSystem::drawRay(vec3d, vec3d) {
    /*
    BeginDrawing();
    DrawLine3D(origin.toRaylib(), (origin + dir * 100).toRaylib(), RED);
    EndDrawing();
    */
}

// Se termina el dibujado
void RenderSystem::endFrame(GameEngine& engine, EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();
    auto& txti = em.getSingleton<TextInfo>();

    if (li.isCharging())
    {
        restartScene(engine);
        drawChargeScreen(engine, em);
    }
    else
        drawHUD(em, engine);

    if (inpi.debugPhy)
        drawDebugPhysics(engine, em);

    drawAlerts_IA(em, engine);

    if (txti.hasText())
        drawTextBox(engine, em);

    if (inpi.pause)
    {
        auto& ss = em.getSingleton<SoundSystem>();
        drawPauseMenu(engine, em, li, ss);
    }

    // else if (inpi.inventory)
        // drawInventory(engine, em);

    // Si se pulsa F2 se activa editor  de parámetros In-game
    else if (inpi.debugAI1)
        drawEditorInGameIA(engine, em);

    // Visual Debug AI
    else if (inpi.debugAI2)
        drawDebuggerInGameIA(engine, em);

    else if (inpi.pathfind)
        drawTestPathfindinf(engine, em);

    if (li.elapsedPause > 0 && !inpi.pause)
        li.elapsedPause = 0;

    getNode(engine, "TextCopy")->setVisibleOne(true);

    // engine.drawTree();
    engine.traverseRoot();
    engine.endDrawing();
}

// Interfaz para probar el pathfinding
void RenderSystem::drawTestPathfindinf(GameEngine& engine, EntityManager& em) {
    auto& debug = em.getSingleton<Debug_t>();
    auto& navs = em.getSingleton<NavmeshInfo>();
    auto& li = em.getSingleton<LevelInfo>();
    //Dibujado de titulo y ventana
    auto* debugNode = getNode(engine, "DebugAI4");
    engine.createRectangle({ engine.getScreenWidth() - 400, 300 }, { 495, 645 }, { 255, 255, 255, 128 }, "pathfinding", debugNode);
    engine.createText({ engine.getScreenWidth() - 370, 320 }, "PATHFINDING", D_RED, "pathfinding_title", debugNode);

    // Datos de los botones
    int buttonWidth = 225;
    int buttonHeight = 45;
    int posX = engine.getScreenWidth() - 370;
    int posY = 350;

    // Slider para startnode
    // float startMinValue = 1.0f;
    // float startMaxValue = 100.0f;
    // const char* startNodeText = "Start Node";
    // posX = 600.0f; // Reseteamos la posición X
    // posY = 355.0f; // Posición Y para el slider de startnode
    // int startnodenew = GuiSliderBar(Rectangle(posX, posY, buttonWidth, buttonHeight), startNodeText, NULL, &debug.startnode, startMinValue, startMaxValue);
    // engine.drawText(std::to_string(static_cast<int>(debug.startnode)).c_str(), static_cast<int>(posX + 160), static_cast<int>(posY), 20, D_BLUE);
    // startnodenew += 1;
    // // Slider para goalnode
    // float goalMinValue = 1.0f;
    // float goalMaxValue = 100.0f;
    // const char* goalNodeText = "Goal Node";
    // int goalnodenew = GuiSliderBar(Rectangle(posX, posY + 40, buttonWidth, buttonHeight), goalNodeText, NULL, &debug.goalnode, goalMinValue, goalMaxValue);
    // engine.drawText(std::to_string(static_cast<int>(debug.goalnode)).c_str(), static_cast<int>(posX + 160), static_cast<int>(posY + 40), 20, D_BLUE);
    // goalnodenew += 1;

    auto& butt1 = *engine.createButton({ posX - 10, posY + 80 }, { buttonWidth, buttonHeight }, "CALCULATE", "pathfinding_butt1", debugNode)->getEntity<Button>();
    auto& butt2 = *engine.createButton({ posX + 140, posY + 80 }, { buttonWidth, buttonHeight }, "CLEAR", "pathfinding_butt2", debugNode)->getEntity<Button>();
    auto& butt3 = *engine.createButton({ posX + 140, posY }, { buttonWidth, buttonHeight }, "CORNERS", "pathfinding_butt3", debugNode)->getEntity<Button>();
    auto& butt4 = *engine.createButton({ posX - 10, posY }, { buttonWidth, buttonHeight }, "CENTERS", "pathfinding_butt4", debugNode)->getEntity<Button>();
    auto& butt5 = *engine.createButton({ posX + 140, posY + 40 }, { buttonWidth, buttonHeight }, "MIDPOINTS", "pathfinding_butt5", debugNode)->getEntity<Button>();
    auto& butt6 = *engine.createButton({ posX - 10, posY + 40 }, { buttonWidth, buttonHeight }, "CONEXIONES", "pathfinding_butt6", debugNode)->getEntity<Button>();

    // Botón
    if (butt1.state == ButtonState::CLICK) {
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
        // uint16_t targetnode = findNearestNode(em, vec3d{ -12.33, 40.0, 22.41 }, navs.nodes);
        std::cout << startnode << "\n";
        //Creamos Grafo
        Graph graph{};
        graph.createGraph(navs.conexiones, navs.nodes);
        std::vector<vec3d> path = graph.PathFindAStar(debug, startnode, 819);

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
    if (butt2.state == ButtonState::CLICK) {
        debug.path.clear();
        debug.nodes.clear();
        debug.closedlist.clear();
    }
    if (butt3.state == ButtonState::CLICK) {
        debug.seecorners = !debug.seecorners;
    }
    if (butt4.state == ButtonState::CLICK) {
        debug.seecenters = !debug.seecenters;
    }
    if (butt5.state == ButtonState::CLICK) {
        debug.seemidpoint = !debug.seemidpoint;
    }
    if (butt6.state == ButtonState::CLICK) {
        debug.seeconex = !debug.seeconex;
    }
    //resultado
    engine.createText({ posX, 480 }, "PATH RESULT", D_RED, "path_result", debugNode);

    //Dibujar path
    int posyt = 510;
    for (auto pos : debug.path) {
        std::string text = std::to_string(pos.x()) + " " + std::to_string(pos.y()) + " " + std::to_string(pos.z());
        engine.createText({ posX, posyt }, text, D_RED, "path_numbers", debugNode);
        posyt += 20;
    }

    //DIbujar nodos de la lista cerrada y nodos del path resultado
    for (auto& closenode : debug.closedlist) {
        engine.drawCube(closenode, { 2, 2, 2 }, D_YELLOW);
    }
    for (auto& node : debug.nodes) {
        engine.drawCube(node, { 2, 2, 2 }, D_GREEN);
    }

    // for (auto& node : navs.nodes) {
    //     engine.drawCube(node.second, 2, 2, 2, RED);
    // }
//Dibujar corners
    if (debug.seecorners) {
        for (auto& node : navs.corners) {
            engine.drawCube(node, { 2, 2, 2 }, D_RED);
        }
    }
    if (debug.seecenters) {
        for (auto it = navs.centers.begin(); it != std::prev(navs.centers.end()); ++it) {
            engine.drawCube(it->second, { 2, 2, 2 }, D_BLUE);
        }
    }
    if (debug.seemidpoint) {
        for (auto& node : navs.midpoints) {
            engine.drawCube(node, { 2, 2, 2 }, D_VIOLET);
        }
    }
    if (debug.seeconex) {
        for (auto& conex : navs.conexpos) {
            engine.drawLine3D(conex.first, conex.second, D_GREEN);
        }
        for (auto& bbox : navs.boundingnavmesh) {
            auto boxSize = bbox.max - bbox.min;
            vec3d boxPosition = (bbox.min + bbox.max) / 2;
            //boxPosition.setY(boxPosition.y + 20.0);
            engine.drawCubeWires(boxPosition,
                { static_cast<float>(boxSize.x()),
                static_cast<float>(boxSize.y()),
                static_cast<float>(boxSize.z()), },
                D_VIOLET);
        }
    }

    for (auto& node : debug.nodes) {
        std::string text = std::to_string(node.x()) + " " + std::to_string(node.y()) + " " + std::to_string(node.z());
        int posx = static_cast<int>(engine.getWorldToScreenX(node));
        int posy = static_cast<int>(engine.getWorldToScreenY(node));
        engine.createText({ posx, posy }, text, D_RED, "path_numbers2", debugNode, 15);
    }

    // debugNode->setTranslation({ engine.getScreenWidth() / 2, 0.0f, 0.0f });
    // engine.drawNode(debugNode);
}

//Debugger visual in-game
void RenderSystem::drawDebuggerInGameIA(GameEngine& engine, EntityManager& em)
{
    auto& debugsnglt = em.getSingleton<Debug_t>();

    int posX = engine.getScreenWidth() - 330;
    int posText = static_cast<int>(posX + 10);

    auto* debugNode = getNode(engine, "DebugAI2");
    engine.createRectangle({ posX, 80 }, { 330, 230 }, { 255, 255, 255, 128 }, "debugAI2_rect", debugNode);
    engine.createText({ posText, 90 }, "INFO", D_BLACK, "debugAI2_info", debugNode);

    using SYSCMPss = MP::TypeList<AIComponent, ColliderComponent, RenderComponent>;
    using SYSTAGss = MP::TypeList<EnemyTag>;

    // AQUI PONDRIA
    em.forEach<SYSCMPss, SYSTAGss>([&](Entity& e, AIComponent& aic, ColliderComponent& col, RenderComponent& ren)
    {
        RayCast ray = engine.getMouseRay();
        if (col.bbox.intersectsRay(ray.origin, ray.direction) && !(col.behaviorType & BehaviorType::STATIC || col.behaviorType & BehaviorType::ZONE)) {
            if (engine.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                isSelectedfordebug = !isSelectedfordebug;
                debugsnglt.IA_id_debug = e.getID();
            }
        }
        if (isSelectedfordebug && e.getID() == debugsnglt.IA_id_debug) {
            auto& bb = em.getSingleton<BlackBoard_t>();
            engine.drawCubeWires(ren.position, { ren.scale.x(), ren.scale.y(), ren.scale.z() }, D_VIOLET_DARK);
            engine.createText({ posText, 110 }, "ID:", D_BLACK, "debugAI2_id", debugNode);
            engine.createText({ posText + 90 ,110 }, std::to_string(e.getID()), D_GRAY, "debugAI2_id2", debugNode);
            engine.createText({ posText, 130 }, "Node active:", D_BLACK, "debugAI2_node", debugNode);
            // std::cout << debugsnglt.elapsed << "\n";
             // std::cout << debugsnglt.countdown << "\n";
            if (debugsnglt.elapsed >= debugsnglt.countdown) {
                debugsnglt.elapsed = 0;
                debugsnglt.text = aic.bh;
            }
            else {
                debugsnglt.plusDeltatime(timeStep, debugsnglt.elapsed);
            }
            if (!debugsnglt.text)
                debugsnglt.text = "a";
            engine.createText({ posText + 130, 130 }, debugsnglt.text, D_GRAY, "debugAI2_text", debugNode);

            engine.createText({ posText, 150 }, "TEID:", D_BLACK, "debugAI2_teid", debugNode);
            engine.createText({ posText + 90, 150 }, std::to_string(aic.teid), D_GRAY, "debugAI2_teid2", debugNode);

            engine.createText({ posText, 170 }, "TX:", D_BLACK, "debugAI2_tx", debugNode);
            engine.createText({ posText + 80, 170 }, std::to_string(aic.tx), D_GRAY, "debugAI2_tx2", debugNode);

            engine.createText({ posText, 190 }, "TZ:", D_BLACK, "debugAI2_tz", debugNode);
            engine.createText({ posText + 80, 190 }, std::to_string(aic.tz), D_GRAY, "debugAI2_tz2", debugNode);

            engine.createText({ posText, 210 }, "Culldown:", D_BLACK, "debugAI2_culldown", debugNode);
            engine.createText({ posText + 90, 210 }, std::to_string(aic.elapsed_shoot), D_GRAY, "debugAI2_culldown2", debugNode);

            engine.createText({ posText, 230 }, "Player Detected?:", D_BLACK, "debugAI2_playerdetected", debugNode);
            engine.createText({ posText + 180, 230 }, (aic.playerdetected == 0) ? "No" : "Sí", D_GRAY, "debugAI2_playerdetected2", debugNode);

            engine.createText({ posText, 250 }, "Player hunted?:", D_BLACK, "debugAI2_playerhunted", debugNode);
            engine.createText({ posText + 180, 250 }, (bb.playerhunted == 0) ? "No" : "Sí", D_GRAY, "debugAI2_playerhunted2", debugNode);

            engine.createText({ posText, 270 }, "Subditos alive:", D_BLACK, "debugAI2_subditos", debugNode);
            engine.createText({ posText + 180, 270 }, std::to_string(bb.subditosData.size()), D_GRAY, "debugAI2_subditos2", debugNode);

            engine.createText({ posText, 290 }, "Subditos id alive:", D_BLACK, "debugAI2_subditosid", debugNode);
            engine.createText({ posText + 180, 290 }, std::to_string(bb.idsubditos.size()), D_GRAY, "debugAI2_subditosid2", debugNode);

            engine.createText({ posText, 310 }, "Alert state:", D_BLACK, "debugAI2_alertstate", debugNode);
            engine.createText({ posText + 180, 310 }, (aic.alert_state == 0) ? "No" : "Sí", D_GRAY, "debugAI2_alertstate2", debugNode);

            //raycast
            if (bb.launched) {
                auto dir = bb.direction * 100;
                engine.drawLine3D(bb.position_origin, dir, D_BLUE);
                bb.launched = false;
            }
            //Cone
            //if(e.hasTag<SnowmanTag>())
            auto& phy = em.getComponent<PhysicsComponent>(e);
            drawVisionCone(phy.position, phy.orientation, bb.horizontalFOV);
            // if(e.hasTag<GolemTag>())
            //     drawVisionCone(bb.conegolem.first, bb.conegolem.second, bb.horizontalFOV);

        }
    });
}

// Dentro de tu clase BTDecisionPlayerDetected, podrías tener un método para dibujar el cono de visión
void RenderSystem::drawVisionCone(vec3d, double, double) {
    // // Calcula las direcciones de las líneas del cono
    // Vector3 direction1 = { static_cast<float>(std::sin(orientation - horizontalFOV / 2.0)), 0.0f, static_cast<float>(std::cos(orientation - horizontalFOV / 2.0)) };
    // Vector3 direction2 = { static_cast<float>(std::sin(orientation + horizontalFOV / 2.0)), 0.0f, static_cast<float>(std::cos(orientation + horizontalFOV / 2.0)) };

    // // Calcula los puntos de inicio de las líneas
    // Vector3 start1 = pos_enemy.toRaylib();
    // Vector3 start2 = pos_enemy.toRaylib();

    // // Calcula los puntos finales de las líneas (multiplica por una distancia adecuada para hacerlas visibles)
    // Vector3 end1 = { start1.x + direction1.x * 10.0f, start1.y + direction1.y * 10.0f, start1.z + direction1.z * 10.0f };
    // Vector3 end2 = { start2.x + direction2.x * 10.0f, start2.y + direction2.y * 10.0f, start2.z + direction2.z * 10.0f };

    // // Dibuja las líneas
    // DrawLine3D(start1, end1, PURPLE);
    // DrawLine3D(start2, end2, PURPLE);
}

//Dibuja Slider en función de los parámetros
double RenderSystem::SelectValue(GameEngine& engine, double value, int posx, int posy, int height, int width, const char* name, Node* parent) {
    // pasamos a float el valor
    float floatValue = static_cast<float>(value) / 100.f;
    // dibujamos el slider para modificar su valor
    auto* slider = engine.createSlider({ posx, posy }, { height, width }, floatValue, D_VIOLET, D_BLUE_LIGHT, name, parent);
    auto& sliderInfo = *slider->getEntity<Slider>();

    floatValue = sliderInfo.valor * 100.f;
    engine.createText({ posx + width + 80, posy + 5 }, std::to_string(floatValue).c_str(), D_BLUE, (std::string(name) + "_text").c_str(), parent);    // seteamos el nuevo valor
    return static_cast<double>(floatValue);
}

uint16_t RenderSystem::findNearestNode(EntityManager& em, const vec3d& position, const std::map<uint16_t, vec3d>& nodes) {
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

//Editor In-Game
void RenderSystem::drawEditorInGameIA(GameEngine& engine, EntityManager& em) {
    // Parametros ventana editor in-game
    int windowRectX = 0;
    int windowRectY = 100;

    int windowRectWidth = 390;
    int windowRectHeight = 550;

    auto* debugAI = getNode(engine, "DebugAI1");
    engine.createTextBox({ windowRectX, windowRectY }, { windowRectWidth, windowRectHeight }, { 255, 255, 255, 128 }, "Editor IA", engine.getFontDefault(), 20, D_BLUE_DARK, Aligned::TOP, Aligned::LEFT, "Editor IA", debugAI);
    engine.createText({ windowRectX + 20, windowRectY + 50 }, "PARAMETROS", engine.getFontDefault(), 20, D_RED, "text_parametros", debugAI);

    auto& debugsglt = em.getSingleton<Debug_t>();
    using SYSCMPss = MP::TypeList<AIComponent, PhysicsComponent, ColliderComponent, RenderComponent>;
    using SYSTAGss = MP::TypeList<EnemyTag>;

    RayCast ray = engine.getMouseRay();
    em.forEach<SYSCMPss, SYSTAGss>([&](Entity& e, AIComponent& aic, PhysicsComponent& phy, ColliderComponent& col, RenderComponent& ren)
    {
        // Comprobar si el rayo intersecta con el collider
        if (col.bbox.intersectsRay(ray.origin, ray.direction) && !(col.behaviorType & BehaviorType::STATIC || col.behaviorType & BehaviorType::ZONE)) {
            if (engine.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                isSelected = !isSelected;
                debugsglt.IA_id = e.getID();
            }
            // si es seleccionada => wires morados
            // no es seleccionada => wires rojos

            engine.drawCubeWires(ren.position, { ren.scale.x(), ren.scale.y(), ren.scale.z() }, D_RED);
        }

        if (isSelected && e.getID() == debugsglt.IA_id) {

            engine.drawCubeWires(ren.position, { ren.scale.x(), ren.scale.y(), ren.scale.z() }, D_VIOLET_DARK);
            // si se seleccionada una entidad se muestra el Editor de parámetros
            if (isSelected) {
                // ID DE LA ENTIDAD SELECCIONADA
                engine.createText({ 15, 170 }, "EID:", D_BLACK, "text_eid", debugAI);
                engine.createText({ 55, 170 }, std::to_string(debugsglt.IA_id).c_str(), D_GRAY, "text_eid_value", debugAI);
                //Detect Radius
                aic.detect_radius = SelectValue(engine, aic.detect_radius, 45.0, 200.0, 120.0, 30.0, "detect_radius", debugAI);
                // Attack Radius
                aic.attack_radius = SelectValue(engine, aic.attack_radius, 45.0, 240.0, 120.0, 30.0, "attack_radius", debugAI);
                // Arrival Radius
                aic.arrival_radius = SelectValue(engine, aic.arrival_radius, 45.0, 280.0, 120.0, 30.0, "arriv_rad", debugAI);
                // Max Speed
                phy.max_speed = SelectValue(engine, phy.max_speed, 45.0, 320.0, 120.0, 30.0, "max_speed", debugAI);
                //COuntdown Perception
                aic.countdown_perception = SelectValue(engine, aic.countdown_perception, 45.0, 360.0, 120.0, 30.0, "countdown_perception", debugAI);
                //Countdown Shoot
                aic.countdown_shoot = SelectValue(engine, aic.countdown_shoot, 45.0, 400.0, 120.0, 30.0, "countdown_shoot", debugAI);
                //Countdown stop
                aic.countdown_stop = SelectValue(engine, aic.countdown_stop, 45.0, 440.0, 120.0, 30.0, "countdown_stop", debugAI);
            }
        }
    });
}

//Dibujado alertas de detección de enemigos
void RenderSystem::drawAlerts_IA(EntityManager& em, GameEngine& engine) {
    for (auto const& e : em.getEntities())
    {
        //Alert state
        if (e.hasTag<EnemyTag>() && e.hasComponent<RenderComponent>() && e.hasComponent<AIComponent>() && !e.hasTag<EnemyDeathTag>()) {
            auto& aic = em.getComponent<AIComponent>(e);
            auto& r = em.getComponent<RenderComponent>(e);

            auto wRate = engine.getWidthRate();
            auto hRate = engine.getHeightRate();
            int barX = static_cast<int>(engine.getWorldToScreenX(r.position));
            int barY = static_cast<int>(engine.getWorldToScreenY(r.position));

            if (!aic.playerdetected) {
                aic.show_icon = true;
            }

            if (aic.playerdetected && aic.show_icon) {
                // vec2d point1 = { barX, barY - 120.0f };
                // vec2d point2 = { barX - 30.0f, barY - 50.0f };
                // vec2d point3 = { barX + 30.0f, barY - 50.0f };
                // //dibujar icono alerta
                // // Dibuja el triángulo
                // engine.drawTriangle(point1, point2, point3, D_BLACK);
                // // Dibuja el signo de exclamación dentro del triángulo
                // engine.drawText("!", static_cast<int>(barX - 2), static_cast<int>(barY - 100), 50, YELLOW);
                auto* icon = engine.createNode(getNode(engine, "detectionIcon"), getNode(engine, "Copy"));
                int offSetX = static_cast<int>(22.5f * wRate);
                int offSetY = static_cast<int>(202.5f * hRate);
                engine.drawNode(icon, { barX - offSetX, barY - offSetY });

                //emepezar contador para borrar
                if (aic.elapsed_show_icon >= aic.countdown_show_icon) {
                    aic.elapsed_show_icon = 0.0;
                    aic.show_icon = false;
                }
                else {
                    aic.plusDeltatime(timeStep, aic.elapsed_show_icon);
                }
            }

            //vec2d center = { barX, barY - 120.0f };
            if (aic.alert_state) {
                // if(e.getID() == 148){
                //     std::cout << aic.endangle << "\n";
                // }
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

                const char* icon = "";
                if (std::abs(aic.endangle) >= 0.0 && std::abs(aic.endangle) <= 180.0) {
                    icon = "Oido_parp1";
                }
                else if (std::abs(aic.endangle) >= 180.0 && std::abs(aic.endangle <= 360.0)) {
                    icon = "Oido_parp2";
                }

                auto* gif = getNode(engine, icon);
                auto& gifInfo = *dynamic_cast<Gif*>(gif->getEntity());
                auto& frames = gifInfo.frames;
                auto& currentFrame = frames[gifInfo.currentFrame];

                int centerX = barX - static_cast<int>(static_cast<float>(currentFrame->getWidth() / 2) * wRate);
                int centerY = barY - static_cast<int>(static_cast<float>(currentFrame->getHeight() / 2) * hRate) * 4;

                engine.drawNode(gif, { centerX, centerY });
            }
            else if (aic.endangle != 0.0f) {
                aic.endangle += aic.increase_angle;
            }
        }
    }
}

// Se dibuja el HUD
void RenderSystem::drawHUD(EntityManager& em, GameEngine& engine)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();

    if (li.isDead)
    {
        li.resetFromDeath = true;
        return;
    }

    if (li.currentScreen == GameScreen::CONTROLS)
    {
        drawControls(em, engine);
        return;
    }

    if (inpi.debugPhy)
        pointedEntity = std::numeric_limits<std::size_t>::max();

    for (auto const& e : em.getEntities())
    {
        if (e.hasTag<PlayerTag>())
            continue;

        if (e.hasTag<CrusherTag>() && !e.hasTag<EnemyDeathTag>()) {
            auto& phy = em.getComponent<PhysicsComponent>(e);
            auto& ai = em.getComponent<AIComponent>(e);

            auto wRate = engine.getWidthRate();
            auto hRate = engine.getHeightRate();

            int posx = static_cast<int>(engine.getWorldToScreenX(phy.position) + 45.f * wRate);
            int posz = static_cast<int>(engine.getWorldToScreenY(phy.position) - 105.f * hRate);

            engine.drawRectangle({ posx, posz }, { 10, 100 }, D_BLACK);

            int barHeight = static_cast<int>((ai.elapsed_shoot / ai.countdown_shoot) * 100);
            engine.drawRectangle({ posx, posz }, { 10, barHeight }, D_BLUE_LIGHT);
        }

        // Vidas HUD
        if (e.hasTag<EnemyTag>() && e.hasComponent<LifeComponent>() && em.getComponent<RenderComponent>(e).visible &&
            !(e.hasTag<AngryBushTag>() || e.hasTag<AngryBushTag2>() || e.hasTag<EnemyDeathTag>()))
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            auto& l{ em.getComponent<LifeComponent>(e) };

            // Datos para la barra para la barra de vida
            auto wRate = engine.getWidthRate();
            int barWidth = 60;
            int barHeight = 6;
            int barX = static_cast<int>(engine.getWorldToScreenX(r.position)) - static_cast<int>(static_cast<float>(barWidth) * wRate / 2);
            int barY = static_cast<int>(engine.getWorldToScreenY(r.position)) - static_cast<int>(r.scale.y() * 10);

            auto* copy = getNode(engine, "Copy");
            auto* bar = engine.createRectangle({ 0, 0 }, { barWidth, barHeight }, { D_GRAY }, "borde_vida", copy);
            engine.drawNode(bar, { barX, barY });

            // Normaliza la vida actual del personaje
            float normalizedLife = (static_cast<float>(l.life) / static_cast<float>(l.maxLife));

            // Calcula la anchura de la barra de vida
            float lifeWidth = static_cast<float>(barWidth) * normalizedLife;

            if (!l.vidaMax())
                lifeWidth = l.life_width + (static_cast<float>(lifeWidth) - static_cast<float>(l.life_width)) * 0.25f;

            // Dibujamos la barra de vida
            auto* lifeBar = engine.createRectangle({ 0, 0 }, { 1, 4 }, { D_RED }, "vida_rect", copy);
            engine.drawNode(lifeBar, { barX, barY }, { lifeWidth, 1.0f });

            l.life_width = lifeWidth;

            // TODO: Dibujar barra de vida del escudo de los súbditos
            // if (e.hasTag<SubjectTag>() && e.hasComponent<SubjectComponent>())
            // {
            //     auto& sub{ em.getComponent<SubjectComponent>(e) };

            //     // Dibujamos una barra de vida para el escudo si es que tiene uno activo
            //     if (sub.activeShield)
            //     {
            //         engine.drawRectangle(barX, barY - 10, barWidth, barHeight, D_GRAY);
            //         float normalizedShieldLife = (static_cast<float>(sub.shieldLife) / static_cast<float>(sub.maxShieldLife));

            //         // Calcula la anchura de la barra de vida
            //         int shieldWidth = static_cast<int>(static_cast<float>(barWidth) * normalizedShieldLife);

            //         if (sub.shieldLife != sub.maxShieldLife)
            //             shieldWidth = sub.shieldLifeWidth + static_cast<int>((shieldWidth - sub.shieldLifeWidth) * 0.25);

            //         // Dibujamos la barra de vida
            //         engine.drawRectangle(barX, barY - 10, shieldWidth, barHeight, GREEN);

            //         sub.shieldLifeWidth = shieldWidth;
            //     }
            // }
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
                std::string text = "";
                int sum;

                if (engine.isGamepadAvailable(0))
                    text = "x";
                else
                {
                    text = "e";
                    sum = 13;
                }

                auto* gif = getNode(engine, text.c_str());
                auto* gifInfo = dynamic_cast<Gif*>(gif->getEntity());
                auto& frames = gifInfo->frames;
                auto width = frames[gifInfo->currentFrame]->getWidth();

                float offSetX = static_cast<float>(width / 2);
                if (e.hasTag<DoorTag>())
                    offSetX = -static_cast<float>(width / 2 + sum);

                offSetX *= engine.getWidthRate() * 0.75f;

                int posX = static_cast<int>(engine.getWorldToScreenX(pos) - offSetX);
                int posY = static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 11);

                engine.drawNode(gif, { posX, posY }, { 0.75f, 0.75f });

                if (e.hasTag<DoorTag>())
                {
                    auto* lock = getNode(engine, "candado_abierto");
                    auto& lockText = *dynamic_cast<Texture2D*>(lock->getEntity());
                    posX = static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(lockText.texture->getWidth() / 2));
                    posY = static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 13);
                    engine.drawNode(lock, { posX, posY });
                }
            }
            else if (inter.showLock)
            {
                auto* lock = getNode(engine, "candado_cerrado");
                auto& lockText = *dynamic_cast<Texture2D*>(lock->getEntity());
                int posX = static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(lockText.texture->getWidth() / 2));
                int posY = static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 13);
                engine.drawNode(lock, { posX, posY });

                if (e.hasTag<ChestTag>())
                {
                    auto* sword = getNode(engine, "batalla");
                    auto& swordText = *dynamic_cast<Texture2D*>(sword->getEntity());
                    posX = static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(swordText.texture->getWidth() / 2));
                    posY = static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 9.5);
                    engine.drawNode(sword, { posX, posY }, { 0.5f, 0.5f });

                    auto& ch = em.getComponent<ChestComponent>(e);
                    if (ch.closeEnemies > 0)
                    {
                        // Dibujamos el número de partes de barco encontradas
                        auto* copyNode = getNode(engine, "Copy");
                        auto* textureNum = engine.createNode(getNode(engine, std::to_string(ch.closeEnemies).c_str()), copyNode);
                        auto* textureMax = engine.createNode(getNode(engine, std::to_string(ch.maxEnemies).c_str()), copyNode);
                        auto* textureBar = engine.createNode(getNode(engine, "barra"), copyNode);

                        auto* numText = dynamic_cast<Texture2D*>(textureNum->getEntity())->texture;
                        auto* barText = dynamic_cast<Texture2D*>(textureBar->getEntity())->texture;

                        auto posX = static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(numText->getWidth() / 2) - 100);
                        auto posY = static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 12);

                        // Dibujamos el num / 4
                        engine.drawNode(textureNum, { posX, posY });

                        // Dibujamos la barra
                        engine.drawNode(textureBar, { posX + numText->getWidth() / 3, posY });

                        // Dibujamos el num / 4
                        engine.drawNode(textureMax, { posX + numText->getWidth() / 2 + barText->getWidth() / 2, posY });
                    }
                }
            }

            if (e.hasTag<ChestTag>() && li.enemyToChestPos != vec3d::zero() && !li.playerDetected)
            {
                auto& chest = em.getComponent<ChestComponent>(e);
                if (!chest.isOpen)
                {
                    auto& phy = em.getComponent<PhysicsComponent>(e);
                    auto& ch = em.getComponent<ChestComponent>(e);
                    if (phy.position.distance(li.enemyToChestPos) < 50.0 && ch.closeEnemies == 0)
                    {
                        if (elapsed_Lock < elapsed_limit_Lock)
                        {
                            auto* lock = getNode(engine, "candado_abierto");
                            auto& lockText = *dynamic_cast<Texture2D*>(lock->getEntity());
                            int posX = static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(lockText.texture->getWidth() / 2));
                            int posY = static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 9);
                            engine.drawNode(lock, { posX, posY });

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
    }

    auto& pl = *em.getEntityByID(li.playerID);
    // Dibujar las cosas del jugador
    if (pl.hasTag<PlayerTag>())
    {
        // Dibujar Lock In
        drawLockInfo(engine, em);

        // Dibujar vidas restantes del player en el HUD
        if (pl.hasComponent<LifeComponent>())
        {
            updateHealthBar(engine, em, pl);
        }

        // Dibujamos el número de monedas en pantalla
        drawCoinBar(engine, em);

        // drawFPSCounter(engine);
        // engine.node_sceneTextures->clearChildren();

        // Dibujar el bastón
        drawStaff(engine, em);

        // Dibujar espacios de hechizos
        drawSpellSlots(engine, em);

        drawAnimatedTextures(engine);

        if (li.mapID == 2 && li.volcanoMission)
            drawBoatParts(engine, em);

        if ((li.mapID == 0 || li.mapID == 1) && pl.hasComponent<AttackComponent>() && !li.tutorialEnemies.empty())
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

                        std::string gifName = "cuadrado";
                        if (li.lockedEnemy == li.max)
                        {
                            if (engine.isGamepadAvailable(0))
                                gifName = "l2";
                            else
                                gifName = "q";
                        }
                        else
                        {
                            switch (li.mapID)
                            {
                            case 0:
                            {
                                if (engine.isGamepadAvailable(0))
                                    gifName = "r2";
                                else
                                    gifName = "espacio";

                                break;
                            }
                            case 1:
                            {
                                if (engine.isGamepadAvailable(0))
                                    gifName = "cuadrado";
                                else
                                    gifName = "j";
                                break;
                            }
                            }
                        }

                        auto* gif = getNode(engine, gifName.c_str());

                        multiplier = 20.0;

                        if (ene.hasTag<DestructibleTag>())
                            multiplier = 8.0;
                        else if (li.mapID == 1)
                            multiplier = 25.0;

                        auto& textEntity = *dynamic_cast<Gif*>(gif->getEntity());
                        auto& frames = textEntity.frames;
                        auto& currentFrame = textEntity.currentFrame;
                        auto wRate = engine.getWidthRate() * 0.75f;
                        int posX = static_cast<int>(engine.getWorldToScreenX(phy.position) - static_cast<float>(frames[currentFrame]->getWidth()) * wRate / 2);
                        int posY = static_cast<int>(engine.getWorldToScreenY(phy.position) - phy.scale.y() * multiplier);

                        engine.drawNode(gif, { posX, posY }, { 0.75f, 0.75f });
                    }
                }
            }
        }

        if (li.mapID == 0 && li.num_zone == 1 && elapsed_WASD < elapsed_limit_WASD)
        {
            auto& phy{ em.getComponent<PhysicsComponent>(pl) };

            // Mostramos gif de joystick para moverse o texto WASD
            auto text = "wasd";
            if (engine.isGamepadAvailable(0))
            {
                text = "joystick_izq";
            }

            auto joystickL = getNode(engine, text);
            int posX = static_cast<int>(engine.getWorldToScreenX(phy.position) - 40);
            int posY = static_cast<int>(engine.getWorldToScreenY(phy.position) - phy.scale.y() * 37);

            engine.drawNode(joystickL, { posX, posY }, { 0.5f, 0.5f });

            elapsed_WASD += 1.0f / 60.0f;
        }

        if (li.mapID == 2 && pl.hasComponent<AttackComponent>() && !li.volcanoLava.empty())
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

                    // Nombre del gif
                    std::string gifName = "";

                    // Usar el mapa para obtener el nombre del gif
                    if (engine.isGamepadAvailable(0))
                        gifName = spellToGif[spellID].first;
                    else
                        gifName = spellToGif[spellID].second;

                    // Pillamos el nodo del gif
                    auto* gif = getNode(engine, gifName.c_str());
                    auto& gifInfo = *dynamic_cast<Gif*>(gif->getEntity());
                    auto& frames = gifInfo.frames;
                    auto& currentFrame = frames[gifInfo.currentFrame];
                    auto wRate = engine.getWidthRate() * 0.75f;

                    // Posición del gif
                    int posX = static_cast<int>(engine.getWorldToScreenX(phy.position)) - static_cast<int>(static_cast<float>(currentFrame->getWidth()) * wRate / 2);
                    int posY = static_cast<int>(engine.getWorldToScreenY(phy.position) - phy.scale.y() * 10);

                    engine.drawNode(gif, { posX, posY }, { 0.75f, 0.75f });
                }
            }
        }
    }
}

void RenderSystem::drawDebugPhysics(GameEngine& engine, EntityManager& em)
{
    RayCast ray = engine.getMouseRay();
    pointedEntity = std::numeric_limits<std::size_t>::max();
    pointedDistance = std::numeric_limits<double>::max();
    vec3d auxPointed{};
    for (auto const& e : em.getEntities())
    {
        if (e.hasComponent<LifeComponent>() && em.getComponent<RenderComponent>(e).visible)
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            auto const& l{ em.getComponent<LifeComponent>(e) };

            auto point = r.position.y() + r.scale.y() / 2 + 50;
            engine.drawText(std::to_string(l.life).c_str(),
                static_cast<int>(engine.getWorldToScreenX(r.position)),
                static_cast<int>(engine.getWorldToScreenY(r.position) - point),
                20,
                D_BLACK, Aligned::CENTER);

            if (e.hasComponent<TypeComponent>())
            {
                auto const& t{ em.getComponent<TypeComponent>(e) };

                std::string tipo = "Hielo";
                Color color = D_BLUE_LIGHT;

                if (t.type == ElementalType::Neutral)
                {
                    tipo = "Neutral";
                    color = D_BLACK;
                }
                else if (t.type == ElementalType::Water)
                {
                    tipo = "Agua";
                    color = D_BLUE;
                }
                else if (t.type == ElementalType::Fire)
                {
                    tipo = "Fuego";
                    color = D_RED;
                }

                auto point = r.position.y() + r.scale.y() / 2 - 40;
                engine.drawText(tipo.c_str(),
                    static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
                    static_cast<int>(engine.getWorldToScreenY(r.position) - point),
                    20,
                    color);
            }
        }

        if (e.hasComponent<ZoneComponent>())
        {
            auto& ren{ em.getComponent<RenderComponent>(e) };
            auto& z{ em.getComponent<ZoneComponent>(e) };

            engine.drawText(std::to_string(z.zone).c_str(),
                static_cast<int>(engine.getWorldToScreenX(ren.position) - 5),
                static_cast<int>(engine.getWorldToScreenY(ren.position) - ren.scale.y() * 5),
                20,
                D_RED);
        }

        if (e.hasComponent<RampComponent>() && e.hasComponent<PhysicsComponent>())
        {
            // // Dibujamos el rectángulo de la rampa
            auto& phy = em.getComponent<PhysicsComponent>(e);
            auto& rc = em.getComponent<RampComponent>(e);
            // La rampa solo tiene vec2d mínimos y máximos, vamos a dibujar el rectángulo que los une
            engine.drawText(std::to_string(rc.offset.y()).c_str(),
                static_cast<int>(engine.getWorldToScreenX(phy.position) - 5),
                static_cast<int>(engine.getWorldToScreenY(phy.position) - phy.scale.y() * 5),
                20, D_RED);
            engine.drawCubeWires(phy.position, { phy.scale.x(), phy.scale.y(), phy.scale.z() }, D_RED);
        }

        if (e.hasComponent<PhysicsComponent>() && e.hasComponent<ColliderComponent>() && e.hasComponent<RenderComponent>())
        {
            auto& col{ em.getComponent<ColliderComponent>(e) };

            // // Calcular la posición y el tamaño de la bounding box
            vec3d boxPosition = (col.bbox.min + col.bbox.max) / 2;
            vec3d boxSize = col.bbox.max - col.bbox.min;

            Color color = D_BLUE;
            if (col.behaviorType & BehaviorType::ZONE)
                color = D_GREEN;

            bool notStatic = !(col.behaviorType & BehaviorType::ZONE);

            // Comprobar si el rayo intersecta con el collider
            if (col.bbox.intersectsRay(ray.origin, ray.direction, auxPointed) && notStatic)
            {
                auto dist = auxPointed.distance(ray.origin);
                if (pointedDistance > dist)
                {
                    pointedEntity = e.getID();
                    pointedDistance = dist;
                }
            }

            if (e.getID() != pointedEntity)
                engine.drawCubeWires(boxPosition,
                    { static_cast<float>(boxSize.x()),
                    static_cast<float>(boxSize.y()),
                    static_cast<float>(boxSize.z()), },
                    color);

            // Dibujar el ID de las entidades // DEBUG
            // if (e.hasComponent<RenderComponent>())
            // {
            //     auto const& r{ em.getComponent<RenderComponent>(e) };
            //     auto point = r.position.y() + r.scale.y() / 2;
            //     engine.drawText(std::to_string(e.getID()).c_str(),
            //         static_cast<int>(engine.getWorldToScreenX(r.position)),
            //         static_cast<int>(engine.getWorldToScreenY(r.position) - point - 30),
            //         20,
            //         c);
            // }
        }
    }

    if (pointedEntity != std::numeric_limits<std::size_t>::max())
    {
        auto& e = *em.getEntityByID(pointedEntity);
        auto& phy = em.getComponent<PhysicsComponent>(e);
        engine.drawCubeWires(phy.position, { phy.scale.x(), phy.scale.y(), phy.scale.z() }, D_RED);

        // Dibujar el HUD de debug
        auto posTextX = 15;
        engine.drawNode(getNode(engine, "debugRectPhy"));
        auto* debugNode = getNode(engine, "DebugPhy");
        engine.createText({ posTextX,70 }, "Posición", D_BLACK, "debugPhyPos", debugNode);
        std::string posX = "X: " + std::to_string(phy.position.x());
        engine.createText({ posTextX,95 }, posX.c_str(), D_BLACK, "debugPhyPosX", debugNode);
        std::string posY = "Y: " + std::to_string(phy.position.y());
        engine.createText({ posTextX,120 }, posY.c_str(), D_BLACK, "debugPhyPosY", debugNode);
        std::string posZ = "Z: " + std::to_string(phy.position.z());
        engine.createText({ posTextX,145 }, posZ.c_str(), D_BLACK, "debugPhyPosZ", debugNode);

        engine.createText({ posTextX,175 }, "Escala", D_BLACK, "debugPhyScale", debugNode);
        std::string sclX = "X: " + std::to_string(phy.scale.x());
        engine.createText({ posTextX,200 }, sclX.c_str(), D_BLACK, "debugPhyScaleX", debugNode);
        std::string sclY = "Y: " + std::to_string(phy.scale.y());
        engine.createText({ posTextX,225 }, sclY.c_str(), D_BLACK, "debugPhyScaleY", debugNode);
        std::string sclZ = "Z: " + std::to_string(phy.scale.z());
        engine.createText({ posTextX,250 }, sclZ.c_str(), D_BLACK, "debugPhyScaleZ", debugNode);

        engine.createText({ posTextX,280 }, "Velocidad", D_BLACK, "debugPhyVel", debugNode);
        std::string velX = "X: " + std::to_string(phy.velocity.x());
        engine.createText({ posTextX,305 }, velX.c_str(), D_BLACK, "debugPhyVelX", debugNode);
        std::string velY = "Y: " + std::to_string(phy.velocity.y());
        engine.createText({ posTextX,330 }, velY.c_str(), D_BLACK, "debugPhyVelY", debugNode);
        std::string velZ = "Z: " + std::to_string(phy.velocity.z());
        engine.createText({ posTextX,355 }, velZ.c_str(), D_BLACK, "debugPhyVelZ", debugNode);

        std::string id = "ID: " + std::to_string(e.getID());
        engine.createText({ posTextX,385 }, id.c_str(), D_BLACK, "debugPhyID", debugNode);
    }
}

void RenderSystem::drawLockInfo(GameEngine& ge, EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();

    // Dibujar el lock in
    std::size_t enemyID = li.max;
    Color color;

    if (li.lockedEnemy != li.max)
    {
        enemyID = li.lockedEnemy;
        color = D_WHITE;
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
            auto wRate = ge.getWidthRate();
            auto hRate = ge.getHeightRate();
            if (color.a == 100)
            {
                auto* destellin = getNode(ge, "destellin");
                auto* destellinText = dynamic_cast<Texture2D*>(destellin->getEntity())->texture;


                int posX = static_cast<int>(ge.getWorldToScreenX(r.position) - static_cast<float>(destellinText->getWidth()) * wRate / 2);
                int posY = static_cast<int>(ge.getWorldToScreenY(r.position) - static_cast<float>(destellinText->getHeight()) * hRate / 2);

                ge.drawNode(destellin, { posX, posY });
            }
            else
            {
                auto* fijado = getNode(ge, "fijado");
                auto* fijadoInfo = dynamic_cast<Gif*>(fijado->getEntity());
                auto& frames = fijadoInfo->frames;
                auto& current = frames[fijadoInfo->currentFrame];
                int posX = static_cast<int>(ge.getWorldToScreenX(r.position) - static_cast<float>(current->getWidth()) * wRate * 1.1f / 2);
                int posY = static_cast<int>(ge.getWorldToScreenY(r.position) - static_cast<float>(current->getHeight()) * hRate * 1.1f / 2);

                ge.drawNode(fijado, { posX, posY }, { 1.1f, 1.1f });
            }
        }
    }
}

// void RenderSystem::drawDeath(GameEngine& engine)
// {
//     engine.drawRectangle(0, 0, engine.getScreenWidth(), engine.getScreenHeight(), Fade(BLACK, 0.5f));

//     // Valores de la caja de texto
//     float boxWidth = 300.f;
//     float boxWidth2 = 500.f;
//     float boxHeight = 100.f;
//     float posX = static_cast<float>(engine.getScreenWidth() / 2) - (boxWidth / 2.f);
//     float posX2 = static_cast<float>(engine.getScreenWidth() / 2) - (boxWidth2 / 2.f);
//     float posY = static_cast<float>(engine.getScreenHeight() / 2) - (boxHeight / 2.f);

//     // Tamaño de la fuente
//     GuiSetStyle(DEFAULT, TEXT_SIZE, 40);

//     // Color de la fuente de texto
//     GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFF0000ff);

//     GuiLabelButton(Rectangle{ posX, posY, boxWidth, boxHeight }, "HAS MUERTO");

//     std::string text = "[ENTER] para volver a jugar";
//     if (engine.isGamepadAvailable(0))
//         text = "Pulsa [X] para volver a jugar";
//     GuiLabelButton(Rectangle{ posX2, posY + 50, boxWidth2, boxHeight }, text.c_str());
// }

void RenderSystem::unloadModels(EntityManager& em, GameEngine&)
{
    using SYSCMPs = MP::TypeList<RenderComponent>;
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity&, RenderComponent& ren)
    {
        // engine.unloadMesh(ren.mesh);
        // engine.unloadModel(ren.model);
        ren.meshLoaded = false;
    });
}

// ---------- //
// Update HUD //
// ---------- //

void RenderSystem::updateHealthBar(GameEngine& engine, EntityManager& em, const Entity& e)
{
    auto const& l{ em.getComponent<LifeComponent>(e) };
    auto& plfi = em.getSingleton<PlayerInfo>();

    auto wRate = engine.getWidthRate();
    auto hRate = engine.getHeightRate();

    // Datos de la barra de vida
    int barWidth = static_cast<int>(60.6f * wRate);
    int barX = static_cast<int>(234.84f * wRate);
    int barY = static_cast<int>(45.45f * hRate);
    int spacing = static_cast<int>(15.15f * wRate);
    int posFaceX = static_cast<int>(30.3f * wRate);
    int posFaceY = static_cast<int>(30.3f * hRate);

    // ------------------- //
    // Node: Cara del mago //
    // ------------------- //
    std::string face = "";

    // Mago Happy
    if (l.life > l.maxLife / 2)
        face = "mago_happy";
    // Mago Meh
    else if (l.life > 2)
        face = "mago_meh";
    // Mago sos
    else
        face = "mago_sos";
    engine.drawNode(getNode(engine, face.c_str()), { posFaceX, posFaceY }, { 1.2f, 1.2f });

    // -------------------- //
    // Node: Puntos de vida //
    // -------------------- //

    auto* copyNode = getNode(engine, "Copy");

    int i{};

    // Corazon
    for (; i < l.life / 2; ++i)
    {
        auto* heart = engine.createNode(getNode(engine, "heart"), copyNode);
        engine.drawNode(heart, { barX + i * (barWidth + spacing), barY });
    }

    // Corazon medio
    if (l.life & 1)
    {
        auto* half = engine.createNode(getNode(engine, "half_heart"), copyNode);
        engine.drawNode(half, { barX + i * (barWidth + spacing), barY });
        ++i;
    }

    // Corazon vacio
    for (; i < l.maxLife / 2; ++i)
    {
        auto* empty = engine.createNode(getNode(engine, "empty_heart"), copyNode);
        engine.drawNode(empty, { barX + i * (barWidth + spacing), barY });
    }

    // Si la vida máxima es impar, dibujamos un corazón vacío
    if ((l.maxLife & 1) && l.life < l.maxLife)
    {
        auto* empty = engine.createNode(getNode(engine, "empty_heart"), copyNode);
        engine.drawNode(empty, { barX + i * (barWidth + spacing), barY });
        ++i;
    }

    // Armadura
    if (plfi.armor > 0)
    {
        auto& armor = plfi.armor;
        int armorLife = l.maxLife + plfi.armor;
        int maxArmorLife = l.maxLife + plfi.max_armor;

        // Dibujamos el corazón de armadura lleno
        for (; i < armorLife / 2; ++i)
        {
            auto* ice_heart = engine.createNode(getNode(engine, "ice_heart"), copyNode);
            engine.drawNode(ice_heart, { barX + i * (barWidth + spacing), barY });
        }

        // Si la vida es impar, dibujamos un medio corazón
        if (armor & 1)
        {
            auto* ice_half = engine.createNode(getNode(engine, "half_ice_heart"), copyNode);
            engine.drawNode(ice_half, { barX + i * (barWidth + spacing), barY });
            ++i;
        }

        for (; i < maxArmorLife / 2; ++i)
        {
            // Dibujamos el corazón vacío
            auto* ice_empty = engine.createNode(getNode(engine, "empty_ice_heart"), copyNode);
            engine.drawNode(ice_empty, { barX + i * (barWidth + spacing), barY });
        }

        // Si la vida máxima es impar, dibujamos un corazón vacío
        if ((maxArmorLife & 1) && armor < plfi.max_armor)
        {
            auto* ice_empty = engine.createNode(getNode(engine, "empty_ice_heart"), copyNode);
            engine.drawNode(ice_empty, { barX + i * (barWidth + spacing), barY });
        }
    }
}

void RenderSystem::updateManaBar(GameEngine& engine, EntityManager& em)
{
    auto& plfi{ em.getSingleton<PlayerInfo>() };

    if (plfi.mana > plfi.max_mana)
        plfi.mana = plfi.max_mana - 2;

    // Datos para la barra para el maná
    auto wRate = engine.getWidthRate();
    auto hRate = engine.getHeightRate();
    int barX = static_cast<int>(220.84f * wRate);
    int barY = static_cast<int>(110.78f * hRate);
    int barWidth = 236;

    float manaWidth = static_cast<float>(barWidth * plfi.mana / plfi.max_mana);

    // Interpolación
    if (plfi.mana != plfi.max_mana)
        manaWidth = plfi.mana_width + (static_cast<float>(manaWidth) - static_cast<float>(plfi.mana_width)) * 0.175f;

    // ------------------- //
    // Node: Barra de mana //
    // ------------------- //

    auto* rect = getNode(engine, "mana_rect");
    int offSetX = static_cast<int>(18 * wRate);
    int offSetY = static_cast<int>(12 * hRate);
    engine.drawNode(rect, { barX + offSetX, barY + offSetY }, { manaWidth, 0.9f });

    auto* border = getNode(engine, "borde_mana");
    engine.drawNode(border, { barX, barY });

    plfi.mana_width = manaWidth;
}

// -------- //
// Draw HUD //
// -------- //

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

    float div = elapsed_CoinBar / elapsed_limit_CoinBar;

    // Posición de la barra de destellos
    auto wRate = engine.getWidthRate();
    auto hRate = engine.getHeightRate();

    auto sum = static_cast<float>(digits.size()) * 16.5f; // 16.5 es la mitad del ancho de la textura de la moneda
    float offSetX = 180.f * wRate + sum;
    float screenWidth = static_cast<float>(engine.getScreenWidth());

    // Interpolación
    coinBarX = static_cast<int>(1.f - div * screenWidth + div * screenWidth - offSetX);

    // Barra para los destellos
    int posX = coinBarX;
    int posY = static_cast<int>(screenWidth - 195 * hRate);

    auto* numsCopy = getNode(engine, "Copy");
    auto* destellos = getNode(engine, "destellos");

    engine.drawNode(destellos, { posX, posY });

    // Interpolación de la posición de los números
    int offSetCoinNum = static_cast<int>(60.f * wRate + sum);
    coinNumberX = static_cast<int>((1.f - div) * (screenWidth + (offSetX - static_cast<float>(offSetCoinNum))) + div * screenWidth - static_cast<float>(offSetCoinNum));
    posY = static_cast<int>(screenWidth - 175.5f * hRate);
    auto coinNumberX2 = coinNumberX;
    std::string plusMinus = "+";

    if (plfi.minusCoins)
        plusMinus = "-";

    // Dibujamos el número de destellos
    if (elapsed_CoinBar > 0 && plfi.coins > 0)
    {
        // Dibujamos el símbolo de más o menos
        auto* symbol = getNode(engine, plusMinus.c_str());
        int symbolX = coinNumberX - static_cast<int>(30.5f * wRate);
        int symbolY = posY - static_cast<int>(63.f * hRate);
        engine.drawNode(symbol, { symbolX, symbolY });

        // Dibujamos los destellos actuales
        for (std::size_t i = digits.size(); i-- > 0; )
        {
            // Numero
            int posX = coinNumberX;
            auto* numero = engine.createNode(getNode(engine, std::to_string(digits[i]).c_str()), numsCopy);

            engine.drawNode(numero, { posX, posY });

            coinNumberX += static_cast<int>(30 * wRate);
        }

        // Dibujamos los destellos ganados
        for (std::size_t i = digits2.size(); i-- > 0; )
        {
            // Numero
            int posX = coinNumberX2;
            int offSetY = static_cast<int>(67.5f * hRate);
            auto* numero = engine.createNode(getNode(engine, std::to_string(digits2[i]).c_str()), numsCopy);

            engine.drawNode(numero, { posX, posY - offSetY });

            coinNumberX2 += static_cast<int>(30 * wRate);
        }
    }
    else if (plfi.minusCoins)
        plfi.minusCoins = false;
}

void RenderSystem::handleAnimatedTexture(const std::string& name, const std::string& textureName, int x, int y, float scaleFactor)
{
    if (animatedTextures.find(name) == animatedTextures.end())
    {
        animatedTextures[name] = { textureName, x, y, scaleFactor };
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
        // Dibujamos la cantidad de mana restante del player en el HUD
        updateManaBar(engine, em);

        auto wRate = engine.getWidthRate();
        auto hRate = engine.getHeightRate();
        float screenWidth = static_cast<float>(engine.getScreenWidth());

        std::map<std::size_t, std::pair<float, float>> spellPositions = {
            {0, {screenWidth - 420 * wRate, 30 * hRate}},
            {1, {screenWidth - 315 * wRate, 187.5f * hRate}},
            {2, {screenWidth - 165 * wRate, 247.5f * hRate}}
        };

        static std::map<Spells, std::tuple<std::string, std::string, float, float, float>> spellToTexture = {
            {Spells::WaterBomb, {"pompas", "exp_pompa", 10.0f, 15.5f, 2.5f}},
            {Spells::WaterDash, {"dash", "exp_dash", 7.5f, 7.5f, 2.5f}},
            {Spells::FireBall, {"bola_fuego", "exp_bola_f", 22.5f, 25.5f, 2.55f}},
            {Spells::FireMeteorites, {"meteoritos", "exp_pompa", 22.5f, 25.5f, 2.5f}},
            {Spells::IceShards, {"estacas", "exp_pompa", 22.5f, 25.5f, 2.5f}},
            {Spells::IceShield, {"escudo", "exp_pompa", 22.5f, 25.5f, 2.5f}},
        };

        for (std::size_t i = 0; i < plfi.spellSlots.size(); i++)
        {
            auto& spell = plfi.spellSlots[i];
            if (spell.spell != plfi.noSpell)
            {
                if (!plfi.showBook)
                {
                    std::string spellName = "hechizo" + std::to_string(i + 1);
                    handleAnimatedTexture(std::to_string(i + 1) + "_pl", "placeholder", static_cast<int>(spellPositions[i].first), static_cast<int>(spellPositions[i].second), 2.45f);

                    // Usar el mapa para obtener el nombre de la textura, posiciones y factor de escala
                    auto textureDetails = spellToTexture[spell.spell];
                    handleAnimatedTexture(spellName, std::get<0>(textureDetails), static_cast<int>(spellPositions[i].first + std::get<2>(textureDetails) * wRate), static_cast<int>(spellPositions[i].second + std::get<3>(textureDetails) * hRate), std::get<4>(textureDetails));
                }
                else
                {
                    if (getNode(engine, "libro")->isVisible())
                        break;
                    // Usar el mapa para obtener el nombre de la textura de explosión
                    auto textureDetails = spellToTexture[plfi.spells.back().spell];
                    drawSpellExplanation(engine, std::get<1>(textureDetails));

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

void RenderSystem::drawSpellExplanation(GameEngine& engine, std::string name)
{
    // Dibujamos textura del libro
    auto wRate = engine.getWidthRate();
    auto hRate = engine.getHeightRate();
    auto* libro = getNode(engine, "libro");
    auto* libroText = dynamic_cast<Texture2D*>(libro->getEntity())->texture;
    // Calculamos la posición inicial fuera de la pantalla
    int initialPosY = -libroText->getHeight();
    int posX = engine.getScreenWidth() / 2 - static_cast<int>(static_cast<float>(libroText->getWidth()) * wRate / 2);

    // Calculamos la posición final en el centro de la pantalla
    int finalPosY = engine.getScreenHeight() / 2 - static_cast<int>(static_cast<float>(libroText->getHeight()) * hRate / 2);

    // Animamos la posición Y
    int posY = initialPosY + static_cast<int>(static_cast<float>(finalPosY - initialPosY) * elapsed_book);

    // Dibujamos el libro
    engine.drawNode(libro, { posX, posY });

    // Dibujamos el gif de la explicación por encima
    auto* gif = getNode(engine, name.c_str());
    auto* gifInfo = dynamic_cast<Gif*>(gif->getEntity());
    auto& gifFrames = gifInfo->frames;
    auto& gifCurrent = gifFrames[gifInfo->currentFrame];

    // Calculamos la posición inicial y final para el gif
    initialPosY = -gifCurrent->getHeight();
    finalPosY = engine.getScreenHeight() / 2 - static_cast<int>(static_cast<float>(gifCurrent->getHeight()) * hRate / 2);

    // Animamos la posición Y del gif
    posY = initialPosY + static_cast<int>(static_cast<float>(finalPosY - initialPosY) * elapsed_book);

    posX = engine.getScreenWidth() / 2 - static_cast<int>(static_cast<float>(gifCurrent->getWidth()) * wRate / 2);

    engine.drawNode(gif, { posX, posY });

    // Incremento
    elapsed_book += timeStep240;

    if (elapsed_book > 1.0f)
        elapsed_book = 1.0f;
}

void RenderSystem::drawStaff(GameEngine& engine, EntityManager& em)
{
    auto& plfi{ em.getSingleton<PlayerInfo>() };

    if (plfi.hasStaff)
    {
        auto wRate = engine.getWidthRate();
        auto hRate = engine.getHeightRate();
        float screenWidth = static_cast<float>(engine.getScreenWidth());

        // Posiciones
        int posPLX = static_cast<int>(screenWidth - 165.f * wRate);
        int posPLY = static_cast<int>(67.5f * hRate);
        int posPalX = static_cast<int>(screenWidth - 142.5f * wRate);
        int posPalY = static_cast<int>(90.f * hRate);

        handleAnimatedTexture("4_pl", "placeholder", posPLX, posPLY, 2.7f);
        handleAnimatedTexture("palo", "palo", posPalX, posPalY, 2.8f);
    }
}

void RenderSystem::drawAnimatedTextures(GameEngine& engine)
{
    auto wRate = engine.getWidthRate();
    auto hRate = engine.getHeightRate();

    for (auto& [_, textureInfo] : animatedTextures)
    {
        // Calculamos los factores de escala
        textureInfo.scaleFactorX = 3.5f - textureInfo.scaleChange * textureInfo.lerpFactor;
        textureInfo.scaleFactorY = 3.5f - textureInfo.scaleChange * textureInfo.lerpFactor;
        Node* texture = nullptr;
        if (textureInfo.textureName == "placeholder")
            texture = engine.createNode(getNode(engine, textureInfo.textureName.c_str()), getNode(engine, "Copy"));
        else
            texture = getNode(engine, textureInfo.textureName.c_str());

        // Información de la textura
        auto* textureData = dynamic_cast<Texture2D*>(texture->getEntity());
        float width = static_cast<float>(textureData->texture->getWidth()) * wRate;
        float height = static_cast<float>(textureData->texture->getHeight()) * hRate;

        // Calcula la posición del centro de la pantalla
        int centerX = engine.getScreenWidth() / 2 - static_cast<int>(width * textureInfo.scaleFactorX / 2);
        int centerY = engine.getScreenHeight() / 2 - static_cast<int>(200.f * hRate);

        // Interpola entre el centro de la pantalla y la posición objetivo
        int posX = centerX + static_cast<int>(textureInfo.lerpFactor * static_cast<float>(textureInfo.targetPosX - centerX));
        int posY = centerY + static_cast<int>(textureInfo.lerpFactor * static_cast<float>(textureInfo.targetPosY - centerY));

        // Dibujamos la textura
        engine.drawNode(texture, { posX, posY }, { textureInfo.scaleFactorX, textureInfo.scaleFactorY });

        // Si el tiempo transcurrido es menor que 1.5 segundos, no hagas nada
        if (textureInfo.elapsed < 2.5f)
        {
            // Incrementamos el tiempo transcurrido
            textureInfo.elapsed += timeStep * 2;
        }
        else
        {
            // Incrementamos el factor de interpolación
            textureInfo.lerpFactor += textureInfo.lerpSpeed;
            if (textureInfo.lerpFactor > 1.0f)
                textureInfo.lerpFactor = 1.0f;

            if (textureInfo.textureName == "placeholder")
            {
                drawSmallButtons(engine, _, posX, posY, static_cast<int>(width), static_cast<int>(height));
            }
        }
    }
}

void RenderSystem::drawSmallButtons(GameEngine& engine, const std::string& name, int posX, int posY, int width, int height)
{
    std::string texture = "";
    posX += static_cast<int>(static_cast<float>(width) / 1.6f);
    posY += static_cast<int>(static_cast<float>(height) / 1.4f);

    if (name == "1_pl")
    {
        if (engine.isGamepadAvailable(0))
            texture = "boton_cuadrado";
        else
            texture = "tecla_j";
    }
    else if (name == "2_pl")
    {
        if (engine.isGamepadAvailable(0))
            texture = "boton_circulo";
        else
            texture = "tecla_k";
    }
    else if (name == "3_pl")
    {
        if (engine.isGamepadAvailable(0))
            texture = "boton_triangulo";
        else
            texture = "tecla_l";
    }
    else if (name == "4_pl")
    {
        if (engine.isGamepadAvailable(0))
        {
            texture = "boton_r2";
            posX -= static_cast<int>(static_cast<float>(width) / 2.5f);
            posY -= static_cast<int>(static_cast<float>(height) / 6.5f);
        }
        else
        {
            texture = "tecla_espacio";
            posX -= static_cast<int>(static_cast<float>(width) / 1.7f);
            posY -= static_cast<int>(static_cast<float>(height) / 7.f);
        }
    }

    auto* button = getNode(engine, texture.c_str());
    engine.drawNode(button, { posX, posY });
}

void RenderSystem::drawTextBox(GameEngine& engine, EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& txti = em.getSingleton<TextInfo>();
    auto& textQueue = txti.getTextQueue();

    // Sacamos el texto en formatos para GuiTextBox
    auto& str = textQueue.front();
    auto text = const_cast<char*>(str.second.c_str());

    std::map<SpeakerType, std::string> speakerTextures = {
        {SpeakerType::PLAYER, "mago_happy"},
        {SpeakerType::PLAYER_SAD, "mago_meh"},
        {SpeakerType::PLAYER_DANGER, "mago_sos"},
        {SpeakerType::CAT, "investigador"},
        {SpeakerType::NOMAD, "nomada"},
        {SpeakerType::INVESTIGATOR, "investigador"}
    };

    auto* box = getNode(engine, "cuadroDialogo");
    auto& boxInfo = *dynamic_cast<TextBox*>(box->getEntity());
    auto& boxRect = boxInfo.box.size;
    auto& boxWidth = boxRect.x;
    auto& boxHeight = boxRect.y;

    auto wRate = engine.getWidthRate();
    auto hRate = engine.getHeightRate();

    // Centramos la posición del cuadro de texto
    int posX = engine.getScreenWidth() / 2 - static_cast<int>(boxWidth * wRate / 2);
    int posY = static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 1.25f) - static_cast<int>(boxHeight * hRate / 2);

    boxInfo.text.setText(text);
    engine.drawNode(box, { posX, posY });

    int offSetX = 40;
    int offSetY = 50;
    if (speakerTextures.count(str.first) > 0) {
        auto* speaker = engine.createNode(getNode(engine, speakerTextures[str.first].c_str()), getNode(engine, "Copy"));
        engine.drawNode(speaker, { posX - offSetX, posY - offSetY });
    }

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

    int posButtonX = posX + static_cast<int>(boxWidth * wRate);
    int posButtonY = posY + static_cast<int>(boxHeight * hRate);

    bool isLast = true;
    if (textQueue.size() > 1)
    {
        isLast = false;
        auto* arrowNode = getNode(engine, "sig");
        auto& flecha = *dynamic_cast<Texture2D*>(arrowNode->getEntity())->texture;

        posButtonX -= static_cast<int>(static_cast<float>(flecha.getWidth()) * wRate * 0.5f);
        posButtonY -= static_cast<int>(static_cast<float>(flecha.getHeight()) * 1.2f * hRate * 0.5f);

        engine.drawNode(arrowNode, { posButtonX, posButtonY }, { 0.5f, 0.5f });
    }

    std::string textGif = "x";
    int rest{};
    if (!engine.isGamepadAvailable(0))
    {
        textGif = "e";
        rest = static_cast<int>(7.57f * hRate);
    }

    auto* gif = getNode(engine, textGif.c_str());
    auto& textEntity = *dynamic_cast<Gif*>(gif->getEntity());
    auto& frames = textEntity.frames;
    posButtonX -= static_cast<int>(static_cast<float>(frames[textEntity.currentFrame]->getWidth()) * wRate * 0.4f);
    if (!isLast)
        posButtonY -= rest;
    else
    {
        posButtonX -= static_cast<int>(6 * wRate);
        posButtonY -= static_cast<int>(static_cast<float>(frames[textEntity.currentFrame]->getHeight()) * hRate * 0.5f);
    }

    engine.drawNode(gif, { posButtonX, posButtonY }, { 0.4f, 0.4f });
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
    if (!nodeExists(engine, "fpstext"))
    {
        auto* fpsText = engine.createText({ engine.getScreenWidth() - 100, 10 }, fpsStr.c_str(), engine.getDefaultFont(), 20, D_RED, "fpstext", getNode(engine, "HUD"));
        fpsText->setVisibleOne(true);
    }
    else
    {
        auto* fpsNode = getNode(engine, "fpstext");
        dynamic_cast<Text*>(fpsNode->getEntity())->setText(fpsStr);
        fpsNode->setVisibleOne(true);
    }
}

void RenderSystem::drawBoatParts(GameEngine& ge, EntityManager& em)
{
    auto& plfi = em.getSingleton<PlayerInfo>();
    auto wRate = ge.getWidthRate();
    auto hRate = ge.getHeightRate();

    if (elapsed_boat < elapsed_limit_boat)
    {
        elapsed_boat += timeStep;
        if (elapsed_boat > elapsed_limit_boat) elapsed_boat = elapsed_limit_boat;
    }

    // Dibujamos la textura de la barra que sale desde la derecha
    auto* barca = getNode(ge, "barco");
    auto* barcaText = dynamic_cast<Texture2D*>(barca->getEntity())->texture;

    // Calculamos la posición inicial y final de la barra
    int initialPosX = 0 - barcaText->getWidth();
    int finalPosX = 0;

    // Interpolamos la posición X
    int posX = initialPosX - static_cast<int>(static_cast<float>(initialPosX - finalPosX) * elapsed_boat);
    int posY = ge.getScreenHeight() / 4;

    // Dibujamos la barra
    ge.drawNode(barca, { posX, posY });

    auto* copyNode = getNode(ge, "BoatCopy");
    copyNode->clearChildren();

    auto* textureNum = ge.createNode(getNode(ge, std::to_string(plfi.boatParts.size()).c_str()), copyNode);
    auto* texture4 = ge.createNode(getNode(ge, "4"), copyNode);
    auto* textureBar = ge.createNode(getNode(ge, "barra"), copyNode);

    auto* numText = dynamic_cast<Texture2D*>(textureNum->getEntity())->texture;
    auto* barText = dynamic_cast<Texture2D*>(textureBar->getEntity())->texture;
    int offSetX1 = static_cast<int>(157.5f * wRate);
    int offSetX2 = offSetX1 - static_cast<int>(19.5f * wRate);
    int offSetX3 = offSetX1 - static_cast<int>(15.f * wRate);
    int textWidth = static_cast<int>(static_cast<float>(numText->getWidth()) * wRate);
    int barWidth = static_cast<int>(static_cast<float>(barText->getWidth()) * wRate);
    int offSetY1 = static_cast<int>(3 * hRate);
    int offSetY2 = static_cast<int>(30 * hRate);

    // Dibujamos el num / 4
    ge.drawNode(textureNum, { posX + offSetX1, posY + offSetY1 });

    // Dibujamos la barra
    ge.drawNode(textureBar, { posX + offSetX2 + textWidth / 2, posY + offSetY1 });

    // Dibujamos el num / 4
    ge.drawNode(texture4, { posX + offSetX3 + textWidth / 2 + barWidth / 2, posY + offSetY2 });
}

Node* RenderSystem::getNode(GameEngine& engine, const char* name)
{
    return engine.nodes.at(name);
}

bool RenderSystem::nodeExists(GameEngine& engine, const char* name)
{
    return engine.nodes.find(name) != engine.nodes.end();
}