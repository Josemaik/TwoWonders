#include "components/node.hpp"
#include "components/entity.hpp"
#include "components/entity_model.hpp"
#include "managers/resource_manager.hpp"
#include "managers/windows_manager.hpp"
#include "managers/render_manager.hpp"
#include "components/resource_shader.hpp"

#include <iostream>

std::shared_ptr<Node> createSceneTree();
std::shared_ptr<Model> loadModel(const char*, std::shared_ptr<Node>, ResourceManager& rm);
std::shared_ptr<Texture> loadTexture(const char*, ResourceManager&);

int main(){

    //----- Initialize managers -----//
    WindowsManager wm;
    ResourceManager rm;
    RenderManager renm;

    //----- Create scene tree -----//
    auto nScene = createSceneTree();

    if(wm.initWindow(800, 600, "Salty Pixel")){
        //----- Load model -----// 
        std::cout << "┌──────┐" << std::endl;
        std::cout << "│ Load │" << std::endl;
        std::cout << "└──────┘" << std::endl;
        auto filePath = "assets/dummy.obj";
        auto eModel = loadModel(filePath, nScene, rm);

        filePath = "assets/container.jpg";
        auto rTexture = loadTexture(filePath, rm); 

        auto rShaderColor = rm.loadResource<Shader>("src/shaders/color.vs", "src/shaders/color.fs", ShaderType::COLOR);
        auto rShaderTexture = rm.loadResource<Shader>("src/shaders/texture.vs", "src/shaders/texture.fs", ShaderType::TEXTURE);

        //----- View tree -----//
        std::cout << "┌──────┐" << std::endl;
        std::cout << "│ Tree │" << std::endl;
        std::cout << "└──────┘" << std::endl;
        nScene->drawTree();

        //------ Shaders -----//
        // renm.compilingShaders();

        // Main loop
        while(!wm.windowShouldClose()){
            wm.beginDrawing();

            renm.clearBackground({1.0f, 1.0f, 1.0f, 1.0f});

            // Draw (color)
            renm.useShader(rShaderColor);

            renm.drawTriangle({560.0f, 300.0f}, {10.0f, 590.0f}, {410.0f, 590.0f}, {1.0f, 0.5f, 0.2f, 1.0f});
            renm.drawPixel({400.0f, 300.0f}, {0.0f, 0.0f, 0.0f, 1.0f});

            // HUD
            renm.drawRectangle({12.0f, 12.0f}, {200.0f, 40.0f}, {0.5f, 0.5f, 0.5f, 1.0f});
            renm.drawRectangle({10.0f, 10.0f}, {200.0f, 40.0f}, {0.7f, 0.7f, 0.7f, 1.0f});

            // Draw (texture)
            renm.useShader(rShaderTexture);

            // renm.drawTexture(rTexture, {0.0f, 500.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
            renm.drawTextureExtra(rTexture, {10.0f, 60.0f}, 0.0f, 0.6f, {1.0f, 1.0f, 1.0f, 1.0f});

            // Draw (model)
            //eModel->draw(glm::mat4());

            wm.endDrawing();
        }

        //----- Unload -----//
        std::cout << "┌────────┐" << std::endl;
        std::cout << "│ Unload │" << std::endl;
        std::cout << "└────────┘" << std::endl;
        rm.unloadAllResources();

        wm.closeWindow();
    }

    /*
    //----- Draw -----//
    std::cout << "┌──────┐" << std::endl;
    std::cout << "│ Draw │" << std::endl;
    std::cout << "└──────┘" << std::endl;
    nScene->traverse(glm::mat4());
    */

    return 0;
}

std::shared_ptr<Node> createSceneTree(){
    //Create scene
    auto nScene = std::make_unique<Node>();
    nScene->name = "Scene";

    // Create Light
    auto nLight = std::make_unique<Node>();
    nLight->name = "Light";
    auto eLight = std::make_shared<Light>();
    nLight->setEntity(eLight);
    nScene->addChild(std::move(nLight));

    // Create Camera
    auto nCamera = std::make_unique<Node>();
    nCamera->name = "Camera";
    auto eCamera = std::make_shared<Camera>();
    nCamera->setEntity(eCamera);
    nScene->addChild(std::move(nCamera));

    return nScene;
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