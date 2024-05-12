#include "darkmoon.hpp"

DarkMoon::Node* createHUD(DarkMoon::DarkMoonEngine& engine) {
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

    return p_nodeHUD;
}

DarkMoon::Node* createScene3D(DarkMoon::DarkMoonEngine& engine) {
    // Node Scene 3D
    auto p_node3D = engine.CreateNode("Scene 3D", engine.GetRootNode());

    // Node: Rejilla
    engine.CreateGrid(10, 10.0f, D_GRAY, "Rejilla principal", p_node3D);

    //Node: Linea diagonal
    engine.CreatePoint3D({ -1.0f, 0.0f, 1.0f }, 5.0f, D_BLACK, "Punto principio linea", p_node3D);
    engine.CreateLine3D({ -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, -1.0f }, 2.0f, D_YELLOW_DARK, "Linea amarilla", p_node3D);
    engine.CreatePoint3D({ 1.0f, 1.0f, -1.0f }, 5.0f, D_BLACK, "Punto fin linea", p_node3D);

    // Node: Plano
    engine.CreatePlane({ 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, D_PINK, "Plano rosita", p_node3D);

    // Node: Cubo
    engine.CreateCube({ -30.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f }, D_AQUA_DARK, "Cubo azulito", p_node3D);
    engine.CreateCubeWires({ -30.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f }, D_BLACK, "Wireframe", p_node3D);
    // Node: Wireframe
    engine.CreateCubeWires({ 20.0f, 0.0f, -30.0f }, { 10.0f, 10.0f, 10.0f }, D_BLACK, "Wireframe", p_node3D);

    // Node: Modelo Dummy 1
    auto modelDummy = engine.CreateModel("assets/Dummy.obj", D_WHITE, "Modelo: Dummy 1", p_node3D);
    //modelDummy->scale({0.2f, 0.2f, 0.2f});
    modelDummy->translate({ 0.0f, 0.0f, -30.0f });

    // Node: Modelo Dummy 2
    auto modelDummy2 = engine.CreateModel("assets/Dummy.obj", D_WHITE, "Modelo: Dummy 2", p_node3D);
    //modelDummy2->scale({0.2f, 0.2f, 0.2f});
    modelDummy2->translate({ 0.0f, 0.0f, 30.0f });
    modelDummy2->rotate({ 0.0f, 1.0f, 0.0f }, 180.0f);

    // Node: Modelo Dummy 3
    auto modelDummy3 = engine.CreateModel("assets/Dummy.obj", D_WHITE, "Modelo: Dummy 3", p_node3D);
    //modelDummy3->scale({0.2f, 0.2f, 0.2f});
    modelDummy3->translate({ 30.0f, 0.0f, 0.0f });
    modelDummy3->rotate({ 0.0f, 1.0f, 0.0f }, 270.0f);

    return p_node3D;
}

DarkMoon::Node* createMainCharacter(DarkMoon::DarkMoonEngine& engine) {
    // Node: Modelo
    auto model = engine.CreateModel("assets/lvl_0-cnk_0.obj", D_WHITE, "Modelo: Main Character", engine.GetRootNode());
    //model->scale({0.02f, 0.02f, 0.02f});
    //model->translate({0.0f, 0.0f, 0.0f});
    //model->rotate({ 1.0f, 0.0f, 0.0f }, -90.0f);

    auto eModel = dynamic_cast<DarkMoon::Model*>(model->getEntity());
    eModel->drawModel = true;
    eModel->drawWires = false;

    return model;
}

void inputManager(DarkMoon::DarkMoonEngine& engine, DarkMoon::Node* nodeCharacter) {
    auto camera = engine.GetCamera();
    float velocity = 0.5f;

    if (engine.IsKeyDown(D_KEY_A)) { // +X -Z
        camera->position.x += velocity;
        camera->target.x += velocity;

        camera->position.z -= velocity;
        camera->target.z -= velocity;

        nodeCharacter->translate({ velocity, 0.0f, -velocity });
        nodeCharacter->setRotation({ 0.0f, 1.0f, 0.0f }, 45.0f);
    }
    if (engine.IsKeyDown(D_KEY_D)) { // -X +Z
        camera->position.x -= velocity;
        camera->target.x -= velocity;

        camera->position.z += velocity;
        camera->target.z += velocity;

        nodeCharacter->translate({ -velocity, 0.0f, velocity });
        nodeCharacter->setRotation({ 0.0f, 1.0f, 0.0f }, 225.0f);
    }
    if (engine.IsKeyDown(D_KEY_W)) { // +X +Z
        camera->position.x += velocity;
        camera->target.x += velocity;

        camera->position.z += velocity;
        camera->target.z += velocity;

        nodeCharacter->translate({ velocity, 0.0f, velocity });
        nodeCharacter->setRotation({ 0.0f, 1.0f, 0.0f }, -45.0f);
    }
    if (engine.IsKeyDown(D_KEY_S)) { // -X -Z
        camera->position.x -= velocity;
        camera->target.x -= velocity;

        camera->position.z -= velocity;
        camera->target.z -= velocity;

        nodeCharacter->translate({ -velocity, 0.0f, -velocity });
        nodeCharacter->setRotation({ 0.0f, 1.0f, 0.0f }, 135.0f);
    }
    if (engine.IsKeyDown(D_KEY_G)) {
        nodeCharacter->scale({ 0.9,0.9,0.9 });
    }
    if (engine.IsKeyDown(D_KEY_H)) {
        nodeCharacter->scale({ 1.1,1.1,1.1 });
    }

    if (engine.IsKeyPressed(D_KEY_SPACE))
        engine.ToggleLights();
}

int main() {
    DarkMoon::DarkMoonEngine engine;

    if (engine.InitWindow(800, 600, "DarkMoon Engine")) {

        // std::cout << "┌────────────────┐" << std::endl;
        // std::cout << "│ Load Resources │" << std::endl;
        // std::cout << "└────────────────┘" << std::endl;

        createScene3D(engine);
        auto mainCharacter = createMainCharacter(engine);
        createHUD(engine);

        auto camera = engine.GetCamera();
        camera->position = { -60.0f, 60.0f, -60.0f };
        camera->fovy = 60.0f;

        //engine.CreatePointLight({0, 10, 0}, D_YELLOW, "Luz amarilla prueba", engine.GetRootNode());
        auto node_light = engine.CreateSpotLight({0, 10, 0}, {0, -1, 0}, 0.2f, D_RED, "Linterna roja", engine.GetRootNode());
        
        auto light = node_light->getEntity<DarkMoon::SpotLight>();

        engine.SetExitKey(D_KEY_F8);
        engine.SetTargetFPS(60);

        //scene3D->getParent()->removeChild(scene3D);

        engine.DrawTree();

        while (!engine.WindowShouldClose()) {

            // Logic

            inputManager(engine, mainCharacter);

            if(engine.IsKeyDown(D_KEY_I))
                light->position.y -= 1.0f;
            if(engine.IsKeyDown(D_KEY_K))
                light->position.y += 1.0f;

            DarkMoon::Ray ray = engine.GetMouseRay();
            light->position = ray.origin;
            light->direction = ray.direction;

            //std::cout << "Posicion: " << light->position.y << "\n";

            //textBoxEntity->text.text = std::to_string(engine.GetFPS());            

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
    // Patron Dirty //
*/