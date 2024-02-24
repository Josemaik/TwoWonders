#include "components/node.hpp"
#include "components/entity.hpp"
#include "components/entity_model.hpp"
#include "managers/resource_manager.hpp"
#include "managers/windows_manager.hpp"
#include "managers/render_manager.hpp"

#include <iostream>

std::shared_ptr<Node> createSceneTree();
std::shared_ptr<Model> loadModel(const char*, std::shared_ptr<Node>, ResourceManager& rm);

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
        auto filePath = "assets/main_character.obj";
        auto eModel = loadModel(filePath, nScene, rm);

        //----- View tree -----//
        std::cout << "┌──────┐" << std::endl;
        std::cout << "│ Tree │" << std::endl;
        std::cout << "└──────┘" << std::endl;
        nScene->drawTree();

        //------ Shaders -----//
        renm.compilingShaders();

        // Main loop
        while(!wm.windowShouldClose()){
            wm.beginDrawing();

            renm.clearBackground({1.0f, 1.0f, 1.0f, 1.0f});
            renm.drawTriangle({0.0f, 0.5f}, {-0.5f, -0.5f}, {0.5f, -0.5f}, {1.0f, 0.5f, 0.2f, 1.0f});

            wm.endDrawing();
        }

        //----- Unload -----//
        std::cout << "┌────────┐" << std::endl;
        std::cout << "│ Unload │" << std::endl;
        std::cout << "└────────┘" << std::endl;
        eModel->unload(rm);

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