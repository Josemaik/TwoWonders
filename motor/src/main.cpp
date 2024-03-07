#include "darkmoon.hpp"

int main(){
    DarkMoonEngine engine;

    if(engine.InitWindow(800, 600, "DarkMoon Engine")){

        while(!engine.WindowShouldClose()){
            // ----- //
            // Logic //
            // ----- //

            if(engine.IsKeyPressed(KEY_Q))
                engine.SetWindowSize(1024, 800);
            if(engine.IsKeyPressed(KEY_W))
                engine.SetWindowSize(800, 600);

            // ------- //
            // Drawing //
            // ------- //

            engine.BeginDrawing();
            engine.ClearBackground(WHITE);

            /* PRUEBAS
            engine.DrawPixel(engine.GetScreenWidth() / 2, engine.GetScreenHeight() / 2, {0, 0, 0, 255});
            engine.DrawPixelV({20.0f, 40.0f}, BLACK);

            engine.DrawLine(0, 0, engine.GetScreenWidth(), engine.GetScreenHeight(), {255, 0, 0, 255});
            engine.DrawLineV({0, 0}, {100.0f, 20.0f}, {255, 0, 255, 255});

            engine.DrawTriangle({560.0f, 300.0f}, {10.0f, 590.0f}, {410.0f, 590.0f}, {255, 128, 50, 255});
            engine.DrawTriangleLines({560.0f, 300.0f}, {10.0f, 590.0f}, {410.0f, 590.0f}, BLACK);

            engine.DrawRectangle(12, 12, 200, 40, {128, 128, 128, 255});     
            engine.DrawRectangleV({10, 10}, {200, 40}, {180, 180, 180, 255});
            engine.DrawRectangleLines({10, 10}, {200, 40}, BLACK);

            engine.DrawCircle(engine.GetScreenWidth() / 2, engine.GetScreenHeight() / 2, 30.0f, 20, {100, 0, 0, 255});
            engine.DrawCircleV({engine.GetScreenWidth() / 2, engine.GetScreenHeight() / 2}, 20.0f, 20, {255, 100, 0, 255});
            */

            engine.EndDrawing();
        }

        engine.CloseWindow();
    }

    return 0;
}

/*
std::shared_ptr<Model> loadModel(const char*, std::shared_ptr<Node>, ResourceManager& rm);
std::shared_ptr<Texture> loadTexture(const char*, ResourceManager&);

    // Patron Dirty //

        //----- Gamepad -----//
        if(im.isGamePadAvailable(0) == 1)
            std::cout << "Gamepad: " << im.getGamePadName(0) << std::endl;

        //----- Load model -----// 
        std::cout << "┌──────┐" << std::endl;
        std::cout << "│ Load │" << std::endl;
        std::cout << "└──────┘" << std::endl;
        auto filePath = "assets/dummy.obj";
        auto eModel = loadModel(filePath, nScene, rm);

        //------ Textures -----//
        auto rTexture = loadTexture("assets/container.jpg", rm); 
        auto rTexture2 = loadTexture("assets/koromaru.png", rm); 

        //------ Shaders -----//
        auto rShaderColor = rm.loadResource<Shader>("src/shaders/color.vs", "src/shaders/color.fs", ShaderType::COLOR);
        auto rShaderTexture = rm.loadResource<Shader>("src/shaders/texture.vs", "src/shaders/texture.fs", ShaderType::TEXTURE);
        auto rShaderTexture3D = rm.loadResource<Shader>("src/shaders/texture3D.vs", "src/shaders/texture3D.fs", ShaderType::TEXTURE3D);

            // Input
            im.update();

            renm.beginMode3D();

            // Draw (texture) -> 3D
            renm.useShader(rShaderTexture3D);
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

            // Draw (color)
            renm.useShader(rShaderColor);

            //renm.drawTriangle({560.0f, 300.0f}, {10.0f, 590.0f}, {410.0f, 590.0f}, {1.0f, 0.5f, 0.2f, 1.0f});
            //renm.drawLine({0.0f, 0.0f}, {500.0f, 200.0f}, {1.0f, 0.0f, 0.0f, 1.0f});
            //renm.drawPixel({400.0f, 300.0f}, {0.0f, 0.0f, 0.0f, 1.0f});
            //renm.drawCircle({100.0f, 300.0f}, 60.0f, 20, {1.0f, 1.0f, 0.0f, 1.0f});

            // HUD
            renm.drawRectangle({12.0f, 12.0f}, {200.0f, 40.0f}, {0.5f, 0.5f, 0.5f, 1.0f});
            renm.drawRectangle({10.0f, 10.0f}, {200.0f, 40.0f}, {0.7f, 0.7f, 0.7f, 1.0f});

            // Draw (texture)
            // renm.useShader(rShaderTexture);
            // renm.drawTextureExtra(rTexture, {100.0f, 150.0f}, 120.0f, 0.3f, {1.0f, 1.0f, 1.0f, 1.0f});
            // renm.drawTexture(rTexture, {0.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f});

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

std::shared_ptr<Texture> loadTexture(const char* filePath, ResourceManager& rm){
    auto texture = rm.loadResource<Texture>();

    texture->load(filePath);

    return texture;
}
*/