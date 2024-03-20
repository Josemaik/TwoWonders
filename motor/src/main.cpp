#include "darkmoon.hpp"

Node* createHUD(DarkMoonEngine& engine){
    // Node HUD
    auto p_nodeHUD = engine.CreateNode("HUD", engine.GetRootNode());
    
    // Node: Rectangulo Naranja Oscuro
    engine.CreateRectangle({12.0f, 12.0f}, {200.0f, 40.0f}, ORANGE_DARK, "Rectangulo naranja oscuro", p_nodeHUD);
    // Node: Rectangulo Naranja
    engine.CreateRectangle({10.0f, 10.0f}, {200.0f, 40.0f}, ORANGE, "Rectangulo naranja", p_nodeHUD);

    // Node: Texture2D
    auto nodeTexture = engine.CreateTexture2D({70.0f, 170.0f}, "assets/koromaru.png", {255, 255, 255, 0}, "Textura Koromaru", p_nodeHUD);
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

int main(){
    DarkMoonEngine engine;

    if(engine.InitWindow(800, 600, "DarkMoon Engine")){

        auto nodeHUD = createHUD(engine);

        std::cout << "┌──────┐" << std::endl;
        std::cout << "│ Tree │" << std::endl;
        std::cout << "└──────┘" << std::endl;
        engine.GetRootNode()->drawTree();

        while(!engine.WindowShouldClose()){

            // Logic

            if(engine.IsKeyPressed(KEY_A))
                nodeHUD->translate({-1.0f,  0.0f, 0.0f});
            if(engine.IsKeyPressed(KEY_D))
                nodeHUD->translate({ 1.0f,  0.0f, 0.0f});
            if(engine.IsKeyPressed(KEY_W))
                nodeHUD->translate({ 0.0f, -1.0f, 0.0f});
            if(engine.IsKeyPressed(KEY_S))
                nodeHUD->translate({ 0.0f,  1.0f, 0.0f});

            // Draw

            engine.BeginDrawing();

            engine.ClearBackground(WHITE);

            engine.BeginMode3D();
            engine.DrawGrid(10, 1.0f, GRAY);
            engine.EndMode3D();
        
            engine.GetRootNode()->traverse(glm::mat4());

            engine.EndDrawing();
        }
    }

    return 0;
}
/*
#include "darkmoon.hpp"

int main(){
    DarkMoonEngine engine;

    if(engine.InitWindow(800, 600, "DarkMoon Engine")){

        // Auxiliars

        // if(im.isGamePadAvailable(0) == 1)
        //     std::cout << "Gamepad: " << im.getGamePadName(0) << std::endl;
        // 

        if(engine.IsGamepadAvailable(0))
            std::cout << "Gamepad: " << engine.GetGamepadName(0) << std::endl;

        glm::vec3 positionCharacter = {0.0f, 0.0f, 0.0f};
        float angleCharacter = 0.0f;

        // ---- //
        // Node //
        // ---- //

        auto nodoEspecial = engine.CreateNode("Yema");
        engine.GetRootNode()->addChild(std::move(nodoEspecial));

        // ------ //
        // Camera //
        // ------ //

        auto camera = engine.CreateCamera("Camera");
        engine.UseCamera(camera);

        // camera->cameraProjection = CameraProjection::CAMERA_ORTHOGRAPHIC;

        // ---- //
        // Load //
        // ---- //

        // auto font = engine.LoadFont("assets/fonts/roboto.ttf");
        // auto shader = engine.LoadShader("src/shaders/texture.vs", "src/shaders/texture.fs");
        // auto texture = engine.LoadTexture("assets/koromaru.png");
        // auto texture2 = engine.LoadTexture("assets/wall.jpg");

        auto model = engine.LoadModel("assets/Dummy.obj");

        std::cout << "┌──────┐" << std::endl;
        std::cout << "│ Tree │" << std::endl;
        std::cout << "└──────┘" << std::endl;
        engine.GetRootNode()->drawTree();

        // ------ //
        // Unload //
        // ------ //

        //engine.UnloadTexture(texture);
        //engine.UnloadModel(model);

        while(!engine.WindowShouldClose()){
            // ----- //
            // Logic //
            // ----- //

            //std::cout << engine.GetGamepadAxisMovement(0, 0) << std::endl;

            //if(engine.IsGamepadButtonReleased(0, GLFW_GAMEPAD_BUTTON_A))
            //    std::cout << "Boton levantado" << std::endl;

            //std::cout << "X: " << engine.GetMouseX() << " - Y: " << engine.GetMouseY() << std::endl;
            // if(engine.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
            //     std::cout << "Boton izquierdo apretado" << std::endl;
            // if(engine.IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
            //     std::cout << "Boton izquierdo mantenido" << std::endl;
            // if(engine.IsMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT))
            //     std::cout << "Boton izquierdo levantado" << std::endl;
            // if(engine.IsMouseButtonUp(GLFW_MOUSE_BUTTON_LEFT))
            //    std::cout << "Boton izquierdo levantando" << std::endl;

            // engine.SetMousePosition(engine.GetScreenWidth() / 2, engine.GetScreenHeight() / 2);

            if(engine.IsKeyPressed(KEY_A) || engine.IsGamepadButtonPressed(0, GLFW_GAMEPAD_BUTTON_DPAD_LEFT)){
                positionCharacter.x -= 0.1f;
                camera->position.x  -= 0.1f;
                angleCharacter       = 270.0f;
            }
            if(engine.IsKeyPressed(KEY_D) || engine.IsGamepadButtonPressed(0, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT)){
                positionCharacter.x += 0.1f;
                camera->position.x  += 0.1f;
                angleCharacter       = 90.0f;
            }
            if(engine.IsKeyPressed(KEY_W) || engine.IsGamepadButtonPressed(0, GLFW_GAMEPAD_BUTTON_DPAD_UP)){
                positionCharacter.z -= 0.1f;
                camera->position.z  -= 0.1f;
                angleCharacter       = 180.0f;
            }
            if(engine.IsKeyPressed(KEY_S) || engine.IsGamepadButtonPressed(0, GLFW_GAMEPAD_BUTTON_DPAD_DOWN)){
                positionCharacter.z += 0.1f;
                camera->position.z  += 0.1f;
                angleCharacter       = 0.0f;
            }

            if(engine.IsKeyPressed(KEY_UP))
                camera->fovy    -= 1.0f;
            if(engine.IsKeyPressed(KEY_DOWN))
                camera->fovy    += 1.0f;

            if(engine.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
                camera->position.y -= 0.1f;
            if(engine.IsKeyPressed(KEY_SPACE))
                camera->position.y += 0.1f;

            camera->target = positionCharacter;

            camera->updateCameraVectors();

            // ------- //
            // Drawing //
            // ------- //

            engine.BeginDrawing();
            engine.ClearBackground(WHITE);

            // 3D

            engine.BeginMode3D();

            engine.DrawPoint3DExtra({0.0f, 0.0f, 0.0f}, 5.0f, BLACK);
            engine.DrawPoint3DExtra({0.0f, 1.0f, 0.0f}, 5.0f, BLACK);
            engine.DrawPoint3DExtra({0.0f, -1.0f, 0.0f}, 5.0f, BLACK);
            engine.DrawPoint3DExtra({1.0f, 0.0f, -1.0f}, 5.0f, BLACK);
            engine.DrawPoint3DExtra({1.0f, 1.0f, -1.0f}, 5.0f, BLACK);

            engine.DrawLine3D({0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, 1.0f, {255, 255, 0, 255});
            engine.DrawLine3D({-1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, -1.0f}, 1.0f, {255, 255, 0, 255});
            engine.DrawLine3D({1.0f, 0.0f, -1.0f}, {1.0f, 1.0f, -1.0f}, 1.0f, {255, 255, 0, 255}); 

            engine.DrawGrid(10, 1.0f, {140, 140, 140, 255});
            engine.DrawPlane({0.0f, 0.0f, 0.0f}, {2.0f, 2.0f}, PINK);

            engine.DrawCubeV({2.0f, 0.0f, -3.0f}, {1.0f, 1.0f, 1.0f}, AQUA_DARK);
            // engine.DrawCubeWiresV({2.0f, 0.0f, -3.0f}, {1.0f, 1.0f, 1.0f}, BLACK);
            // engine.DrawCubeWiresV({-3.0f, 2.0f, 0.0f}, {1.0f, 1.0f, 3.0f}, RED);
            
            //engine.DrawModel(model, {0.0f, 0.0f, 0.0f}, 0.2f, {255, 0, 0, 255});
            //engine.DrawModelWires(model, {0.0f, 0.0f, 0.0f}, 0.2f, BLACK);
            engine.DrawModelExtra(model, positionCharacter, 0.15f, {0.0f, 1.0f, 0.0f}, angleCharacter, GRAY);
            engine.DrawModelWiresExtra(model, positionCharacter, 0.15f, {0.0f, 1.0f, 0.0f}, angleCharacter, BLACK);

            engine.EndMode3D();

            // 2D

            // engine.DrawText("Hola", 0, 0, 24, BLACK);

            engine.DrawRectangle(12, 12, 200, 40, {128, 128, 128, 255});     
            engine.DrawRectangleV({10, 10}, {200, 40}, {180, 180, 180, 255});
            engine.DrawRectangleLines({10, 10}, {200, 40}, BLACK);    

            engine.DrawPixel(engine.GetScreenWidth() / 2, engine.GetScreenHeight() / 2, {0, 0, 0, 255});
            engine.DrawPixelV({20.0f, 40.0f}, BLACK);

            engine.DrawTriangle({560.0f, 300.0f}, {10.0f, 590.0f}, {410.0f, 590.0f}, {255, 128, 50, 255});
            engine.DrawTriangleLines({560.0f, 300.0f}, {10.0f, 590.0f}, {410.0f, 590.0f}, BLACK);

            engine.DrawCircle(engine.GetScreenWidth() / 2, engine.GetScreenHeight() / 2, 30.0f, 20, {100, 0, 0, 255});
            engine.DrawCircleV({engine.GetScreenWidth() / 2, engine.GetScreenHeight() / 2}, 20.0f, 20, {255, 100, 0, 255});

            engine.DrawTextureV(texture2, {engine.GetScreenWidth() / 2, engine.GetScreenHeight() / 2}, WHITE);
            engine.DrawTextureEx(texture, {engine.GetScreenWidth() / 2, engine.GetScreenHeight() / 2}, 0.0f, 0.2f, WHITE);

            engine.DrawLine(0, 0, engine.GetScreenWidth(), engine.GetScreenHeight(), {140, 140, 140, 255});
            engine.DrawLine(0, engine.GetScreenHeight(), engine.GetScreenWidth(), 0, {140, 140, 140, 255});
            

            engine.EndDrawing();
        }

        engine.CloseWindow();
    }

    return 0;
}
*/
/*
    // Patron Dirty //
    // renm.drawTexture3D(rTexture2, {0.0f, 0.0f}, 50.0f, 0.01f, {1.0f, 1.0f, 1.0f, 1.0f});
*/