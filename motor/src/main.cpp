#include "darkmoon.hpp"

int main(){
    DarkMoonEngine engine;

    if(engine.InitWindow(800, 600, "DarkMoon Engine")){

        // ------ //
        // Camera //
        // ------ //

        auto camera = engine.CreateCamera("Main");
        engine.UseCamera(camera);

        // camera->cameraProjection = CameraProjection::CAMERA_ORTHOGRAPHIC;

        // ---- //
        // Load //
        // ---- //

        // auto font = engine.LoadFont("assets/fonts/roboto.ttf");
        // auto shader = engine.LoadShader("src/shaders/texture.vs", "src/shaders/texture.fs");
        auto texture = engine.LoadTexture("assets/koromaru.png");
        auto texture2 = engine.LoadTexture("assets/wall.jpg");

        auto model = engine.LoadModel("assets/main_character.obj");

        while(!engine.WindowShouldClose()){
            // ----- //
            // Logic //
            // ----- //

            if(engine.IsKeyPressed(KEY_A)){
                camera->target.x   -= 0.1f;
                camera->position.x -= 0.1f;
            }
            if(engine.IsKeyPressed(KEY_D)){
                camera->target.x   += 0.1f;
                camera->position.x += 0.1f;
            }
            if(engine.IsKeyPressed(KEY_W)){
                camera->target.z   -= 0.1f;
                camera->position.z -= 0.1f;
            }
            if(engine.IsKeyPressed(KEY_S)){
                camera->target.z   += 0.1f;
                camera->position.z += 0.1f;
            }

            if(engine.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)){
                camera->target.y   -= 0.1f;
                camera->position.y -= 0.1f;
            }
            if(engine.IsKeyPressed(KEY_SPACE)){
                camera->target.y   += 0.1f;
                camera->position.y += 0.1f;
            }

            // ------- //
            // Drawing //
            // ------- //

            engine.BeginDrawing();
            engine.ClearBackground(WHITE);

            // 3D

            engine.BeginMode3D();

            /* PRUEBA
            engine.DrawPoint3DExtra({0.0f, 0.0f, 0.0f}, 5.0f, BLACK);
            engine.DrawPoint3DExtra({0.0f, 1.0f, 0.0f}, 5.0f, BLACK);
            engine.DrawPoint3DExtra({0.0f, -1.0f, 0.0f}, 5.0f, BLACK);
            engine.DrawPoint3DExtra({1.0f, 0.0f, -1.0f}, 5.0f, BLACK);
            engine.DrawPoint3DExtra({1.0f, 1.0f, -1.0f}, 5.0f, BLACK);

            engine.DrawLine3D({0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, 1.0f, {255, 255, 0, 255});
            engine.DrawLine3D({-1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, -1.0f}, 1.0f, {255, 255, 0, 255});
            engine.DrawLine3D({1.0f, 0.0f, -1.0f}, {1.0f, 1.0f, -1.0f}, 1.0f, {255, 255, 0, 255}); 

            engine.DrawPlane({0.0f, 0.0f, 0.0f}, {2.0f, 2.0f}, {255, 128, 128, 255});
            engine.DrawCubeV({2.0f, 0.0f, -3.0f}, {1.0f, 1.0f, 1.0f}, {255, 0, 0, 255});
            engine.DrawCubeWiresV({2.0f, 0.0f, -3.0f}, {1.0f, 1.0f, 1.0f}, BLACK);
            engine.DrawCubeWiresV({-3.0f, 2.0f, 0.0f}, {1.0f, 1.0f, 3.0f}, {128, 0, 128, 255});
            */

            engine.DrawGrid(10, 1.0f, {140, 140, 140, 255});

            //engine.DrawModel(model, {0.0f, 0.0f, 0.0f}, 0.2f, {255, 0, 0, 255});
            engine.DrawModelExtra(model, {0.0f, 0.0f, 0.0f}, 0.2f, {0.0f, 1.0f, 0.0f}, 0.0f, {100, 100, 100, 255});

            engine.EndMode3D();

            // 2D

            // engine.DrawText("Hola", 0, 0, 24, BLACK);

            engine.DrawRectangle(12, 12, 200, 40, {128, 128, 128, 255});     
            engine.DrawRectangleV({10, 10}, {200, 40}, {180, 180, 180, 255});
            engine.DrawRectangleLines({10, 10}, {200, 40}, BLACK);    

            /* PRUEBAS
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
            */

            engine.EndDrawing();
        }

        engine.CloseWindow();
    }

    return 0;
}

/*
std::shared_ptr<Model> loadModel(const char*, std::shared_ptr<Node>, ResourceManager& rm);

    // Patron Dirty //

        //----- Gamepad -----//
        if(im.isGamePadAvailable(0) == 1)
            std::cout << "Gamepad: " << im.getGamePadName(0) << std::endl;

        //----- Load model -----// 
        auto filePath = "assets/dummy.obj";
        auto eModel = loadModel(filePath, nScene, rm);

            // Input
            im.update();

            // renm.drawTexture3D(rTexture2, {0.0f, 0.0f}, 50.0f, 0.01f, {1.0f, 1.0f, 1.0f, 1.0f});
            // Draw (model)
            // eModel->draw(glm::mat4());

        }
    }

    //----- Draw -----//
    // std::cout << "┌──────┐" << std::endl;
    // std::cout << "│ Draw │" << std::endl;
    // std::cout << "└──────┘" << std::endl;
    // nScene->traverse(glm::mat4());

    return 0;
}
*/