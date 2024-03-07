#include "darkmoon.hpp"

// ------------------------ //
// Window-related functions //
// ------------------------ //

// Initialize window and OpenGL context
bool DarkMoonEngine::InitWindow(int width, int height, const char* title){
    m_renderManager.setWidthHeight(width, height);

    //Create scene
    m_rootNode = std::make_unique<Node>();
    m_rootNode->name = "Scene";

    // Create Light
    auto nLight = std::make_unique<Node>();
    nLight->name = "Light";
    auto eLight = std::make_shared<Light>();
    nLight->setEntity(eLight);
    m_rootNode->addChild(std::move(nLight));

    // Create Camera
    auto nCamera = std::make_unique<Node>();
    nCamera->name = "Main camera";
    auto eCamera = std::make_shared<Camera>();
    nCamera->setEntity(eCamera);
    m_rootNode->addChild(std::move(nCamera));
    // Assigns Camera to RenderManager
    m_renderManager.setCamera(eCamera);

    //----- View tree -----//
    std::cout << "┌──────┐" << std::endl;
    std::cout << "│ Tree │" << std::endl;
    std::cout << "└──────┘" << std::endl;
    m_rootNode->drawTree();

    if(m_windowsManager.initWindow(width, height, title)){
        //----- Configure callback -----//
        glfwSetWindowUserPointer(m_windowsManager.getWindow(), &m_inputManager);
        glfwSetKeyCallback(m_windowsManager.getWindow(), InputManager::keyCallback);

        //------ Shaders -----//
        std::cout << "┌─────────┐" << std::endl;
        std::cout << "│ Shaders │" << std::endl;
        std::cout << "└─────────┘" << std::endl;
        auto rShaderColor = m_resourceManager.loadResource<Shader>("src/shaders/color.vs", "src/shaders/color.fs", ShaderType::COLOR);
        auto rShaderTexture = m_resourceManager.loadResource<Shader>("src/shaders/texture.vs", "src/shaders/texture.fs", ShaderType::TEXTURE);
        auto rShaderTexture3D = m_resourceManager.loadResource<Shader>("src/shaders/texture3D.vs", "src/shaders/texture3D.fs", ShaderType::TEXTURE3D);

        return true;
    }


    return false;
}

// Close window and unload OpenGL context
void DarkMoonEngine::CloseWindow(){
    m_windowsManager.closeWindow();

    //----- Unload -----//
    std::cout << "┌────────┐" << std::endl;
    std::cout << "│ Unload │" << std::endl;
    std::cout << "└────────┘" << std::endl;
    m_resourceManager.unloadAllResources();
}

// Check if application should close
bool DarkMoonEngine::WindowShouldClose(){
    return m_windowsManager.windowShouldClose();
}

// ------------------------- //
// Drawing-related functions //
// ------------------------- //

// Set background color
void DarkMoonEngine::ClearBackground(glm::vec4 color){
    m_renderManager.clearBackground(color);
}

// Setup canvas to start drawing
void DarkMoonEngine::BeginDrawing(){
    m_windowsManager.beginDrawing();
}

// End canvas drawing and swap buffers
void DarkMoonEngine::EndDrawing(){
    m_windowsManager.endDrawing();
}
