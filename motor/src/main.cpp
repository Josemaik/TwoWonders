#include "components/node.hpp"
#include "components/entity.hpp"
#include "components/entity_model.hpp"
#include "components/entity_camera.hpp"
#include "managers/resource_manager.hpp"
#include "managers/windows_manager.hpp"
#include "managers/render_manager.hpp"
#include "managers/input_manager.hpp"
#include "components/resource_shader.hpp"

#include "utils/keys.hpp"

#include <iostream>

std::shared_ptr<Node> createSceneTree(RenderManager&);
std::shared_ptr<Model> loadModel(const char*, std::shared_ptr<Node>, ResourceManager& rm);
std::shared_ptr<Texture> loadTexture(const char*, ResourceManager&);

int main(){

    // Patron Dirty //

    //----- Initialize managers -----//
    WindowsManager wm;
    ResourceManager rm;
    RenderManager& renm = RenderManager::getInstance();
    InputManager im;

    //----- Create scene tree -----//
    auto nScene = createSceneTree(renm);

    if(wm.initWindow(800, 600, "Salty Pixel")){

        //----- Configure callback -----//
        glfwSetWindowUserPointer(wm.getWindow(), &im);
        glfwSetKeyCallback(wm.getWindow(), InputManager::keyCallback);

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

        //----- View tree -----//
        std::cout << "┌──────┐" << std::endl;
        std::cout << "│ Tree │" << std::endl;
        std::cout << "└──────┘" << std::endl;
        nScene->drawTree();

        // Main loop
        while(!wm.windowShouldClose()){

            // Input
            im.update();

            // std::cout << "Pos X: " << renm.m_camera->position.x << std::endl;
            // std::cout << "Pos Y: " << renm.m_camera->position.y << std::endl;
            // std::cout << "Pos Z: " << renm.m_camera->position.z << std::endl;

            if(im.isKeyPressed(KEY_A)){
                renm.m_camera->position.x -= 0.1f;
                renm.m_camera->target.x   -= 0.1f;
            }
            if(im.isKeyPressed(KEY_D)){
                renm.m_camera->position.x += 0.1f;
                renm.m_camera->target.x   += 0.1f;
            }
            if(im.isKeyPressed(KEY_W)){
                renm.m_camera->position.z -= 0.1f;
                renm.m_camera->target.z   -= 0.1f;
            }
            if(im.isKeyPressed(KEY_S)){
                renm.m_camera->position.z += 0.1f;
                renm.m_camera->target.z   += 0.1f;
            }

            if(im.isKeyPressed(KEY_SPACE)){
                renm.m_camera->position.y += 0.1f;
            }

            if(im.isKeyPressed(GLFW_KEY_LEFT_SHIFT)){
                renm.m_camera->position.y -= 0.1f;
            }

            // Input Tests
            // std::cout << "A: " << im.isKeyPressed(KEY_A) << std::endl;
            // std::cout << "S: " << im.isKeyDown(wm.getWindow(), KEY_S) << std::endl;
            // std::cout << "D: " << im.isKeyReleased(KEY_D) << std::endl;
            // std::cout << "W: " << im.isKeyUp(wm.getWindow(), KEY_W) << std::endl;
            // std::cout << "----" << std::endl;
            // std::cout << "A: " << im.isGamepadButtonPressed(0, GLFW_GAMEPAD_BUTTON_A) << std::endl;
            // std::cout << "B: " << im.isGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_B) << std::endl;
            // std::cout << "X: " << im.isGamepadButtonReleased(0, GLFW_GAMEPAD_BUTTON_X) << std::endl;
            // std::cout << "Y: " << im.isGamepadButtonUp(0, GLFW_GAMEPAD_BUTTON_Y) << std::endl;
            // std::cout << "----" << std::endl;

            // if(im.isGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_A))
            //     std::cout << "Estoy pulsando A" << std::endl;

            // if(im.isGamepadButtonUp(0, GLFW_GAMEPAD_BUTTON_B))
            //     std::cout << "No estoy pulsando B" << std::endl;

            wm.beginDrawing();

            renm.clearBackground({1.0f, 1.0f, 1.0f, 1.0f});

            //renm.beginMode3D();


            // Draw (texture) -> 3D
            renm.useShader(rShaderTexture3D);
            // renm.drawTexture3D(rTexture2, {0.0f, 0.0f}, 50.0f, 0.01f, {1.0f, 1.0f, 1.0f, 1.0f});
            // Draw (model)
            // eModel->draw(glm::mat4());

            renm.drawPlane({0.0f, 0.0f, 0.0f}, {4.0f, 4.0f}, {1.0f, 0.5f, 0.5f, 1.0f});
            renm.drawGrid(10, 1.0f, {0.5f, 0.5f, 0.5f, 0.0f});

            //renm.endMode3D();

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

std::shared_ptr<Node> createSceneTree(RenderManager& renm){
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
    nCamera->name = "Main camera";
    auto eCamera = std::make_shared<Camera>();
    nCamera->setEntity(eCamera);
    nScene->addChild(std::move(nCamera));
    // Assigns Camera to RenderManager
    renm.setCamera(eCamera);

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