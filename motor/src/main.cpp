#include "darkmoon.hpp"

Node* createHUD(DarkMoonEngine& engine){
    // Node HUD
    auto p_nodeHUD = engine.CreateNode("HUD", engine.GetRootNode());
    
    // Node: Rectangulo rosa Oscuro
    engine.CreateRectangle({12.0f, 12.0f}, {200.0f, 40.0f}, PINK_DARK, "Rectangulo rosa oscuro", p_nodeHUD);
    // Node: Rectangulo rosa
    engine.CreateRectangle({10.0f, 10.0f}, {200.0f, 40.0f}, PINK, "Rectangulo rosa", p_nodeHUD);

    // Node: Texture2D
    auto nodeTexture = engine.CreateTexture2D({70.0f, 170.0f}, "assets/koromaru.png", WHITE, "Textura Koromaru", p_nodeHUD);
    nodeTexture->scale({0.3f, 0.3f, 1.0f});

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

Node* createScene3D(DarkMoonEngine& engine){
    // Node Scene 3D
    auto p_node3D = engine.CreateNode("Scene 3D", engine.GetRootNode());

    // Node: Rejilla
    engine.CreateGrid(10, 1.0f, GRAY, "Rejilla principal", p_node3D);

    // Node: Linea diagonal
    engine.CreatePoint3D({-1.0f, 0.0f, 1.0f}, 5.0f, BLACK, "Punto principio linea", p_node3D);
    engine.CreateLine3D({-1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, -1.0f}, 2.0f, YELLOW_DARK, "Linea amarilla", p_node3D);
    engine.CreatePoint3D({1.0f, 1.0f, -1.0f}, 5.0f, BLACK, "Punto fin linea", p_node3D);

    // Node: Plano
    engine.CreatePlane({0.0f, 0.0f, 0.0f}, {2.0f, 2.0f}, PINK, "Plano rosita", p_node3D);

    // Node: Cubo
    engine.CreateCube({-3.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, AQUA_DARK, "Cubo azulito", p_node3D);
    engine.CreateCubeWires({-3.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, BLACK, "Wireframe", p_node3D);
    // Node: Wireframe
    engine.CreateCubeWires({2.0f, 0.5f, -3.0f}, {1.0f, 1.0f, 1.0f}, BLACK, "Wireframe", p_node3D);

    // Node: Modelo Dummy 1
    auto modelDummy = engine.CreateModel("assets/Dummy.obj", WHITE, "Modelo: Dummy 1", p_node3D);
    modelDummy->scale({0.2f, 0.2f, 0.2f});
    modelDummy->translate({0.0f, -0.5f, -3.0f});

    // Node: Modelo Dummy 2
    auto modelDummy2 = engine.CreateModel("assets/Dummy.obj", WHITE, "Modelo: Dummy 2", p_node3D);
    modelDummy2->scale({0.2f, 0.2f, 0.2f});
    modelDummy2->translate({0.0f, -0.5f, 3.0f});
    modelDummy2->rotate({0.0f, 1.0f, 0.0f}, 180.0f);

    // Node: Modelo Dummy 3
    auto modelDummy3 = engine.CreateModel("assets/Dummy.obj", WHITE, "Modelo: Dummy 3", p_node3D);
    modelDummy3->scale({0.2f, 0.2f, 0.2f});
    modelDummy3->translate({3.0f, -0.5f, 0.0f});
    modelDummy3->rotate({0.0f, 1.0f, 0.0f}, 270.0f);

    return p_node3D;
}

Node* createMainCharacter(DarkMoonEngine& engine){
    // Node: Modelo
    auto model = engine.CreateModel("assets/Nomada.obj", WHITE, "Modelo: Main Character", engine.GetRootNode());
    model->scale({0.2f, 0.2f, 0.2f});
    model->translate({0.0f, 0.0f, 0.0f});
    model->rotate({0.0f, 1.0f, 0.0f}, 225.0f);

    auto eModel = dynamic_cast<Model*>(model->getEntity());
    eModel->drawModel = true;
    eModel->drawWires = false;

    return model;
}

void inputManager(DarkMoonEngine& engine, Node* nodeCharacter){
    auto camera = engine.GetCamera();

    if(engine.IsKeyPressed(KEY_A)){ // +X -Z
        camera->position.x += 0.1f;
        camera->target.x   += 0.1f;

        camera->position.z -= 0.1f;
        camera->target.z   -= 0.1f;

        nodeCharacter->translate({0.1f, 0.0f, -0.1f});
        nodeCharacter->setRotation({0.0f, 1.0f, 0.0f}, 135.0f);
    }
    if(engine.IsKeyPressed(KEY_D)){ // -X +Z
        camera->position.x -= 0.1f;
        camera->target.x   -= 0.1f;

        camera->position.z += 0.1f;
        camera->target.z   += 0.1f;

        nodeCharacter->translate({-0.1f, 0.0f, 0.1f});
        nodeCharacter->setRotation({0.0f, 1.0f, 0.0f}, 315.0f);
    }
    if(engine.IsKeyPressed(KEY_W)){ // +X +Z
        camera->position.x += 0.1f;
        camera->target.x   += 0.1f;

        camera->position.z += 0.1f;
        camera->target.z   += 0.1f;

        nodeCharacter->translate({0.1f, 0.0f, 0.1f});
        nodeCharacter->setRotation({0.0f, 1.0f, 0.0f}, 45.0f);
    }
    if(engine.IsKeyPressed(KEY_S)){ // -X -Z
        camera->position.x -= 0.1f;
        camera->target.x   -= 0.1f;

        camera->position.z -= 0.1f;
        camera->target.z   -= 0.1f;

        nodeCharacter->translate({-0.1f, 0.0f, -0.1f});
        nodeCharacter->setRotation({0.0f, 1.0f, 0.0f}, 225.0f);
    }
}


int main(){
    DarkMoonEngine engine;

    if(engine.InitWindow(800, 600, "DarkMoon Engine")){

        std::cout << "┌────────────────┐" << std::endl;
        std::cout << "│ Load Resources │" << std::endl;
        std::cout << "└────────────────┘" << std::endl;

        createHUD(engine);
        createScene3D(engine);
        auto mainCharacter = createMainCharacter(engine);

        //scene3D->getParent()->removeChild(scene3D);

        std::cout << "┌──────┐" << std::endl;
        std::cout << "│ Tree │" << std::endl;
        std::cout << "└──────┘" << std::endl;
        engine.GetRootNode()->drawTree();

        while(!engine.WindowShouldClose()){
    
            // Logic

            inputManager(engine, mainCharacter);

            // Draw

            engine.BeginDrawing();

            engine.ClearBackground(WHITE);
            engine.GetRootNode()->traverse(glm::mat4());

            engine.EndDrawing();
        }
    }

    return 0;
}
/*
    // Patron Dirty //
    // renm.drawTexture3D(rTexture2, {0.0f, 0.0f}, 50.0f, 0.01f, {1.0f, 1.0f, 1.0f, 1.0f});
*/