#include "darkmoon.hpp"

DarkMoon::Node* createScene3D(DarkMoon::DarkMoonEngine& engine) {
    // Node Scene 3D
    auto p_node3D = engine.CreateNode("Scene 3D", engine.GetRootNode());

    //Node: Rejilla
    engine.CreateGrid(10, 10.0f, D_GRAY, "Rejilla principal", p_node3D);

    auto mapa = engine.CreateModel("assets/lvl_0-cnk_0.obj", D_WHITE, "Modelo: Main Character", engine.GetRootNode());
    mapa->translate({ 0.0f, 0.0f, 0.0f });
    mapa->setScale({ 1.f, 1.f, 1.f });
    mapa->setRotation({ 0.0f, 1.0f, 0.0f }, 225.0f);

    // Node: Linea diagonal
    engine.CreatePoint3D({ -1.0f, 0.0f, 1.0f }, 5.0f, D_BLACK, "Punto principio linea", p_node3D);
    engine.CreateLine3D({ -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, -1.0f }, 2.0f, D_YELLOW_DARK, "Linea amarilla", p_node3D);
    engine.CreatePoint3D({ 1.0f, 1.0f, -1.0f }, 5.0f, D_BLACK, "Punto fin linea", p_node3D);

    // Node: Plano
    engine.CreatePlane({ 0.0f, 0.0f, 0.0f }, { 15.0f, 15.0f }, D_PINK, "Plano rosita", p_node3D);

    //Node: Cubo
    engine.CreateCube({ -30.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f }, D_AQUA_DARK, "Cubo azulito", p_node3D);
    //engine.CreateCubeWires({ -30.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f }, D_BLACK, "Wireframe", p_node3D);
    //Node: Wireframe
    engine.CreateCubeWires({ 20.0f, 0.0f, -30.0f }, { 10.0f, 10.0f, 10.0f }, D_BLACK, "Wireframe", p_node3D);

    engine.CreatePointLight({30.0f, 10.0f, 30.0f}, D_YELLOW, "Luz prueba", p_node3D);
    engine.CreatePointLight({30.0f, 10.0f,  0.0f},    D_RED, "Luz prueba", p_node3D);
    engine.CreatePointLight({ 0.0f, 10.0f, 30.0f}, D_ORANGE, "Luz prueba", p_node3D);
    engine.CreatePointLight({ 0.0f, 10.0f,  0.0f},   D_BLUE, "Luz prueba", p_node3D);

    engine.CreatePointLight({}, D_YELLOW_LIGHT, "Luz prueba", p_node3D);

    // Node: Modelo Dummy 3
    // auto modelDummy3 = engine.CreateModel("assets/Cofre/Texturas/Cofre.fbx", D_WHITE, "Modelo: Dummy 3", p_node3D);
    // modelDummy3->translate({ 30.0f, 0.0f, 0.0f });
    // modelDummy3->rotate({ 1.0f, 0.0f, 0.0f }, -90.0f);
    // auto eModel = dynamic_cast<DarkMoon::Model*>(modelDummy3->getEntity());
    // auto& m_meshes = eModel->getMeshes();

    // auto& am = AnimationManager::getInstance();
    // auto* animation = am.createAnimation("assets/Cofre/Texturas/Cofre.fbx", eModel->getboneInfoMap());

    // auto id = am.PlayAnimation(animation);
    // for (auto& mesh : m_meshes) {
    //     mesh->animID = id;
    // }

    // auto modelCharacter = engine.CreateModel("assets/MainCharacter/Main_character.fbx", D_WHITE, "Modelo MAin character", p_node3D);
    // modelCharacter->translate({ 0.0f, 0.0f, 30.0f });
    // modelCharacter->rotate({ 1.0f, 0.0f, 0.0f }, -90.0f);
    // // modelCharacter->rotate({ 0.0f, 1.0f, 0.0f }, 90.0f);
    // auto eModel1 = dynamic_cast<DarkMoon::Model*>(modelCharacter->getEntity());
    // auto& m_meshes1 = eModel1->getMeshes();

    // auto* animation1 = am.createAnimation("assets/MainCharacter/Main_character.fbx", eModel1->getboneInfoMap());

    // auto id1 = am.PlayAnimation(animation1);
    // for (auto& mesh : m_meshes1) {
    //     mesh->animID = id1;
    // }

    //engine.CreateBillboard("assets/mago_happy.png", {0,10,0}, {20,20}, "Billboard", p_node3D);

    return p_node3D;
}

DarkMoon::Node* model = nullptr;

void createHUD(DarkMoon::DarkMoonEngine& engine){
    // Node HUD
    auto p_nodeHUD = engine.CreateNode("HUD", engine.GetRootNode());

    // PRUEBAS RENDIMIENTO //

    // // Node: Pixel prueba
    // engine.CreatePixel({ 5, 5 }, D_BLACK, "Pixel prueba", p_nodeHUD);
    // // Node: Linea prueba
    // engine.CreateLine({ 0, 0 }, { 100, 100 }, D_RED, "Linea prueba", p_nodeHUD);
    // // Node: Triangulo prueba
    // engine.CreateTriangle({ 0, 0 }, {100, 100}, {0, 100}, D_YELLOW_DARK, "Triangulo prueba", p_nodeHUD);
    // Node: Circulo prueba
    engine.CreateCircle({ 500, 500 }, 30, 20, D_AQUA_DARK, "Circulo prueba", p_nodeHUD);

    // Node: Rectangulo rosa Oscuro
    engine.CreateRectangle({ 12.0f, 12.0f }, { 375.0f, 40.0f }, D_PINK_DARK, "Rectangulo rosa oscuro", p_nodeHUD);
    // Node: Rectangulo rosa
    engine.CreateRectangle({ 10.0f, 10.0f }, { 375.0f, 40.0f }, D_PINK, "Rectangulo rosa", p_nodeHUD);

    // Node: Texto
    //engine.CreateText({10.0f, 10.0f}, "Ya era hora pato", engine.GetDefaultFont(), 24, D_GRAY,1,"Texto ya era hora", p_nodeHUD);

    // Node: Texture2D
    engine.CreateTexture2D({ 10.0f, 400.0f }, "assets/mago_happy.png", D_WHITE, "Textura Koromaru", p_nodeHUD);

    // Node: Rectangulo negro
    engine.CreateRectangle({ 10.0f, 100.0f }, { 150.0f, 150.0f }, D_BLACK, "Rectangulo negro", p_nodeHUD);

    // Node: AnimatedTexture2D
    std::vector<DarkMoon::Texture*> texturesGif;
    texturesGif.push_back(engine.LoadTexture2D("assets/joystickL-0.png"));
    texturesGif.push_back(engine.LoadTexture2D("assets/joystickL-1.png"));
    texturesGif.push_back(engine.LoadTexture2D("assets/joystickL-2.png"));
    texturesGif.push_back(engine.LoadTexture2D("assets/joystickL-3.png"));
    engine.CreateAnimatedTexture2D({ 10.0f, 100.0f }, texturesGif, D_WHITE, 1.0f, 0, "Textura Joystick Izquierdo", p_nodeHUD);

    // Node: TextBox
    //auto textBox = engine.CreateTextBox({ 660, 20 }, { 120, 60 }, D_AQUA_LIGHT, "Texto", engine.GetDefaultFont(), 24, D_BLACK, DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER, "Caja de texto de prueba", p_nodeHUD);

    // Node: Button
    engine.CreateButton({ 660, 100 }, { 120, 60 }, "Boton", engine.GetDefaultFont(), 24, D_BLACK, DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER, D_ORANGE_DARK, D_ORANGE, D_ORANGE_LIGHT, "Boton de prueba naranja", p_nodeHUD);

    // Node: Slider
    //engine.CreateSlider({660, 180}, {120, 30}, D_GRAY, D_AQUA_LIGHT, "Slider de prueba", p_nodeHUD);

    // // Node: Pixel Negro
    // engine.CreatePixel({400.0f, 300.0f}, BLACK, "Pixel negro", p_nodeHUD);
    // // Node: Linea Roja
    // engine.CreateLine({0.0f, 0.0f}, {700.0f, 600.0f}, RED, "Linea roja", p_nodeHUD);
    // // Node: Triangulo Verde
    // engine.CreateTriangle({10.0f, 54.0f}, {10.0f, 590.0f}, {210.0f, 590.0f}, KAIWA, "Triangulo verde", p_nodeHUD);
    // // Node: Circulo 
    // engine.CreateCircle({250.0f, 50.0f}, 30.0f, 20, LAVENDER, "Circulo lavanda", p_nodeHUD);
}

void createMainCharacter(DarkMoon::DarkMoonEngine& engine) {
    // Model
    model = engine.CreateModel("assets/Nomada.obj", D_WHITE, "Modelo: Main Character", engine.GetRootNode());
    model->translate({ 0.0f, 0.0f, 0.0f });
    model->setScale({ 2.f, 2.f, 2.f });
    model->setRotation({ 0.0f, 1.0f, 0.0f }, 225.0f);

    // Camera
    auto camera = engine.GetCamera();
    camera->position = { -60.f, 60.f, -60.f };
    camera->fovy = 60.0f;
}

void inputManager(DarkMoon::DarkMoonEngine& engine) {
    auto camera = engine.GetCamera();
    float velocity = 0.5f;

    if (engine.IsKeyDown(D_KEY_A) || engine.IsGamepadButtonDown(0, D_GAMEPAD_BUTTON_DPAD_LEFT)) { // +X -Z
        camera->position.x += velocity;
        camera->target.x += velocity;

        camera->position.z -= velocity;
        camera->target.z -= velocity;

        model->translate({ velocity, 0.0f, -velocity });
        model->setRotation({ 0.0f, 1.0f, 0.0f }, 135.0f);
        // model->setRotation({ 1.0f, 0.0f, 0.0f }, 45.0f);
    }
    if (engine.IsKeyDown(D_KEY_D) || engine.IsGamepadButtonDown(0, D_GAMEPAD_BUTTON_DPAD_RIGHT)) { // -X +Z
        camera->position.x -= velocity;
        camera->target.x -= velocity;

        camera->position.z += velocity;
        camera->target.z += velocity;

        model->translate({ -velocity, 0.0f, velocity });
        model->setRotation({ 0.0f, 1.0f, 0.0f }, -45.0f);

    }
    if (engine.IsKeyDown(D_KEY_W) || engine.IsGamepadButtonDown(0, D_GAMEPAD_BUTTON_DPAD_UP)) { // +X +Z
        camera->position.x += velocity;
        camera->target.x += velocity;

        camera->position.z += velocity;
        camera->target.z += velocity;

        model->translate({ velocity, 0.0f, velocity });
        model->setRotation({ 0.0f, 1.0f, 0.0f }, 45.0f);

    }
    if (engine.IsKeyDown(D_KEY_S) || engine.IsGamepadButtonDown(0, D_GAMEPAD_BUTTON_DPAD_DOWN)) { // -X -Z
        camera->position.x -= velocity;
        camera->target.x -= velocity;

        camera->position.z -= velocity;
        camera->target.z -= velocity;

        model->translate({ -velocity, 0.0f, -velocity });
        model->setRotation({ 0.0f, 1.0f, 0.0f }, 225.0f);

    }
    
    if (engine.IsKeyDown(D_KEY_G))
        model->scale({ 0.9,0.9,0.9 });
    if (engine.IsKeyDown(D_KEY_J))
        model->scale({ 1.1,1.1,1.1 });

    if (engine.IsKeyPressed(D_KEY_F7))
        engine.GetRootNode()->drawTree();

    if (engine.IsKeyPressed(D_KEY_F10))
        engine.ToggleLights();
    if (engine.IsKeyPressed(D_KEY_F11))
        engine.ToggleShaderCartoon();

    model->translate({ 0.0f, 0.0f, 0.0f });
}

void managerShaderCartoon(DarkMoon::DarkMoonEngine& engine, bool comments) {

    if(engine.IsKeyDown(D_KEY_1))
        engine.SetDepthThresholdValue(engine.GetDepthThresholdValue() + 0.01f);
    if(engine.IsKeyDown(D_KEY_2))
        engine.SetDepthThresholdValue(engine.GetDepthThresholdValue() - 0.01f < 0 ? 0 : engine.GetDepthThresholdValue() - 0.01f);

    if(engine.IsKeyDown(D_KEY_3))
        engine.SetNormalThresholdValue(engine.GetNormalThresholdValue() + 0.01f);
    if(engine.IsKeyDown(D_KEY_4))
        engine.SetNormalThresholdValue(engine.GetNormalThresholdValue() - 0.01f < 0 ? 0 : engine.GetNormalThresholdValue() - 0.01f);

    if(comments){
        std::cout << "---------------------------------\n";
        std::cout << "DepthThreshold: " << engine.GetDepthThresholdValue() << "\n";
        std::cout << "NormalThreshold: " << engine.GetNormalThresholdValue() << "\n";
    }
}

int main() {
    DarkMoon::DarkMoonEngine engine;

    if (engine.InitWindow(800, 600, "DarkMoon Engine")) {

        std::cout << "┌────────────────┐\n";
        std::cout << "│ Load Resources │\n";
        std::cout << "└────────────────┘\n";

        createScene3D(engine);
        createMainCharacter(engine);
        createHUD(engine);

        engine.SetExitKey(D_KEY_F8);
        engine.SetTargetFPS(60);

        engine.ActivateLights();
        //engine.ActivateShaderCartoon();

        while (!engine.WindowShouldClose()) {
            
            // Logic

            inputManager(engine);

            managerShaderCartoon(engine, false);

            if(engine.IsKeyPressed(D_KEY_0))
                engine.ToggleShaderCartoon();

            // Draw
    
            engine.BeginDrawing();

            engine.ClearBackground(D_WHITE);
            engine.GetRootNode()->traverse(glm::mat4());

            engine.EndDrawing();
        }

        engine.CloseWindow();
    }

    return 0;
}
/*
    Patron Dirty //
*/