#include "darkmoon.hpp"

int main(){
    DarkMoonEngine engine;

    if(engine.InitWindow(800, 600, "DarkMoon Engine")){

        // ---- //
        // Load //
        // ---- //

        // auto font = engine.LoadFont("assets/fonts/roboto.ttf");
        // auto shader = engine.LoadShader("src/shaders/texture.vs", "src/shaders/texture.fs");
        auto texture = engine.LoadTexture("assets/koromaru.png");
        auto texture2 = engine.LoadTexture("assets/wall.jpg");

        while(!engine.WindowShouldClose()){
            // ----- //
            // Logic //
            // ----- //

            /* PRUEBAS
            if(engine.IsKeyPressed(KEY_Q))
                engine.SetWindowSize(engine.GetScreenWidth() + 10, engine.GetScreenHeight() + 10);
            if(engine.IsKeyPressed(KEY_W))
                engine.SetWindowSize(engine.GetScreenWidth() - 10, engine.GetScreenHeight() - 10);
            */

            // ------- //
            // Drawing //
            // ------- //

            engine.BeginDrawing();
            engine.ClearBackground(WHITE);

            // 3D

            engine.BeginMode3D();



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

            renm.drawPoint3D({0.0f, 0.0f, 0.0f}, 5.0f, {0.0f, 0.0f, 0.0f, 1.0f});
            renm.drawPoint3D({0.0f, 1.0f, 0.0f}, 5.0f, {0.0f, 0.0f, 0.0f, 1.0f});
            renm.drawPoint3D({0.0f, -1.0f, 0.0f}, 5.0f, {0.0f, 0.0f, 0.0f, 1.0f});
            renm.drawPoint3D({1.0f, 0.0f, -1.0f}, 5.0f, {0.0f, 0.0f, 0.0f, 1.0f});
            renm.drawPoint3D({1.0f, 1.0f, -1.0f}, 5.0f, {0.0f, 0.0f, 0.0f, 1.0f});

            renm.drawLine3D({0.0f, 1.0f, 0.0f}, 1.0f, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f});
            renm.drawLine3D({-1.0f, 0.0f, 1.0f}, 1.0f, {1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f, 1.0f});
            renm.drawLine3D({1.0f, 0.0f, -1.0f}, 1.0f, {1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}); 

            renm.drawPlane({0.0f, 0.0f, 0.0f}, {2.0f, 2.0f}, {1.0f, 0.5f, 0.5f, 1.0f});
            renm.drawGrid(10, 1.0f, {0.5f, 0.5f, 0.5f, 0.0f});

            renm.drawCube({2.0f, 0.0f, -3.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f});
            renm.drawCubeWires({-3.0f, 2.0f, 0.0f}, {1.0f, 1.0f, 3.0f}, {0.5f, 0.0f, 0.5f, 1.0f});

            renm.endMode3D();
        }
    }

    //----- Draw -----//
    // std::cout << "┌──────┐" << std::endl;
    // std::cout << "│ Draw │" << std::endl;
    // std::cout << "└──────┘" << std::endl;
    // nScene->traverse(glm::mat4());

    return 0;
}

std::shared_ptr<Model> loadModel(const char* filePath, std::shared_ptr<Node> nScene, ResourceManager& rm){
    auto nModel = std::make_unique<Node>();
    nModel->name = filePath;
    auto eModel = std::make_shared<Model>();
    eModel->load(filePath, rm);
    if(eModel->isLoaded()){
        nModel->setEntity(eModel);
        nScene->addChild(std::move(nModel));
    }
    return eModel;
}
*/