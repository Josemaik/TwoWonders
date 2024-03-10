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

        //----- Shaders -----//
        std::cout << "┌─────────┐" << std::endl;
        std::cout << "│ Shaders │" << std::endl;
        std::cout << "└─────────┘" << std::endl;
        m_shaderColor = LoadShader("src/shaders/color.vs", "src/shaders/color.fs");
        m_shaderTexture = LoadShader("src/shaders/texture.vs", "src/shaders/texture.fs");
        m_shader3D = LoadShader("src/shaders/3D.vs", "src/shaders/3D.fs");

        //----- Font -----//
        m_renderManager.setDefaultFont(LoadFont("assets/fonts/roboto.ttf"));

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

// Begin 3D mode
void DarkMoonEngine::BeginMode3D(){
    m_renderManager.beginMode3D();
    m_renderManager.useShader(m_shader3D);
}

// Ends 3D mode
void DarkMoonEngine::EndMode3D(){
    m_renderManager.endMode3D();
    m_renderManager.useShader(m_shaderColor);
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

// Draw a triangle
void DarkMoonEngine::DrawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, Color color){
    m_renderManager.drawTriangle(v1, v2, v3, color);
}

// Draw triangle outline
void DarkMoonEngine::DrawTriangleLines(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, Color color){
    m_renderManager.drawLine(v1,v2, color);
    m_renderManager.drawLine(v2,v3, color);
    m_renderManager.drawLine(v3,v1, color);
}

// Draw a color-filled rectangle
void DarkMoonEngine::DrawRectangle(int x, int y, int sizeX, int sizeY, Color color){
    m_renderManager.drawRectangle({x, y}, {sizeX, sizeY}, color);
}

// Draw a color-filled rectangle (vector version)
void DarkMoonEngine::DrawRectangleV(glm::vec2 pos, glm::vec2 size, Color color){
    m_renderManager.drawRectangle(pos, size, color);
}

// Draw rectangle outline
void DarkMoonEngine::DrawRectangleLines(glm::vec2 pos, glm::vec2 size, Color color){
    m_renderManager.drawLine({pos.x, pos.y}, {pos.x + size.x, pos.y}, color);
    m_renderManager.drawLine({pos.x, pos.y}, {pos.x, pos.y + size.y}, color);
    m_renderManager.drawLine({pos.x + size.x, pos.y}, {pos.x + size.x, pos.y + size.y}, color);
    m_renderManager.drawLine({pos.x, pos.y + size.y}, {pos.x + size.x, pos.y + size.y}, color);
}

// Draw a color-filled circle
void DarkMoonEngine::DrawCircle(int centerX, int centerY, float radius, int segments, Color color){
    m_renderManager.drawCircle({centerX, centerY}, radius, segments, color);
}

// Draw a color-filled circle (vector version)
void DarkMoonEngine::DrawCircleV(glm::vec2 pos, float radius, int segments, Color color){
    m_renderManager.drawCircle(pos, radius, segments, color);
}

// ------------------------- //
// Texture drawing functions //
// ------------------------- //

// Draw a texture
void DarkMoonEngine::DrawTexture(std::shared_ptr<Texture> texture, int posX, int posY, Color tint){
    m_renderManager.useShader(m_shaderTexture);
    m_renderManager.drawTexture(texture, {posX, posY}, tint);
    m_renderManager.useShader(m_shaderColor);
}

// Draw a texture (vector version)
void DarkMoonEngine::DrawTextureV(std::shared_ptr<Texture> texture, glm::vec2 pos, Color tint){
    m_renderManager.useShader(m_shaderTexture);
    m_renderManager.drawTexture(texture, pos, tint);
    m_renderManager.useShader(m_shaderColor);
}

// Draw a texture with extended parameters
void DarkMoonEngine::DrawTextureEx(std::shared_ptr<Texture> texture, glm::vec2 pos, float rotation, float scale, Color tint){
    m_renderManager.useShader(m_shaderTexture);
    m_renderManager.drawTextureExtra(texture, pos, rotation, scale, tint);
    m_renderManager.useShader(m_shaderColor);
}

// ------------------------------------------- //
// Basic geometric 3D shapes drawing functions //
// ------------------------------------------- //

// Draw a point in 3D space
void DarkMoonEngine::DrawPoint3D(glm::vec3 pos, Color color){
    m_renderManager.drawPoint3D(pos, 1.0f, color);
}

// Draw a point in 3D space with extended parameters
void DarkMoonEngine::DrawPoint3DExtra(glm::vec3 pos, float pointSize, Color color){
    m_renderManager.drawPoint3D(pos, pointSize, color);
}

// Draw a line in 3D space
void DarkMoonEngine::DrawLine3D(glm::vec3 startPos, glm::vec3 endPos, float lineSize, Color color){
    m_renderManager.drawLine3D(startPos, endPos, lineSize, color);
}

// Draw a grid (centered at (0, 0, 0))
void DarkMoonEngine::DrawGrid(int slices, float spacing, Color color){
    m_renderManager.drawGrid(slices, spacing, color);
}

// Draw a plane XZ
void DarkMoonEngine::DrawPlane(glm::vec3 centerPos, glm::vec2 size, Color color){
    m_renderManager.drawPlane(centerPos, size, color);
}

// Draw a cube
void DarkMoonEngine::DrawCube(glm::vec3 position, float width, float height, float length, Color color){
    m_renderManager.drawCube(position, {width, height, length}, color);
}

// Draw a cube (vector version)
void DarkMoonEngine::DrawCubeV(glm::vec3 position, glm::vec3 size, Color color){
    m_renderManager.drawCube(position, size, color);
}

// Draw a cube wires
void DarkMoonEngine::DrawCubeWires(glm::vec3 position, float width, float height, float length, Color color){
    m_renderManager.drawCubeWires(position, {width, height, length}, color);
}

// Draw a cube wires (vector version)
void DarkMoonEngine::DrawCubeWiresV(glm::vec3 position, glm::vec3 size, Color color){
    m_renderManager.drawCubeWires(position, size, color);
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

// ----------------------------- //
// Loading / Unloading functions //
// ----------------------------- //

// Load font from file into GPU memory
std::shared_ptr<Font> DarkMoonEngine::LoadFont(const char* filePath){
    auto font = m_resourceManager.loadResource<Font>();
    font->load(filePath);

    return font;
}

// Load texture from file into GPU memory
std::shared_ptr<Texture> DarkMoonEngine::LoadTexture(const char* filePath){
    auto texture = m_resourceManager.loadResource<Texture>();
    texture->load(filePath);

    return texture;
}

// Load shader from file into GPU memory
std::shared_ptr<Shader> DarkMoonEngine::LoadShader(const char* vsFilePath, const char* fsFilePath){
    return m_resourceManager.loadResource<Shader>(vsFilePath, fsFilePath, ShaderType::COLOR);
}

// ---------------------- //
// Text drawing functions //
// ---------------------- //

// Draw text (using default font)
void DarkMoonEngine::DrawText(const char* text, int posX, int posY, int fontSize, Color color){
    m_renderManager.drawText(text, {posX, posY}, fontSize, color);
}