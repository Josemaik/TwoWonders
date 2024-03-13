#include "darkmoon.hpp"

DarkMoonEngine::DarkMoonEngine(){
    //Create scene
    m_rootNode = CreateNode("Scene");

    // Create Default Light
    auto nLight = CreateNode("Default Light");
    auto eLight = std::make_unique<Light>();
    nLight->setEntity(std::move(eLight));
    m_rootNode->addChild(std::move(nLight));

    // Create Default Camera
    auto eCamera = CreateCamera("Default Camera");
    // Assigns Camera to RenderManager
    UseCamera(eCamera);
}

// ---------------------- //
// Node-related functions //
// ---------------------- //

// Create node in scene tree
std::unique_ptr<Node> DarkMoonEngine::CreateNode(const char* nodeName){
    auto node = std::make_unique<Node>();
    node->name = nodeName;

    return node;
}

// Get root node
Node* DarkMoonEngine::GetRootNode(){
    return m_rootNode.get();
}

// ------------------------ //
// Window-related functions //
// ------------------------ //

// Initialize window and OpenGL context
bool DarkMoonEngine::InitWindow(int width, int height, const char* title){
    m_renderManager.setWindowSize(width, height);

    if(m_windowsManager.initWindow(width, height, title)){
        //----- Configure callback -----//
        glfwSetWindowUserPointer(m_windowsManager.getWindow(), &m_inputManager);
        glfwSetKeyCallback(m_windowsManager.getWindow(), InputManager::keyCallback);
        glfwSetMouseButtonCallback(m_windowsManager.getWindow(), InputManager::mouseButtonCallback);

        std::cout << "┌──────┐" << std::endl;
        std::cout << "│ Load │" << std::endl;
        std::cout << "└──────┘" << std::endl;
        
        //----- Shaders -----//
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

    m_inputManager.update();
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

// Load texture from file into GPU memory
Texture* DarkMoonEngine::LoadTexture(const char* filePath){
    auto texture = m_resourceManager.loadResource<Texture>();
    texture->load(filePath);

    return texture;
}

// Unload texture data from CPU and GPU
void DarkMoonEngine::UnloadTexture(Texture* texture){
    m_resourceManager.unloadResource(texture->id);
}

// Draw a texture
void DarkMoonEngine::DrawTexture(Texture* texture, int posX, int posY, Color tint){
    m_renderManager.useShader(m_shaderTexture);
    m_renderManager.drawTexture(texture, {posX, posY}, tint);
    m_renderManager.useShader(m_shaderColor);
}

// Draw a texture (vector version)
void DarkMoonEngine::DrawTextureV(Texture* texture, glm::vec2 pos, Color tint){
    m_renderManager.useShader(m_shaderTexture);
    m_renderManager.drawTexture(texture, pos, tint);
    m_renderManager.useShader(m_shaderColor);
}

// Draw a texture with extended parameters
void DarkMoonEngine::DrawTextureEx(Texture* texture, glm::vec2 pos, float rotation, float scale, Color tint){
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

// -------------------------------------- //
// Model 3D Loading and Drawing functions //
// -------------------------------------- //

// Load model from file into GPU memory
Model* DarkMoonEngine::LoadModel(const char* filePath){
    auto nModel = std::make_unique<Node>();
    nModel->name = filePath;
    auto eModel = std::make_unique<Model>();
    auto rawPtr = eModel.get();
    eModel->load(filePath, m_resourceManager);
    if(eModel->isLoaded()){
        nModel->setEntity(std::move(eModel));
        m_rootNode->addChild(std::move(nModel));
    }
    return rawPtr;
}

// Unload model data from CPU and GPU
void DarkMoonEngine::UnloadModel(Model* model){
    model->unload(m_resourceManager);
}

// Draw a model (with texture if set)
void DarkMoonEngine::DrawModel(Model* model, glm::vec3 position, float scale, Color tint){
    m_renderManager.drawModel(model, position, scale, tint);
}

// Draw a model with extended parameters
void DarkMoonEngine::DrawModelExtra(Model* model, glm::vec3 position, float scale, glm::vec3 rotationAxis, float rotationAngle, Color tint){
    m_renderManager.drawModelExtra(model, position, scale, rotationAxis, rotationAngle, tint);
}

// Draw a model wires (with textures if set)
void DarkMoonEngine::DrawModelWires(Model* model, glm::vec3 position, float scale, Color tint){
    m_renderManager.drawModelWires(model, position, scale, tint);
}

// Draw a model wires with extended parameters
void DarkMoonEngine::DrawModelWiresExtra(Model* model, glm::vec3 position, float scale, glm::vec3 rotationAxis, float rotationAngle, Color tint){
    m_renderManager.drawModelWiresExtra(model, position, scale, rotationAxis, rotationAngle, tint);
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

// ------------------------------ //
// Input-related functions: mouse //
// ------------------------------ //

// Check if a mouse button has been pressed once
bool DarkMoonEngine::IsMouseButtonPressed(int button){
    return m_inputManager.isMouseButtonPressed(button);
}

// Check if a mouse button is being pressed
bool DarkMoonEngine::IsMouseButtonDown(int button){
    return m_inputManager.isMouseButtonDown(button);
}

// Check if a mouse button has been released once
bool DarkMoonEngine::IsMouseButtonReleased(int button){
    return m_inputManager.isMouseButtonReleased(button);
}

// Check if a mouse button is mot being pressed
bool DarkMoonEngine::IsMouseButtonUp(int button){
    return m_inputManager.isMouseButtonUp(button);
}

// Get mouse position X
int DarkMoonEngine::GetMouseX(){
    return m_inputManager.getMouseX(m_windowsManager.getWindow());
}

// Get mouse position Y
int DarkMoonEngine::GetMouseY(){
    return m_inputManager.getMouseY(m_windowsManager.getWindow());
}

// Set mouse position XY
void DarkMoonEngine::SetMousePosition(int x, int y){
    m_inputManager.setMousePosition(m_windowsManager.getWindow(), x, y);
}

// -------------------------------- //
// Input-related functions: gamepad //
// -------------------------------- //

// Check if gamepad is available
bool DarkMoonEngine::IsGamepadAvailable(int gamepad){
    return m_inputManager.isGamePadAvailable(gamepad);
}

// Get gamepad internal name id
const char* DarkMoonEngine::GetGamepadName(int gamepad){
    return m_inputManager.getGamePadName(gamepad);
}

// Check is a gamepad button has been pressed once
bool DarkMoonEngine::IsGamepadButtonPressed(int gamepad, int button){
    return m_inputManager.isGamepadButtonPressed(gamepad, button);
}

// Check is a gamepad button is being pressed
bool DarkMoonEngine::IsGamepadButtonDown(int gamepad, int button){
    return m_inputManager.isGamepadButtonDown(gamepad, button);
}

// Check is a gamepad button has been released once
bool DarkMoonEngine::IsGamepadButtonReleased(int gamepad, int button){
    return m_inputManager.isGamepadButtonReleased(gamepad, button);
}

// Check is a gamepad button is not being pressed
bool DarkMoonEngine::IsGamepadButtonUp(int gamepad, int button){
    return m_inputManager.isGamepadButtonUp(gamepad, button);
}

// Get gamepad axis count for a gamepad
int DarkMoonEngine::GetGamepadAxisCount(int gamepad){
    return m_inputManager.getGamepadAxisCount(gamepad);
}

// Get axis movement value for a gamepad axis
float DarkMoonEngine::GetGamepadAxisMovement(int gamepad, int axis){
    return m_inputManager.getGamepadAxisMovement(gamepad, axis);
}


// ----------------------------- //
// Loading / Unloading functions //
// ----------------------------- //

// Load font from file into GPU memory
Font* DarkMoonEngine::LoadFont(const char* filePath){
    auto font = m_resourceManager.loadResource<Font>();
    font->load(filePath);

    return font;
}

// Load shader from file into GPU memory
Shader* DarkMoonEngine::LoadShader(const char* vsFilePath, const char* fsFilePath){
    return m_resourceManager.loadResource<Shader>(vsFilePath, fsFilePath, ShaderType::COLOR);
}

// ------ //
// Camera //
// ------ //

// Create camera
Camera* DarkMoonEngine::CreateCamera(const char* name){
    auto nCamera = CreateNode(name);
    auto eCamera = std::make_unique<Camera>();
    auto rawPtr = eCamera.get();
    nCamera->setEntity(std::move(eCamera));
    m_rootNode->addChild(std::move(nCamera));

    return rawPtr;
}

// Use camera
void DarkMoonEngine::UseCamera(Camera* newCamera){
    // Assigns Camera to RenderManager
    m_renderManager.setCamera(newCamera);
}

// ---------------------- //
// Text drawing functions //
// ---------------------- //

// Draw text (using default font)
void DarkMoonEngine::DrawText(const char* text, int posX, int posY, int fontSize, Color color){
    m_renderManager.drawText(text, {posX, posY}, fontSize, color);
}