#include "render_system.hpp"
#include "../motor/src/darkmoon.hpp"
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
    drawParticles(em, engine);

    // Terminamos el frame
    endFrame(engine, em);
}

void RenderSystem::restartScene(GameEngine& engine)
{
    updateAnimatedTextures(engine);
    auto* dosde = getNode(engine, "2D");
    dosde->setVisible(false);
    dosde->setVisibleOne(true);
}

void RenderSystem::changeScene(GameEngine& engine)
{
    // engine.node_scene2D->clearChildren();
    // engine.node_scene3D->clearChildren();
}

void RenderSystem::drawLogoGame(GameEngine& engine, EntityManager& em, SoundSystem& ss) {
    ss.ambient_stop();
    ss.music_stop();
    ss.ambient_started = false;

    engine.beginDrawing();
    engine.clearBackground(D_WHITE);

    restartScene(engine);

    // Logo del videojuego

    float aux_width = 1808.0f;
    float aux_height = 1027.0f;
    auto logoTwoWonders = getNode(engine, "logo_twowonders");

    float reScaleX = 0.7f / (aux_width / static_cast<float>(engine.getScreenWidth()));
    float reScaleY = 0.5f / (aux_height / static_cast<float>(engine.getScreenHeight()));
    logoTwoWonders->setScale({ reScaleX, reScaleY, 0.0f });

    float posX = static_cast<float>(engine.getScreenWidth()) / 2 - (aux_width * reScaleX) / 2;
    float posY = static_cast<float>(engine.getScreenHeight()) / 2 - (aux_height * reScaleY) / 1.5f;
    logoTwoWonders->setTranslation({ posX, posY, 0.0f });
    logoTwoWonders->setVisible(true);


    // Datos de los botones
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    posX = static_cast<float>(engine.getScreenWidth() / 2) - (buttonWidth / 2.f);
    posY = static_cast<float>(engine.getScreenHeight() / 1.30) - (buttonHeight / 2.f);

    // Botones
    if (!nodeExists(engine, "initScreen")) {
        auto* init = engine.createNode("initScreen", getNode(engine, "Menu"));

        engine.dmeg.CreateButton({}, { buttonWidth, buttonHeight }, "JUGAR",
            engine.dmeg.GetDefaultFont(), 15,
            D_BLACK,
            DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            D_AQUA_DARK, D_AQUA, D_AQUA_LIGHT,
            "Boton jugar", init);

        engine.dmeg.CreateButton({}, { buttonWidth, buttonHeight }, "CONFIGURACION",
            engine.dmeg.GetDefaultFont(), 15,
            D_BLACK,
            DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            D_AQUA_DARK, D_AQUA, D_AQUA_LIGHT,
            "Boton configuracion", init);

        engine.dmeg.CreateButton({}, { buttonWidth, buttonHeight }, "SALIR",
            engine.dmeg.GetDefaultFont(), 15,
            D_BLACK,
            DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            D_AQUA_DARK, D_AQUA, D_AQUA_LIGHT,
            "Boton salir", init);
    }

    auto* init = getNode(engine, "initScreen");
    init->setVisible(true);

    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();

    for (auto& bt : init->getChildren()) {
        auto but = dynamic_cast<DarkMoon::Button*>(bt->getEntity());
        // Recolocar botones
        if (bt->name == "Boton jugar")
            bt->setTranslation({ posX, posY, 0.0f });
        else if (bt->name == "Boton configuracion")
            bt->setTranslation({ posX, posY + 55, 0.0f });
        else if (bt->name == "Boton salir")
            bt->setTranslation({ posX, posY + 110, 0.0f });

        // Comprobar estado del boton
        if (but->state == DarkMoon::ButtonState::CLICK) {
            if (bt->name == "Boton jugar") {
                li.currentScreen = GameScreen::STORY;
                ss.seleccion_menu();
                ss.music_stop();
            }
            else if (bt->name == "Boton configuracion") {
                li.currentScreen = GameScreen::OPTIONS;
                li.previousScreen = GameScreen::TITLE;
                ss.seleccion_menu();
            }
            else if (bt->name == "Boton salir") {
                li.gameShouldEnd = true;
                ss.sonido_salir();
            }
        }
    }

    // [ TODO ] -> Funcionalidad Mando

    /*

    // Funcionalidad de botones
    Rectangle btn1Rec = { posX, posY, buttonWidth, buttonHeight };
    Rectangle btn2Rec = { posX, posY + 55, buttonWidth, buttonHeight };
    Rectangle btn3Rec = { posX, posY + 110, buttonWidth, buttonHeight };

    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();

    // Define the current button selection
    auto& currentButton = inpi.currentButton;
    bool buttonTouched = false;

    // Define the buttons
    ButtonRect buttons[] = {
        { btn1Rec, "JUGAR", 0 },
        { btn2Rec, "CONFIGURACION", 1 },
        { btn3Rec, "SALIR", 2 }
    };

    // Control de botones de mando para cambiar el botón seleccionado
    if (inpi.up || inpi.left) {
        currentButton = (currentButton > 0) ? currentButton - 1 : sizeof(buttons) / sizeof(ButtonRect) - 1;
        ss.sonido_mov();
    }
    if (inpi.down || inpi.right) {
        currentButton = (currentButton < sizeof(buttons) / sizeof(ButtonRect) - 1) ? currentButton + 1 : 0;
        ss.sonido_mov();
    }

    for (std::size_t i = 0; i < sizeof(buttons) / sizeof(ButtonRect); i++) {
        ButtonRect& button = buttons[i];
        bool isCurrent = (currentButton == i);
        if (GuiButton(button.rect, isCurrent ? ("[" + std::string(button.text) + "]").c_str() : button.text) ||
            (isCurrent && inpi.interact)) {
            currentButton = i;
            // Handle the button action
            switch (button.action) {
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
            case 2: // "SALIR"
                li.gameShouldEnd = true;
                ss.sonido_salir();
                break;
            }

            inpi.interact = false;
        }

        if (engine.checkCollisionPointRec(GetMousePosition(), button.rect) && !buttonTouched)
            buttonTouched = true;
    }

    if (buttonTouched && !ss.pushed)
    {
        ss.sonido_mov();
        ss.pushed = true;
    }
    else if (!buttonTouched && ss.pushed)
        ss.pushed = false;
    */

    // Dibujar arbol
    engine.dmeg.GetRootNode()->traverse(glm::mat4());

    engine.endDrawing();
}

void RenderSystem::drawChargeScreen(GameEngine& engine, EntityManager& em)
{
    engine.beginDrawing();
    engine.clearBackground({ 171, 159, 197, 255 });
    auto* chargeGif = getNode(engine, "carga");

    // Redimensionamos
    auto& textEntity = *dynamic_cast<DarkMoon::AnimatedTexture2D*>(chargeGif->getEntity());
    auto& frames = textEntity.frames;
    auto& currentFrame = textEntity.currentFrame;
    chargeGif->setScale({ 2.0f, 2.0f, 1.0f });

    int posX = static_cast<int>(engine.getScreenWidth() / 2 - frames[currentFrame]->getWidth() / 2);
    int posY = static_cast<int>(engine.getScreenHeight() / 2 - frames[currentFrame]->getHeight() / 2);

    chargeGif->setTranslation({ posX, posY, 0.0f });
    chargeGif->setVisible(true);

    auto& li = em.getSingleton<LevelInfo>();
    li.loadingTime += timeStep;

    engine.endDrawing();
}

void RenderSystem::drawControls(EntityManager& em, GameEngine& engine)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();

    if ((engine.isKeyDown(KEY_E) && inpi.interact) || engine.isKeyDown(KEY_ESCAPE))
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
    auto cont = dynamic_cast<DarkMoon::Texture2D*>(controles->getEntity());

    float aux_width = cont->texture->getWidth();
    float aux_height = cont->texture->getHeight();

    float reScaleX = 0.7f / (aux_width / static_cast<float>(engine.getScreenWidth()));
    float reScaleY = 0.5f / (aux_height / static_cast<float>(engine.getScreenHeight()));
    controles->setScale({ reScaleX, reScaleY, 0.0f });

    int posX = static_cast<int>(static_cast<float>(engine.getScreenWidth()) / 2 - (aux_width * reScaleX) / 2);
    int posY = static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 2 - (aux_height * reScaleY) / 1.5f);
    controles->setTranslation({ posX, posY, 0.0f });

    auto txtBox = engine.dmeg.CreateTextBox({ engine.getScreenWidth() / 2 - 100, engine.getScreenHeight() - 100 },
        { 200, 50 }, D_WHITE, "DALE A [E] PARA SALIR", engine.dmeg.GetDefaultFont(),
        20, D_WHITE, DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
        "Texto controles", getNode(engine, "Menu"));

    auto entTxtBox = dynamic_cast<DarkMoon::TextBox*>(txtBox->getEntity());
    entTxtBox->drawBox = false;
    /*
    engine.drawRectangle(0, 0, engine.getScreenWidth(), engine.getScreenHeight(), Fade({ 113, 75, 146, 255 }, 0.5f));
    engine.textures[text].width = static_cast<int>(engine.getScreenWidth() / 1.3);
    engine.textures[text].height = static_cast<int>(engine.getScreenHeight() / 1.6);
    engine.drawTexture(engine.textures[text],
        engine.getScreenWidth() / 2 - engine.textures[text].width / 2,
        engine.getScreenHeight() / 2 - engine.textures[text].height / 2,
        WHITE);
    */
    engine.dmeg.GetRootNode()->traverse(glm::mat4());

    engine.endDrawing();
}

void RenderSystem::drawOptions(GameEngine& engine, EntityManager& em, SoundSystem& ss) {
    engine.beginDrawing();
    engine.clearBackground(D_WHITE);

    restartScene(engine);

    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float middleScreen = static_cast<float>(engine.getScreenWidth() / 2);

    // Posición del botón de volver
    float posX = middleScreen - (buttonWidth / 2);
    float posY = static_cast<float>(engine.getScreenHeight() / 1.1) - (buttonHeight / .5f);

    // Botones de resolución
    float posResX = static_cast<float>(middleScreen) - (buttonWidth / 2);
    float posResY = static_cast<float>(200) - (buttonHeight / 2);
    float offSetY = 150;

    // Botones
    if (!nodeExists(engine, "opciones")) {
        engine.createNode("Pantalla de opciones", getNode(engine, "Menu"));

        engine.dmeg.CreateButton({}, { buttonWidth, buttonHeight }, "VOLVER",
            engine.dmeg.GetDefaultFont(), 15, D_BLACK,
            DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            D_AQUA_DARK, D_AQUA, D_AQUA_LIGHT,
            "Boton volver", getNode(engine, "opciones"));

        engine.dmeg.CreateButton({}, { buttonWidth, buttonHeight }, "800x600",
            engine.dmeg.GetDefaultFont(), 15, D_BLACK,
            DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            D_AQUA_DARK, D_AQUA, D_AQUA_LIGHT,
            "Boton 800x600", getNode(engine, "opciones"));

        engine.dmeg.CreateButton({}, { buttonWidth, buttonHeight }, "1280x720",
            engine.dmeg.GetDefaultFont(), 15, D_BLACK,
            DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            D_AQUA_DARK, D_AQUA, D_AQUA_LIGHT,
            "Boton 1280x720", getNode(engine, "opciones"));

        engine.dmeg.CreateButton({}, { buttonWidth, buttonHeight }, "1920x1080",
            engine.dmeg.GetDefaultFont(), 15, D_BLACK,
            DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            D_AQUA_DARK, D_AQUA, D_AQUA_LIGHT,
            "Boton 1920x1080", getNode(engine, "opciones"));

        engine.dmeg.CreateButton({}, { buttonWidth, buttonHeight }, "FULLSCREEN",
            engine.dmeg.GetDefaultFont(), 15, D_BLACK,
            DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            D_AQUA_DARK, D_AQUA, D_AQUA_LIGHT,
            "Boton fullscreen", getNode(engine, "opciones"));

        engine.dmeg.CreateButton({}, { buttonWidth + 50, buttonHeight }, "CONTROLES MANDO",
            engine.dmeg.GetDefaultFont(), 15, D_BLACK,
            DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            D_AQUA_DARK, D_AQUA, D_AQUA_LIGHT,
            "Boton controles mando", getNode(engine, "opciones"));

        engine.dmeg.CreateButton({}, { buttonWidth + 50, buttonHeight }, "CONTROLES TECLADO",
            engine.dmeg.GetDefaultFont(), 15, D_BLACK,
            DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            D_AQUA_DARK, D_AQUA, D_AQUA_LIGHT,
            "Boton controles teclado", getNode(engine, "opciones"));

        engine.dmeg.CreateSlider({}, { buttonWidth, 20 }, D_WHITE, D_AQUA, "Slider volumen", getNode(engine, "opciones"));
    }
    getNode(engine, "opciones")->setVisible(true);

    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();

    for (auto& bt : getNode(engine, "opciones")->getChildren()) {
        // Recolocar botones
        if (bt->name == "Boton volver")
            bt->setTranslation({ posX, posY, 0.0f });
        else if (bt->name == "Boton 800x600")
            bt->setTranslation({ posResX - offSetY, posResY, 0.0f });
        else if (bt->name == "Boton 1280x720")
            bt->setTranslation({ posResX + offSetY, posResY, 0.0f });
        else if (bt->name == "Boton 1920x1080")
            bt->setTranslation({ posResX - offSetY, posResY + offSetY / 2.5f, 0.0f });
        else if (bt->name == "Boton fullscreen")
            bt->setTranslation({ posResX + offSetY, posResY + offSetY / 2.5f, 0.0f });
        else if (bt->name == "Boton controles mando")
            bt->setTranslation({ posX - offSetY - 40, posY - 100, 0.0f });
        else if (bt->name == "Boton controles teclado")
            bt->setTranslation({ posX + offSetY, posY - 100, 0.0f });
        else if (bt->name == "Slider volumen")
            bt->setTranslation({ middleScreen - buttonWidth, 100.0f, 0.0f });

        // Estado de los botones
        auto but = dynamic_cast<DarkMoon::Button*>(bt->getEntity());
        auto sli = dynamic_cast<DarkMoon::Slider*>(bt->getEntity());

        if (but != nullptr && but->state == DarkMoon::ButtonState::CLICK) {
            if (bt->name == "Boton volver")
                li.currentScreen = li.previousScreen;
            else if (bt->name == "Boton 800x600")
                engine.setWindowSize(800, 600);
            else if (bt->name == "Boton 1280x720")
                engine.setWindowSize(1280, 720);
            else if (bt->name == "Boton 1920x1080")
                engine.setWindowSize(1920, 1080);
            else if (bt->name == "Boton fullscreen") {
                engine.setWindowFullScreen();
                fullScreen = !fullScreen;
            }
            else if (bt->name == "Boton controles mando") {
                li.keyboardControls = false;
                li.evenMorePreviousScreen = li.previousScreen;
                li.currentScreen = GameScreen::CONTROLS;
                li.previousScreen = GameScreen::OPTIONS;
            }
            else if (bt->name == "Boton controles teclado") {
                li.keyboardControls = true;
                li.evenMorePreviousScreen = li.previousScreen;
                li.currentScreen = GameScreen::CONTROLS;
                li.previousScreen = GameScreen::OPTIONS;
            }

            ss.seleccion_menu();
        }
        else if (sli != nullptr)
            if (bt->name == "Slider volumen")
                ss.setVolumeMaster(sli->valor);
    }

    // [ TODO ] -> MANDO

    /*

    // Slider del volumen
    Rectangle volumenSlider = { middleScreen - buttonWidth, 100, buttonWidth, 20 };
    auto& sosy = em.getSingleton<SoundSystem>();
    float volumen = sosy.getVolumeMaster() * 100;
    float* vol = &volumen;

    // { volumenSlider, "Volumen", 5 },

    // Define the current button selection
    auto& currentButton = inpi.currentButton;
    bool buttonTouched = false;

    bool inpiCheck1 = inpi.up || inpi.left;
    bool inpiCheck2 = inpi.down || inpi.right;

    // Define the buttons
    ButtonRect buttons[] = {
        { btn1Rec, "VOLVER", 0 },
        { btn2Rec, "800x600", 1 },
        { btn3Rec, "1280x720", 2 },
        { btn4Rec, "1920x1080", 3 },
        { btn5Rec, "FULLSCREEN", 4 },
        { volumenSlider, "Volumen", 5 },
        { btn6Rec, "CONTROLES MANDO", 6},
        { btn7Rec, "CONTROLES TECLADO", 7 }
    };

    std::string volName = "Volumen";
    for (std::size_t i = 0; i < sizeof(buttons) / sizeof(ButtonRect); i++) {
        ButtonRect& button = buttons[i];
        bool isCurrent = (currentButton == i);
        if (GuiButton(button.rect, isCurrent ? ("[" + std::string(button.text) + "]").c_str() : button.text) ||
            (isCurrent && inpi.interact)) {
            currentButton = i;
            // Handle the button action
            switch (button.action) {
            case 5: // "Volumen"
                break;
            case 6: // "CONTROLES"
                li.keyboardControls = false;
                li.evenMorePreviousScreen = li.previousScreen;
                li.currentScreen = GameScreen::CONTROLS;
                li.previousScreen = GameScreen::OPTIONS;
                break;
            case 7: // "CONTROLES"
                li.keyboardControls = true;
                li.evenMorePreviousScreen = li.previousScreen;
                li.currentScreen = GameScreen::CONTROLS;
                li.previousScreen = GameScreen::OPTIONS;
                break;
            }

            inpi.interact = false;
        }
        if (i == 5 && currentButton == i)
        {
            if (engine.isGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || engine.isKeyDown(KEY_RIGHT)) {
                *vol = (*vol < 100) ? *vol + 1 : 100;
                // ss.sonido_mov();
            }
            if (engine.isGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || engine.isKeyDown(KEY_LEFT)) {
                *vol = (*vol > 0) ? *vol - 1 : 0;
                // ss.sonido_mov();
            }

            inpiCheck1 = inpi.up;
            inpiCheck2 = inpi.down;
            volName = "[Volumen]";
        }

        if (engine.checkCollisionPointRec(GetMousePosition(), button.rect) && !buttonTouched)
            buttonTouched = true;
    }
    GuiSliderBar(volumenSlider, NULL, volName.c_str(), vol, 0, 100);
    ss.setVolumeMaster(*vol / 100.0f);

    // Control de botones de mando para cambiar el botón seleccionado
    if (inpiCheck1) {
        currentButton = (currentButton > 0) ? currentButton - 1 : sizeof(buttons) / sizeof(ButtonRect) - 1;
        ss.sonido_mov();
    }
    else if (inpiCheck2) {
        currentButton = (currentButton < sizeof(buttons) / sizeof(ButtonRect) - 1) ? currentButton + 1 : 0;
        ss.sonido_mov();
    }

    if (buttonTouched && !ss.pushed)
    {
        ss.sonido_mov();
        ss.pushed = true;
    }
    else if (!buttonTouched && ss.pushed)
        ss.pushed = false;

    */

    engine.dmeg.GetRootNode()->traverse(glm::mat4());

    engine.endDrawing();
}

void RenderSystem::drawPauseMenu(GameEngine& engine, EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    if (li.currentScreen != GameScreen::CONTROLS)
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

        auto& inpi = em.getSingleton<InputInfo>();

        // Define the current button selection
        auto& currentButton = inpi.currentButton;
        bool buttonTouched = false;

        ButtonRect buttons[] = {
        { { posX, posY, buttonWidth, buttonHeight }, "CONTINUAR", 0 },
        { { posX, posY + 70, buttonWidth, buttonHeight }, "OPCIONES", 1 },
        { { posX, posY + 140, buttonWidth, buttonHeight }, "VOLVER AL INICIO", 2 },
        { { posX, posY + 210, buttonWidth, buttonHeight }, "SALIR", 3 },
        };

        for (std::size_t i = 0; i < sizeof(buttons) / sizeof(ButtonRect); i++) {
            ButtonRect& button = buttons[i];
            bool isCurrent = (currentButton == i);
            if (GuiButton(button.rect, isCurrent ? ("[" + std::string(button.text) + "]").c_str() : button.text) ||
                (isCurrent && inpi.interact)) {
                currentButton = i;
                inpi.mouseClick = true;
                // Aquí puedes manejar la acción del botón
                switch (button.action) {
                case 0: // "CONTINUAR"
                {
                    inpi.pause = false;
                    ss.seleccion_menu();
                    ss.playAmbient();
                    ss.play_music();
                    break;
                }
                case 1: // "OPCIONES"
                {
                    inpi.currentButton = 0;
                    li.currentScreen = GameScreen::OPTIONS;
                    li.previousScreen = GameScreen::GAMEPLAY;
                    ss.seleccion_menu();
                    break;
                }
                case 2: // "VOLVER AL INICIO"
                {
                    li.currentScreen = GameScreen::TITLE;
                    li.resetGame = true;
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

                inpi.interact = false;
            }

            if (engine.checkCollisionPointRec(GetMousePosition(), button.rect) && !buttonTouched)
                buttonTouched = true;
        }

        if (buttonTouched && !ss.pushed)
        {
            ss.sonido_mov();
            ss.pushed = true;
        }
        else if (!buttonTouched && ss.pushed)
            ss.pushed = false;

        // Control de botones de mando para cambiar el botón seleccionado
        if (inpi.up || inpi.left) {
            currentButton = (currentButton > 0) ? currentButton - 1 : sizeof(buttons) / sizeof(ButtonRect) - 1;
            ss.sonido_mov();
        }
        if (inpi.down || inpi.right) {
            currentButton = (currentButton < sizeof(buttons) / sizeof(ButtonRect) - 1) ? currentButton + 1 : 0;
            ss.sonido_mov();
        }
    }
}

void RenderSystem::drawInventory(GameEngine& engine, EntityManager& em)
{
    float windowWidth = 450.0f;
    float windowHeight = 450.0f;
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float posButtonX = static_cast<float>(engine.getScreenWidth() / 2) - (buttonWidth / 2);
    float posButtonY = static_cast<float>(engine.getScreenHeight() / 3) - (buttonHeight / 2);
    float posX = static_cast<float>(engine.getScreenWidth() / 2) - (windowWidth / 2);
    float posY = static_cast<float>(engine.getScreenHeight() / 2) - (windowHeight / 2);
    // float augment = 55.f;

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

    // Definimos los botones
    auto size = plfi.inventory.size() + 1;
    std::vector<ButtonRect> buttons(size);
    for (std::size_t i = 0; i < plfi.inventory.size(); i++) {
        buttons[i] = { { posButtonX, posButtonY + 55 * static_cast<float>(i), buttonWidth, buttonHeight }, plfi.inventory[i]->name.c_str(), static_cast<int>(i) };
    }
    buttons[plfi.inventory.size()] = { { posButtonX, posButtonY + 55 * static_cast<float>(plfi.inventory.size()), buttonWidth, buttonHeight }, "VOLVER", static_cast<int>(plfi.inventory.size()) };

    if (plfi.selectedItem == plfi.max)
    {
        // Control de botones de mando para cambiar el botón seleccionado
        if (inpi.up || inpi.left) {
            currentButton = (currentButton > 0) ? currentButton - 1 : plfi.inventory.size();
        }
        else if (inpi.down || inpi.right) {
            currentButton = (currentButton < plfi.inventory.size()) ? currentButton + 1 : 0;
        }

        for (std::size_t i = 0; i < buttons.size(); i++) {
            ButtonRect& button = buttons[i];
            bool isCurrent = (currentButton == i);
            if (GuiButton(button.rect, isCurrent ? ("[" + std::string(button.text) + "]").c_str() : button.text) ||
                (isCurrent && inpi.interact)) {
                currentButton = i;
                // Aquí puedes manejar la acción del botón
                if (i < plfi.inventory.size()) {
                    // Select an item from the inventory
                    plfi.selectedItem = plfi.inventory[i]->getID();
                    currentButton = 0;
                    return;
                }
                else {
                    // "VOLVER" button
                    auto& inpi = em.getSingleton<InputInfo>();
                    inpi.inventory = false;
                }

                inpi.interact = false;
            }
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

        if (inpi.interact) {
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
        else if (inpi.up || inpi.left) {
            // Move the selection up
            currentButton = (currentButton > 0) ? currentButton - 1 : 1;
        }
        else if (inpi.down || inpi.right) {
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
    engine.clearBackground({ 136, 219, 152, 255 });

    restartScene(engine);

    // DrawLogoKaiwa

    float aux_width = 11722.0f;
    float aux_height = 4703.0f;
    auto logoKaiwa = getNode(engine, "logo_kaiwa");

    float reScaleX = 1.0f / (aux_width / static_cast<float>(engine.getScreenWidth()));
    float reScaleY = 0.4f / (aux_height / static_cast<float>(engine.getScreenHeight()));
    logoKaiwa->setScale({ reScaleX, reScaleY, 1.0f });

    int posY = static_cast<int>(static_cast<float>(engine.getScreenHeight()) / 2 - (aux_height * reScaleY) / 2);
    logoKaiwa->setTranslation({ 0.0f, posY, 0.0f });
    logoKaiwa->setVisible(true);

    // Dibujar arbol
    engine.dmeg.GetRootNode()->traverse(glm::mat4());

    engine.endDrawing();
}

void RenderSystem::drawEnding(GameEngine& engine) {
    engine.beginDrawing();
    engine.clearBackground(D_WHITE);

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
    engine.clearBackground(D_WHITE);

    restartScene(engine);

    float boxWidth = 700.f;
    float boxHeight = 400.f;
    float posX = static_cast<float>(engine.getScreenWidth() / 2) - (boxWidth / 2);
    float posY = static_cast<float>(engine.getScreenHeight() / 2.5) - (boxHeight / 2);

    if (!nodeExists(engine, "historia")) {
        auto* hist = engine.createNode("historia", getNode(engine, "Dialog"));

        engine.dmeg.CreateTextBox({ posX, posY }, { boxWidth, boxHeight }, D_WHITE,
            "¡Bienvenido a la aventura!", engine.dmeg.GetDefaultFont(), 40,
            D_BLACK, DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            "Texto 1", hist);
        engine.dmeg.CreateTextBox({ posX, posY + 50 }, { boxWidth, boxHeight }, D_WHITE,
            "Estas perdido por el bosque y", engine.dmeg.GetDefaultFont(), 40,
            D_BLACK, DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            "Texto 2", hist);
        engine.dmeg.CreateTextBox({ posX, posY + 100 }, { boxWidth, boxHeight }, D_WHITE,
            "debes encontrar a tu maestro.", engine.dmeg.GetDefaultFont(), 40,
            D_BLACK, DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            "Texto 3", hist);
        engine.dmeg.CreateTextBox({ posX, posY + 150 }, { boxWidth, boxHeight }, D_WHITE,
            "¡Mucha suerte!", engine.dmeg.GetDefaultFont(), 40,
            D_BLACK, DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            "Texto 4", hist);

        engine.dmeg.CreateTextBox({ posX, posY + 250 }, { boxWidth, boxHeight }, D_WHITE,
            "", engine.dmeg.GetDefaultFont(), 40,
            D_BLACK, DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER,
            "Texto 5", hist);

        for (auto& txtEl : hist->getChildren())
            dynamic_cast<DarkMoon::TextBox*>(txtEl->getEntity())->drawBox = false;
    }

    auto* hist = getNode(engine, "historia");
    hist->setVisible(true);

    auto auxText = dynamic_cast<DarkMoon::TextBox*>(hist->getChildren().back()->getEntity());

    auxText->text.text = "DALE A [E] PARA JUGAR";
    if (engine.isGamepadAvailable(0))
        auxText->text.text = "DALE A [X] PARA JUGAR";

    engine.dmeg.GetRootNode()->traverse(glm::mat4());

    init();
    engine.endDrawing();
}

void RenderSystem::drawEntities(EntityManager& em, GameEngine& engine)
{
    using SYSCMPs = MP::TypeList<RenderComponent>;
    auto& frti = em.getSingleton<FrustumInfo>();

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, RenderComponent& r)
    {
        if (!e.hasTags(FrustOut{}) && r.meshLoaded && e.hasComponent<ColliderComponent>())
        {
            auto& col = em.getComponent<ColliderComponent>(e);
            if (frti.bboxIn(col.bbox) == FrustPos::OUTSIDE)
                return;
        }

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
                    pos.setY(pos.y() - 2.4);
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
                    pos.setY(pos.y() - 3.0);
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
                    pos.setY(pos.y() - 5.7);
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
                    pos.setY(pos.y() - 8.6);
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
                else if (e.hasTag<DoorTag>() || e.hasTag<LeverTag>() || e.hasTag<FireBallTag>()
                    || e.hasTag<CoinTag>() || e.hasTag<WaterBombTag>() || e.hasTag<BoatTag>())
                {
                    in = true;
                }

                if (r.rotationVec == vec3d::zero())
                    r.rotationVec = { 0.0, -1.0, 0.0 };

                float orientationInDegrees = static_cast<float>(r.orientation * (180.0f / K_PI));

                if (r.node) {
                    r.node->setTranslation({ pos.x(), pos.y(), pos.z() });
                    r.node->setScale({ scl.x(), scl.y(), scl.z() });

                    r.node->setRotation({ r.rotationVec.x(), r.rotationVec.y(), r.rotationVec.z() }, orientationInDegrees);

                    /*
                    if (!in)
                    {
                        int orientationInDegreesInt = static_cast<int>(orientationInDegrees);
                        if (orientationInDegreesInt % 90 == 0 && std::abs(orientationInDegreesInt) != 270 && std::abs(orientationInDegreesInt) != 90)
                        {
                            // engine.drawCubeWires(r.position, static_cast<float>(r.scale.x()), static_cast<float>(r.scale.y()), static_cast<float>(r.scale.z()), BLACK);
                            //std::cout << "Dibujando cubo\n";
                        }
                        else
                        {
                            // engine.drawModelWires(r.model, pos, r.rotationVec, orientationInDegrees, scl, BLACK);
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
        r.node = engine.dmeg.CreateCube({ 0.0f, 0.0f, 0.0f },
            { static_cast<float>(r.scale.x()), static_cast<float>(r.scale.y()), static_cast<float>(r.scale.z()) },
            D_GRAY, "cubo 3D", getNode(engine, "3D"));
    }
    r.meshLoaded = true;
}


void RenderSystem::drawParticles(EntityManager&, GameEngine&)
{
    //     using partCMPs = MP::TypeList<ColliderComponent, ParticleMakerComponent>;
    //     using noTAGs = MP::TypeList<>;

    //     auto& frti = em.getSingleton<FrustumInfo>();
    //     em.forEach<partCMPs, noTAGs>([&](Entity&, ColliderComponent& col, ParticleMakerComponent& pmc)
    //     {
    //         if (frti.bboxIn(col.bbox) == FrustPos::OUTSIDE)
    //             return;

    //         if (pmc.active)
    //         {
    //             for (auto& p : pmc.particles)
    //             {
    //                 if (p.type == Particle::ParticleType::Pixel)
    //                 {
    //                     // Dibujamos 4 partćulas arriba, abajo, izquierda y derecha de la posición
    //                     engine.drawPoint3D(p.position.to_other<double>(), { p.r, p.g, p.b, p.a });
    //                     engine.drawPoint3D((p.position + vec3f{ 0.0f, 0.1f, 0.0f }).to_other<double>(), { p.r, p.g, p.b, p.a });
    //                     engine.drawPoint3D((p.position + vec3f{ 0.1f, 0.0f, 0.0f }).to_other<double>(), { p.r, p.g, p.b, p.a });
    //                     engine.drawPoint3D((p.position + vec3f{ 0.0f, -0.1f, 0.0f }).to_other<double>(), { p.r, p.g, p.b, p.a });
    //                     engine.drawPoint3D((p.position + vec3f{ -0.1f, 0.0f, 0.0f }).to_other<double>(), { p.r, p.g, p.b, p.a });
    //                 }
    //             }
    //         }
    //     });
}

// Empieza el dibujado y se limpia la pantalla
void RenderSystem::beginFrame(GameEngine& engine, EntityManager& em)
{
    restartScene(engine);
    engine.beginDrawing();

    auto& li = em.getSingleton<LevelInfo>();

    DarkMoon::Color bgColor = D_WHITE;
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
    engine.endMode3D();

    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();
    auto& txti = em.getSingleton<TextInfo>();

    if (li.isCharging())
        return;

    drawHUD(em, engine);

    if (inpi.debugPhy)
        drawDebugPhysics(engine, em, li);

    drawAlerts_IA(em, engine);

    if (txti.hasText())
        drawTextBox(engine, em);

    if (inpi.pause)
        drawPauseMenu(engine, em);
    else if (inpi.pause)
        inpi.pause = false;

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

    engine.dmeg.GetRootNode()->traverse(glm::mat4());
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

uint16_t findNearestNode(EntityManager& em, const vec3d& position, const std::map<uint16_t, vec3d>& nodes) {
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
        std::cout << startnode << "\n";
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
        if (col.bbox.intersectsRay(ray.origin, ray.direction) && !(col.behaviorType & BehaviorType::STATIC || col.behaviorType & BehaviorType::ZONE)) {
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
    // Parametros ventana editor in-game
    float windowRectX = 0.0f;
    float windowRectY = 100.0f;

    float windowRectWidth = 390.0f;
    float windowRectHeight = 550.0f;

    if (!nodeExists(engine, "editorIA"))
    {
        auto* editAI = engine.createNode("Editor IA", getNode(engine, "Menu"));
        engine.dmeg.CreateTextBox({ windowRectX, windowRectY }, { windowRectWidth, windowRectHeight }, { 255, 255, 255, 128 }, "Editor IA", engine.dmeg.GetDefaultFont(), 20, D_BLUE_DARK, DarkMoon::Aligned::TOP, DarkMoon::Aligned::LEFT, "Editor IA", editAI);
        engine.dmeg.CreateLine({ windowRectX, windowRectY + 40 }, { windowRectWidth, windowRectY + 40 }, D_GRAY, "Linea horizontal", editAI);
        engine.dmeg.CreateText({ windowRectX + 20, windowRectY + 50 }, "PARAMETROS", engine.dmeg.GetDefaultFont(), 20, D_RED, "text_parametros", editAI);
    }

    getNode(engine, "editorIA")->setVisible(true);

    auto& debugsglt = em.getSingleton<Debug_t>();
    using SYSCMPss = MP::TypeList<AIComponent, PhysicsComponent, ColliderComponent, RenderComponent>;
    using SYSTAGss = MP::TypeList<EnemyTag>;

    // [TODO] - RAYCAST
    RayCast ray = engine.getMouseRay();
    std::cout << "Origen: " << ray.origin.x() << " | " << ray.origin.y() << " | " << ray.origin.z() << "\n";
    std::cout << "Direccion: " << ray.direction.x() << " | " << ray.direction.y() << " | " << ray.direction.z() << "\n";

    em.forEach<SYSCMPss, SYSTAGss>([&](Entity& e, AIComponent& aic, PhysicsComponent& phy, ColliderComponent& col, RenderComponent& ren)
    {
        // Comprobar si el rayo intersecta con el collider
        if (col.bbox.intersectsRay(ray.origin, ray.direction) && !(col.behaviorType & BehaviorType::STATIC || col.behaviorType & BehaviorType::ZONE)) {
            if (engine.dmeg.IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isSelected = !isSelected;
                debugsglt.IA_id = e.getID();
            }
            //     // si es seleccionada => wires morados
            //     // no es seleccionada => wires rojos
            /*
            engine.beginMode3D();
            engine.drawCubeWires(ren.position, static_cast<float>(ren.scale.x()), static_cast<float>(ren.scale.y()), static_cast<float>(ren.scale.z()), RED);
            engine.endMode3D();
            */
            //std::cout << "hola\n";
        }
        if (isSelected && e.getID() == debugsglt.IA_id) {
            std::cout << "ID: " << std::to_string(debugsglt.IA_id).c_str() << "\n";
            /*
            engine.beginMode3D();
            engine.drawCubeWires(ren.position, static_cast<float>(ren.scale.x()), static_cast<float>(ren.scale.y()), static_cast<float>(ren.scale.z()), PURPLE);
            engine.endMode3D();
            // si se seleccionada una entidad se muestra el Editor de parámetros
            if (isSelected) {
                // ID DE LA ENTIDAD SELECCIONADA
                engine.drawText("EID:", 15, 170, 20, BLACK);
                engine.drawText(std::to_string(debugsglt.IA_id).c_str(), 55, 170, 20, DARKGRAY);
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
            */
        }
    });
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

            //vec2d center = { barX, barY - 120.0f };
            int centerx = static_cast<int>(barX - 25.0f);
            int centery = static_cast<int>(barY - 150.0f);
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

                auto* ear = getNode(engine, icon);

                ear->setScale({ 1.f / 1.3f, 1.f / 1.3f, 1.0f });
                ear->setTranslation({ static_cast<float>(centerx), static_cast<float>(centery), 0.0f });
                ear->setVisibleOne(true);

                //engine.drawCircleSector(center, 30.0f, 0.0f, aic.endangle, 30, RED);
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
void RenderSystem::drawHUD(EntityManager& em, GameEngine& engine)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& inpi = em.getSingleton<InputInfo>();

    getNode(engine, "HUD")->setVisibleOne(true);

    if (li.isDead)
    {
        em.getComponent<RenderComponent>(*em.getEntityByID(li.playerID)).visible = false;
        //drawDeath(engine); TODO
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

        if (e.hasTag<CrusherTag>()) {
            auto& phy = em.getComponent<PhysicsComponent>(e);
            auto& ai = em.getComponent<AIComponent>(e);
            int posx = static_cast<int>(engine.getWorldToScreenX(phy.position) + 30);
            int posz = static_cast<int>(engine.getWorldToScreenY(phy.position) - 70);
            // if (ai.playerdetected) {
            engine.drawRectangle(posx, posz, 10, 100, BLACK);
            int barHeight = static_cast<int>((ai.elapsed_shoot / ai.countdown_shoot) * 100);
            engine.drawRectangle(posx, posz, 10, barHeight, BLUE);
            // }
        }

        // Vidas HUD
        if (e.hasTag<EnemyTag>() && e.hasComponent<LifeComponent>() && em.getComponent<RenderComponent>(e).visible &&
            !(e.hasTag<AngryBushTag>() || e.hasTag<AngryBushTag2>()))
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            auto& l{ em.getComponent<LifeComponent>(e) };

            // Datos para la barra para la barra de vida
            int barWidth = 40;
            int barHeight = 4;
            int barX = static_cast<int>(engine.getWorldToScreenX(r.position) - 18);
            int barY = static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 10);

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
                DarkMoon::Node* animText;
                int sum, aux_width;

                if (engine.isGamepadAvailable(0)) {
                    animText = getNode(engine, "x");
                    aux_width = 134;
                }
                else
                {
                    animText = getNode(engine, "e");
                    sum = 13;
                    aux_width = 102;
                }

                int offSetX = aux_width / 4;
                if (e.hasTag<DoorTag>())
                    offSetX = -(aux_width / 4 + sum);

                int posX = static_cast<int>(engine.getWorldToScreenX(pos)) - offSetX;
                int posY = static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 11);

                animText->setTranslation({ posX, posY, 0.0f });
                animText->setScale({ 0.5f, 0.5f, 1.0f });

                animText->setVisible(true);

                if (e.hasTag<DoorTag>())
                {
                    // Candado abierto
                    // TODO
                    // {
                    //     int posX = static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(113 / 2));
                    //     int posY = static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 13);
                    //     auto candado_abierto = engine.dmeg.CreateTexture2D({ posX, posY },
                    //         "assets/HUD/candado_abierto.png",
                    //         D_WHITE,
                    //         "candado abierto",
                    //         engine.node_sceneTextures);
                    //     candado_abierto->setScale({ 0.7f, 0.7f, 1.0f });
                    // }
                    auto& lock = engine.textures["candado_abierto"];
                    engine.drawTexture(lock,
                        static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(lock.width / 2)),
                        static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 13),
                        { 255, 255, 255, 255 });
                }
            }
            else if (inter.showLock)
            {
                // Candado cerrado
                // TODO
                // {
                //     int posX = static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(113 / 2));
                //     int posY = static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 13);
                //     auto candado_cerrado = engine.dmeg.CreateTexture2D({ posX, posY },
                //         "assets/HUD/candado_cerrado.png",
                //         D_WHITE,
                //         "candado cerrado",
                //         engine.node_sceneTextures);
                //     candado_cerrado->setScale({ 0.7f, 0.7f, 1.0f });
                auto& lock = engine.textures["candado_cerrado"];
                engine.drawTexture(lock,
                    static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(lock.width / 2)),
                    static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 13),
                    { 255, 255, 255, 255 });

                if (e.hasTag<ChestTag>())
                {
                    auto swordText = engine.textures["batalla"];
                    swordText.width = static_cast<int>(swordText.width / 2);
                    swordText.height = static_cast<int>(swordText.height / 2);
                    engine.drawTexture(swordText,
                        static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(swordText.width / 2)),
                        static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 9.5),
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
                        engine.drawTexture(textureNum, posX + 3, posY, { 255, 255, 255, 255 });
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
                    auto& ch = em.getComponent<ChestComponent>(e);
                    if (phy.position.distance(li.enemyToChestPos) < 50.0 && ch.closeEnemies == 0)
                    {
                        if (elapsed_Lock < elapsed_limit_Lock)
                        {
                            // Candado abierto
                            // TODO
                            // {
                            //     int posX = static_cast<int>(engine.getWorldToScreenX(pos) - static_cast<float>(113 / 2));
                            //     int posY = static_cast<int>(engine.getWorldToScreenY(pos) - sclY * 9);
                            //     auto candado_abierto = engine.dmeg.CreateTexture2D({ posX, posY },
                            //         "assets/HUD/candado_abierto.png",
                            //         D_WHITE,
                            //         "candado abierto",
                            //         engine.node_sceneTextures);
                            //     candado_abierto->setScale({ 0.7f, 0.7f, 1.0f });
                            // }
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

        // if ((li.mapID == 0 || li.mapID == 1) && e.hasComponent<AttackComponent>())
        // {
        //     if (!li.tutorialEnemies.empty())
        //     {
        //         for (auto& enemy : li.tutorialEnemies)
        //         {
        //             auto& ene = *em.getEntityByID(enemy);
        //             if (ene.hasComponent<RenderComponent>())
        //             {
        //                 auto& ren{ em.getComponent<RenderComponent>(ene) };
        //                 auto& phy{ em.getComponent<PhysicsComponent>(ene) };
        //                 if (ren.visible && (ene.hasTag<DummyTag>() || ene.hasTag<DestructibleTag>()))
        //                 {
        //                     int aux_width = 134;
        //                     double multiplier = 55.0;

        //                     DarkMoon::Node* animText;

        //                     if (li.lockedEnemy == li.max)
        //                     {
        //                         if (engine.isGamepadAvailable(0))
        //                             animText = engine.nodes_sceneAnimatedTexture["circulo"];
        //                         else {
        //                             animText = engine.nodes_sceneAnimatedTexture["f"];
        //                             aux_width = 102;
        //                         }
        //                     }
        //                     else
        //                     {
        //                         if (engine.isGamepadAvailable(0))
        //                             animText = engine.nodes_sceneAnimatedTexture["cuadrado"];
        //                         else {
        //                             animText = engine.nodes_sceneAnimatedTexture["espacio"];
        //                             aux_width = 292;
        //                         }
        //                     }

        //                     if (ene.hasTag<DestructibleTag>())
        //                         multiplier = 8.0;
        //                     else if (li.mapID == 1)
        //                         multiplier = 25.0;

        //                     aux_width = aux_width / 2;

        //                     int posX = static_cast<int>(engine.getWorldToScreenX(phy.position)) - aux_width / 2;
        //                     int posY = static_cast<int>(engine.getWorldToScreenY(phy.position) - phy.scale.y() * multiplier);

        //                     animText->setTranslation({ posX, posY, 0.0f });
        //                     animText->setScale({ 0.5f, 0.5f, 1.0f });

        //                     animText->setVisible(true);
        //                 }
        //             }
        //         }
        //     }
        // }

        // if (li.num_zone == 1 && elapsed_WASD < elapsed_limit_WASD)
        // {
        //     auto& phy{ em.getComponent<PhysicsComponent>(e) };

        //     // Mostramos gif de joystick para moverse o texto WASD
        //     if (engine.isGamepadAvailable(0))
        //     {
        //         auto joystickL = engine.nodes_sceneAnimatedTexture["joystickL"];
        //         int posX = static_cast<int>(engine.getWorldToScreenX(phy.position) - 40);
        //         int posY = static_cast<int>(engine.getWorldToScreenY(phy.position) - phy.scale.y() * 37);

        //         joystickL->setTranslation({ posX, posY, 0.0f });
        //         joystickL->setScale({ 0.5f, 0.5f, 0.5f });

        //         joystickL->setVisible(true);
        //     }
        //     else
        //     {
        //         std::string text = "WASD para moverse";

        //         engine.drawText(text.c_str(),
        //             static_cast<int>(engine.getWorldToScreenX(phy.position) - 120),
        //             static_cast<int>(engine.getWorldToScreenY(phy.position) - phy.scale.y() * 25),
        //             20,
        //             WHITE);
        //     }

        //     elapsed_WASD += 1.0f / 60.0f;
        // }

        if ((li.mapID == 0 || li.mapID == 1) && pl.hasComponent<AttackComponent>())
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

                            char* gifName = "cuadrado";
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

                            auto* gif = getNode(engine, gifName);

                            // Redimensionamos la copia
                            gif->setScale({ 0.5f, 0.5f, 1.0f });

                            multiplier = 20.0;

                            if (ene.hasTag<DestructibleTag>())
                                multiplier = 8.0;
                            else if (li.mapID == 1)
                                multiplier = 25.0;

                            auto& textEntity = *dynamic_cast<DarkMoon::AnimatedTexture2D*>(gif->getEntity());
                            int posX = static_cast<int>(engine.getWorldToScreenX(phy.position)) - textEntity.frames[textEntity.currentFrame]->getWidth() / 2;
                            int posY = static_cast<int>(engine.getWorldToScreenY(phy.position) - phy.scale.y() * multiplier);

                            gif->setTranslation({ posX, posY, 0.0f });
                            gif->setVisible(true);
                        }
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

            joystickL->setTranslation({ posX, posY, 0.0f });
            joystickL->setScale({ 0.5f, 0.5f, 0.5f });

            joystickL->setVisible(true);

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
}

void RenderSystem::drawDebugPhysics(GameEngine& engine, EntityManager& em, LevelInfo& li)
{
    for (auto const& e : em.getEntities())
    {
        if (e.hasComponent<LifeComponent>() && em.getComponent<RenderComponent>(e).visible)
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

        if (e.hasComponent<ZoneComponent>())
        {
            auto& ren{ em.getComponent<RenderComponent>(e) };
            auto& z{ em.getComponent<ZoneComponent>(e) };

            engine.drawText(std::to_string(z.zone).c_str(),
                static_cast<int>(engine.getWorldToScreenX(ren.position) - 5),
                static_cast<int>(engine.getWorldToScreenY(ren.position) - ren.scale.y() * 5),
                20,
                RED);
        }

        if (e.hasComponent<RampComponent>() && e.hasComponent<PhysicsComponent>())
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

        if (e.hasComponent<PhysicsComponent>() && e.hasComponent<ColliderComponent>() && e.hasComponent<RenderComponent>())
        {
            auto& col{ em.getComponent<ColliderComponent>(e) };

            // Calcular la posición y el tamaño de la bounding box
            vec3d boxPosition = (col.bbox.min + col.bbox.max) / 2;
            vec3d boxSize = col.bbox.max - col.bbox.min;

            Color color = BLUE;
            if (col.behaviorType & BehaviorType::ZONE)
                color = GREEN;

            // Dibujar la bounding box
            //engine.beginMode3D();
            //engine.drawCubeWires(boxPosition,
            //    static_cast<float>(boxSize.x()),
            //    static_cast<float>(boxSize.y()),
            //    static_cast<float>(boxSize.z()),
            //    color);
            //engine.endMode3D();

            auto& phy = em.getComponent<PhysicsComponent>(e);

            RayCast ray = engine.getMouseRay();

            //std::cout << ray.origin << " " << ray.direction << std::endl;

            auto& ren = em.getComponent<RenderComponent>(e);
            bool notStatic = !(col.behaviorType & BehaviorType::ZONE);
            // Comprobar si el rayo intersecta con el collider

            if (col.bbox.intersectsRay(ray.origin, ray.direction) && notStatic && pointedEntity != li.playerID)
            {
                pointedEntity = e.getID();

                auto& col{ em.getComponent<ColliderComponent>(e) };

                // Calcular la posición y el tamaño de la bounding box
                vec3d boxPosition = (col.bbox.min + col.bbox.max) / 2;
                vec3d boxSize = col.bbox.max - col.bbox.min;

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

        // Dibujar el ID de las entidades // DEBUG
        if (e.hasComponent<RenderComponent>())
        {
            auto const& r{ em.getComponent<RenderComponent>(e) };
            engine.drawText(std::to_string(e.getID()).c_str(),
                static_cast<int>(engine.getWorldToScreenX(r.position) - 5),
                static_cast<int>(engine.getWorldToScreenY(r.position) - r.scale.y() * 50),
                20,
                BLACK);
        }
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
                int posX = static_cast<int>(ge.getWorldToScreenX(r.position) - 45 / 2);
                int posY = static_cast<int>(ge.getWorldToScreenY(r.position) - 45 / 2);
                auto* destellin = getNode(ge, "destellin");

                destellin->setScale({ 0.7f, 0.7f, 1.0f });
            }
            else
            {
                auto* fijado = getNode(ge, "fijado");
                int posX = static_cast<int>(ge.getWorldToScreenX(r.position) - 147);
                int posY = static_cast<int>(ge.getWorldToScreenY(r.position) - 147);

                fijado->setTranslation({ posX, posY, 0.0f });
                fijado->setVisible(true);
            }
        }
    }
}

void RenderSystem::initHUD(EntityManager&, GameEngine&)
{

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

    // Datos de la barra de vida
    int barWidth = 40;
    int barX = 155;
    int barY = 30;
    int spacing = 10;

    // ------------------- //
    // Node: Cara del mago //
    // ------------------- //

    auto node_cara_mago = getNode(engine, "Faces");

    // Mago Happy
    if (l.life > l.maxLife / 2)
        getNode(engine, "mago_happy1")->setVisible(true);
    // Mago Meh
    else if (l.life > 2)
        getNode(engine, "mago_meh1")->setVisible(true);
    // Mago sos
    else
        getNode(engine, "mago_sos1")->setVisible(true);

    // -------------------- //
    // Node: Puntos de vida //
    // -------------------- //

    auto* heartsCopyNode = getNode(engine, "textCopy");
    heartsCopyNode->clearChildren();

    int i{};

    // Corazon
    for (; i < l.life / 2; ++i)
    {
        auto* heart = engine.createNode(getNode(engine, "heart"), heartsCopyNode);
        heart->setTranslation({ (barX + i * (barWidth + spacing)), barY, 0.0f });
    }

    // Corazon medio
    if (l.life & 1)
    {
        auto* half = engine.createNode(getNode(engine, "half_heart"), heartsCopyNode);
        half->setTranslation({ (barX + i * (barWidth + spacing)), barY, 0.0f });
        ++i;
    }

    // Corazon vacio
    for (; i < l.maxLife / 2; ++i)
    {
        auto* empty = engine.createNode(getNode(engine, "empty_heart"), heartsCopyNode);
        empty->setTranslation({ (barX + i * (barWidth + spacing)), barY, 0.0f });
    }

    // Si la vida máxima es impar, dibujamos un corazón vacío
    if ((l.maxLife & 1) && l.life < l.maxLife)
    {
        auto* empty = engine.createNode(getNode(engine, "empty_heart"), heartsCopyNode);
        empty->setTranslation({ (barX + i * (barWidth + spacing)), barY, 0.0f });
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
            auto* ice_heart = engine.createNode(getNode(engine, "ice_heart"), heartsCopyNode);
            ice_heart->setTranslation({ (barX + i * (barWidth + spacing)), barY, 0.0f });
        }

        // Si la vida es impar, dibujamos un medio corazón
        if (armor & 1)
        {
            auto* ice_half = engine.createNode(getNode(engine, "half_ice_heart"), heartsCopyNode);
            ice_half->setTranslation({ (barX + i * (barWidth + spacing)), barY, 0.0f });
            ++i;
        }

        for (; i < maxArmorLife / 2; ++i)
        {
            // Dibujamos el corazón vacío
            auto* ice_empty = engine.createNode(getNode(engine, "empty_ice_heart"), heartsCopyNode);
            ice_empty->setTranslation({ (barX + i * (barWidth + spacing)), barY, 0.0f });
        }

        // Si la vida máxima es impar, dibujamos un corazón vacío
        if ((maxArmorLife & 1) && armor < plfi.max_armor)
        {
            auto* ice_empty = engine.createNode(getNode(engine, "empty_ice_heart"), heartsCopyNode);
            ice_empty->setTranslation({ (barX + i * (barWidth + spacing)), barY, 0.0f });
        }
    }

    heartsCopyNode->setVisible(true);
}

void RenderSystem::updateManaBar(GameEngine& engine, EntityManager& em)
{
    auto& plfi{ em.getSingleton<PlayerInfo>() };

    if (plfi.mana > plfi.max_mana)
        plfi.mana = plfi.max_mana - 2;

    // Datos para la barra para el maná
    int barWidth = static_cast<int>(plfi.max_mana * 1.8 * 0.7);
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

    // ------------------- //
    // Node: Barra de mana //
    // ------------------- //

    auto manaBar = getNode(engine, "ManaBar");

    auto* rect = getNode(engine, "mana_rect");
    rect->setTranslation({ barX + 14, barY + 9, 0.0f });
    rect->setScale({ static_cast<float>(manaWidth), 1.0f, 1.0f });
    getNode(engine, "borde_mana")->setTranslation({ barX, barY, 0.0f });

    plfi.mana_width = manaWidth;
}

void RenderSystem::updateAnimatedTextures(GameEngine& engine)
{
    // ----------------------- //
    // Node: Gifs              //
    // ----------------------- //

    auto* gifs = getNode(engine, "Gifs");
    for (DarkMoon::Node* child : gifs->getChildren())
        child->setVisible(false);
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
        elapsed_CoinBar += timeStep30 * multip;
        if (elapsed_CoinBar > elapsed_limit_CoinBar) elapsed_CoinBar = elapsed_limit_CoinBar;

        plfi.elapsed_coins += timeStep30;
    }
    else
    {
        elapsed_CoinBar -= timeStep30 * multip;
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
    int posX = coinBarX;
    int posY = engine.getScreenHeight() - 130;

    auto* numsCopy = getNode(engine, "textCopy");
    // numsCopy->clearChildren();
    auto* destellos = engine.createNode(getNode(engine, "destellos"), numsCopy);

    destellos->setTranslation({ posX, posY, 0.0f });
    destellos->setScale({ 0.7f, 0.7f, 1.0f });
    destellos->setVisible(true);

    // Interpolación de la posición de los números
    int offSetCoinNum = static_cast<int>(40 + sum);
    coinNumberX = static_cast<int>((1.f - div) * (static_cast<float>(engine.getScreenWidth() + (offSetX - offSetCoinNum))) + div * static_cast<float>(engine.getScreenWidth() - offSetCoinNum));
    posY = engine.getScreenHeight() - 117;
    auto coinNumberX2 = coinNumberX;
    char* plusMinus = "+";

    if (plfi.minusCoins)
        plusMinus = "-";

    // Dibujamos el número de destellos
    if (elapsed_CoinBar > 0 && plfi.coins > 0)
    {
        // Dibujamos el símbolo de más o menos
        auto* symbol = getNode(engine, plusMinus);
        symbol->setTranslation({ coinNumberX - 25, posY - 42, 0.0f });
        symbol->setVisible(true);

        // Dibujamos los destellos actuales
        for (std::size_t i = digits.size(); i-- > 0; )
        {
            // Numero

            int posX = coinNumberX;
            auto* numero = engine.createNode(getNode(engine, std::to_string(digits[i]).c_str()), numsCopy);

            numero->setScale({ 0.76f, 0.76f, 1.0f });
            numero->setTranslation({ posX, posY, 0.0f });

            coinNumberX += static_cast<int>(20);
        }

        // Dibujamos los destellos ganados
        for (std::size_t i = digits2.size(); i-- > 0; )
        {
            // Numero

            int posX = coinNumberX2;
            auto* numero = engine.createNode(getNode(engine, std::to_string(digits2[i]).c_str()), numsCopy);

            numero->setScale({ 0.76f, 0.76f, 1.0f });
            numero->setTranslation({ posX, posY - 45, 0.0f });

            coinNumberX2 += static_cast<int>(20);
        }
    }
    else if (plfi.minusCoins)
        plfi.minusCoins = false;

    numsCopy->setVisible(true);
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
        // Dibujamos la cantidad de mana restante del player en el HUD
        updateManaBar(engine, em);

        std::map<std::size_t, std::pair<int, int>> spellPositions = {
            {0, {engine.getScreenWidth() - 280, 20}},
            {1, {engine.getScreenWidth() - 210, 125}},
            {2, {engine.getScreenWidth() - 110, 165}}
        };

        static std::map<Spells, std::tuple<std::string, std::string, int, int, float>> spellToTexture = {
            {Spells::WaterBomb, {"pompas", "exp_pompa", 10, 15, 2.5f}},
            {Spells::WaterDash, {"dash", "exp_dash", 5, 5, 2.5f}},
            {Spells::FireBall, {"bola_fuego", "exp_bola_f", 15, 17, 2.55f}},
            {Spells::FireMeteorites, {"meteoritos", "exp_pompa", 15, 17, 2.5f}},
            {Spells::IceShards, {"estacas", "exp_pompa", 15, 17, 2.5f}},
            {Spells::IceShield, {"escudo", "exp_pompa", 15, 17, 2.5f}},
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
        handleAnimatedTexture("4_pl", "placeholder", engine.getScreenWidth() - 110, 45, engine.textures["placeholder"], 2.75f);
        handleAnimatedTexture("palo", "palo", engine.getScreenWidth() - 95, 60, engine.textures["palo"], 2.85f);
    }
}

void RenderSystem::drawAnimatedTextures(GameEngine& engine)
{
    for (auto& [_, textureInfo] : animatedTextures)
    {
        // Calculamos el factor de escala
        textureInfo.scaleFactor = 3.5f - textureInfo.scaleChange * textureInfo.lerpFactor;
        int& width = textureInfo.width;
        int& height = textureInfo.height;

        // Calcula la posición del centro de la pantalla
        int centerX = static_cast<int>(static_cast<float>(engine.getScreenWidth() / 2) - static_cast<float>(width) * textureInfo.scaleFactor / 2);
        int centerY = static_cast<int>(static_cast<float>(engine.getScreenHeight() / 2 - (50)) - static_cast<float>(width) * textureInfo.scaleFactor / 2);

        // Interpola entre el centro de la pantalla y la posición objetivo
        int posX = static_cast<int>(static_cast<float>(centerX) + textureInfo.lerpFactor * static_cast<float>(textureInfo.targetPosX - centerX));
        int posY = static_cast<int>(static_cast<float>(centerY) + textureInfo.lerpFactor * static_cast<float>(textureInfo.targetPosY - centerY));

        engine.drawTexture(engine.textures[textureInfo.textureName], posX, posY, { 255, 255, 255, 255 }, textureInfo.scaleFactor);

        // Si el tiempo transcurrido es menor que 1.5 segundos, no hagas nada
        if (textureInfo.elapsed < 2.5f)
        {
            // Incrementamos el tiempo transcurrido
            textureInfo.elapsed += timeStep30;
        }
        else
        {
            // Incrementamos el factor de interpolación
            textureInfo.lerpFactor += textureInfo.lerpSpeed;
            if (textureInfo.lerpFactor > 1.0f)
                textureInfo.lerpFactor = 1.0f;

            if (textureInfo.textureName == "placeholder")
            {
                drawSmallButtons(engine, _, posX, posY, width, height);
            }
        }
    }
}

void RenderSystem::drawSmallButtons(GameEngine& engine, const std::string& name, int posX, int posY, int width, int height)
{
    std::string texture = "";
    posX += static_cast<int>(width / 1.6);
    posY += static_cast<int>(height / 1.4);

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
            posX -= static_cast<int>(width / 2.5);
            posY -= static_cast<int>(height / 6.5);
        }
        else
        {
            texture = "tecla_espacio";
            posX -= static_cast<int>(width / 1.6);
            posY -= static_cast<int>(height / 6.5);
        }
    }

    engine.drawTexture(engine.textures[texture], posX, posY, { 255, 255, 255, 255 });
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
    auto text = const_cast<char*>(str.second.c_str());

    // Dibujamos el cuadro de diálogo con RayGui
    // engine.drawRectangle(static_cast<int>(posX), static_cast<int>(posY), static_cast<int>(boxWidth), static_cast<int>(boxHeight), WHITE);
    // GuiTextBox({ posX, posY, boxWidth, boxHeight }, text, static_cast<int>(str.second.size()), false);

    std::map<SpeakerType, std::string> speakerTextures = {
        {SpeakerType::PLAYER, "mago_happy"},
        {SpeakerType::PLAYER_SAD, "mago_meh"},
        {SpeakerType::PLAYER_DANGER, "mago_sos"},
        {SpeakerType::CAT, "investigador"},
        {SpeakerType::NOMAD, "nomada"},
        {SpeakerType::INVESTIGATOR, "investigador"}
    };

    auto* box = getNode(engine, "cuadroDialogo");

    dynamic_cast<DarkMoon::TextBox*>(box->getEntity())->text.text = text;
    box->setTranslation({ posX, posY, 0.0f });
    box->setVisible(true);

    float offSetX = 40.0;
    if (speakerTextures.count(str.first) > 0) {
        engine.drawTexture(engine.textures[speakerTextures[str.first]], static_cast<int>(posX - offSetX), static_cast<int>(posY - 50), { 255, 255, 255, 255 });
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

    int posButtonX = static_cast<int>(posX + boxWidth - 8);
    int posButtonY = static_cast<int>(posY + boxHeight - 8);
    if (textQueue.size() > 1)
    {
        auto* arrowNode = getNode(engine, "sig");
        auto& flecha = *dynamic_cast<DarkMoon::Texture2D*>(arrowNode->getEntity())->texture;

        arrowNode->setScale({ 0.3f, 0.3f, 1.0f });

        posButtonX -= flecha.getWidth() * 0.3;
        posButtonY -= flecha.getHeight() * 0.3;

        arrowNode->setTranslation({ posButtonX, posButtonY, 0.0f });

        posButtonY += flecha.getHeight() * 0.3;

        arrowNode->setVisible(true);
    }

    DarkMoon::Node* animText;
    int rest{};

    if (engine.isGamepadAvailable(0))
        animText = getNode(engine, "x");
    else
    {
        animText = getNode(engine, "e");
        rest = 5;
    }

    auto& textEntity = *dynamic_cast<DarkMoon::AnimatedTexture2D*>(animText->getEntity());
    posButtonX -= textEntity.frames[textEntity.currentFrame]->getWidth() * 0.3;
    posButtonY -= textEntity.frames[textEntity.currentFrame]->getHeight() * 0.3 - rest;

    animText->setTranslation({ posButtonX, posButtonY, 0.0f });
    animText->setScale({ 0.3f, 0.3f, 1.0f });

    animText->setVisible(true);
}

void RenderSystem::displayGif(GameEngine& engine, TextureType& copy, GameEngine::Gif& gif, int& posX, int& posY)
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
    int offSetX = 105;

    // Dibujamos el num / 4
    ge.drawTexture(textureNum, posX + offSetX, posY + 2, { 255, 255, 255, 255 });
    ge.drawTexture(textureBar, posX + (offSetX - 15) + textureNum.width / 2, posY + 2, { 255, 255, 255, 255 });
    ge.drawTexture(texture4, posX + (offSetX - 10) + textureNum.width / 2 + textureBar.width / 2, posY + 20, { 255, 255, 255, 255 });
}

DarkMoon::Node* RenderSystem::getNode(GameEngine& engine, const char* name)
{
    return engine.nodes.at(name);
}

bool RenderSystem::nodeExists(GameEngine& engine, const char* name)
{
    return engine.nodes.find(name) != engine.nodes.end();
}