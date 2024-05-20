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

        engine.SetExitKey(D_KEY_F8);
        engine.SetTargetFPS(60);

        engine.ActivateLights();
        engine.ActivateShaderCartoon();

        while (!engine.WindowShouldClose()) {
            
            // Logic

            inputManager(engine);

            managerShaderCartoon(engine, true);

            if(engine.IsKeyPressed(D_KEY_9)){
                engine.ActivateShaderCartoon();
                engine.SetDepthThresholdValue(0.5f);
                engine.SetNormalThresholdValue(1.5f);
            }
            else if(engine.IsKeyPressed(D_KEY_0)){
                engine.DeactivateShaderCartoon();
                engine.SetDepthThresholdValue(0.5f);
                engine.SetNormalThresholdValue(4.0f);
            }

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