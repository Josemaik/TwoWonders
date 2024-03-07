#include "darkmoon.hpp"

// ------------------------ //
// Window-related functions //
// ------------------------ //

// Initialize window and OpenGL context
bool DarkMoonEngine::InitWindow(int width, int height, const char* title){
    m_renderManager.setWindowSize(width, height);

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
        m_shaderColor = m_resourceManager.loadResource<Shader>("src/shaders/color.vs", "src/shaders/color.fs", ShaderType::COLOR);
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

// Set window dimensions
void DarkMoonEngine::SetWindowSize(int width, int height){
    m_renderManager.setWindowSize(width, height);
    m_windowsManager.setWindowSize(width, height);
}

// Get current screen width
int DarkMoonEngine::GetScreenWidth(){
    return m_windowsManager.getScreenWidth();
}

// Get current screen height
int DarkMoonEngine::GetScreenHeight(){
    return m_windowsManager.getScreenHeight();
}


// ------------------------- //
// Drawing-related functions //
// ------------------------- //

// Set background color
void DarkMoonEngine::ClearBackground(Color color){
    m_renderManager.clearBackground(color);
}

// Setup canvas to start drawing
void DarkMoonEngine::BeginDrawing(){
    m_windowsManager.beginDrawing();
    m_renderManager.useShader(m_shaderColor);
}

// End canvas drawing and swap buffers
void DarkMoonEngine::EndDrawing(){
    m_windowsManager.endDrawing();
}

// ------------------------------ //
// Basic shapes drawing functions //
// ------------------------------ //
// Draw a pixel
void DarkMoonEngine::DrawPixel(int posX, int posY, Color color){
    m_renderManager.drawPixel({posX, posY}, color);
}

// Draw a pixel (vector version)
void DarkMoonEngine::DrawPixelV(glm::vec2 pos, Color color){
    m_renderManager.drawPixel(pos, color);
}

// Draw a line
void DarkMoonEngine::DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color){
    m_renderManager.drawLine({startPosX, startPosY}, {endPosX, endPosY}, color);
}

// Draw a line (vector version)
void DarkMoonEngine::DrawLineV(glm::vec2 startPos, glm::vec2 endPos, Color color){
    m_renderManager.drawLine(startPos, endPos, color);
}


// --------------------------------- //
// Input-related functions: keyboard //
// --------------------------------- //

// Check if a key has been pressed once
bool DarkMoonEngine::IsKeyPressed(int key){
    return m_inputManager.isKeyPressed(key);
}

// Check if a key has been pressed again
bool DarkMoonEngine::IsKeyPressedRepeat(int key){
    return m_inputManager.isKeyPressedRepeat(key);
}

// Check if a key is being pressed
bool DarkMoonEngine::IsKeyDown(int key){
    return m_inputManager.isKeyDown(key);
}

// Check if a key has not been released once
bool DarkMoonEngine::IsKeyReleased(int key){
    return m_inputManager.isKeyReleased(key);
}

// Check if a key is not being pressed
bool DarkMoonEngine::IsKeyUp(int key){
    return m_inputManager.isKeyUp(key);
}
