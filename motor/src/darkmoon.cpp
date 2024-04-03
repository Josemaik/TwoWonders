#include "darkmoon.hpp"

DarkMoonEngine::DarkMoonEngine(){
    //Create scene
    m_rootNode = std::make_unique<Node>();
    m_rootNode->name = "Scene";

    // Create Default Light
    CreateLight({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, "Default Light", GetRootNode());

    // Create Default Camera
    auto eCamera = CreateCamera("Default Camera", GetRootNode());

    // Assigns Camera to RenderManager
    UseCamera(eCamera);
}

// ---------------------- //
// Node-related functions //
// ---------------------- //

// Create node in parentNode
Node* DarkMoonEngine::CreateNode(const char* nodeName, Node* parentNode){
    auto node = std::make_unique<Node>();
    node->name = nodeName;
    auto p_node = node.get();
    parentNode->addChild(std::move(node));

    return p_node;
}

// 2D

// Create pixel in node
Node* DarkMoonEngine::CreatePixel(glm::vec2 position, Color color, const char* nodeName, Node* parentNode){
    auto p_nodePix = CreateNode(nodeName, parentNode);

    // Create Pixel
    auto pixel = std::make_unique<Pixel>(color);
    p_nodePix->translate({position.x, position.y, 0.0f});
    p_nodePix->setEntity(std::move(pixel));

    return p_nodePix;
}

// Create line in node
Node* DarkMoonEngine::CreateLine(glm::vec2 startPos, glm::vec2 endPos, Color color, const char* nodeName, Node* parentNode){
    auto p_nodeLin = CreateNode(nodeName, parentNode);

    // Create Line
    auto line = std::make_unique<Line>(startPos, endPos, color);
    p_nodeLin->setEntity(std::move(line));

    return p_nodeLin;
}

// Create rectangle in node
Node* DarkMoonEngine::CreateRectangle(glm::vec2 position, glm::vec2 size, Color color, const char* nodeName, Node* parentNode){
    auto p_nodeRec = CreateNode(nodeName, parentNode);

    // Create Rectangle
    auto rectangle = std::make_unique<Rectangle>(position, size, color);
    p_nodeRec->translate({position.x, position.y, 0.0f});
    p_nodeRec->setEntity(std::move(rectangle));

    return p_nodeRec;
}

// Create triangle in node
Node* DarkMoonEngine::CreateTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, Color color, const char* nodeName, Node* parentNode){
    auto p_nodeTri = CreateNode(nodeName, parentNode);

    // Create Triangle
    auto triangle = std::make_unique<Triangle>(v1, v2, v3, color);
    p_nodeTri->setEntity(std::move(triangle));

    return p_nodeTri;
}

// Create circle in node
Node* DarkMoonEngine::CreateCircle(glm::vec2 position, float radius, int segments, Color color, const char* nodeName, Node* parentNode){
    auto p_nodeCir = CreateNode(nodeName, parentNode);

    // Create circle
    auto circle = std::make_unique<Circle>(position, radius, segments, color);
    p_nodeCir->translate({position.x, position.y, 0.0f});
    p_nodeCir->setEntity(std::move(circle));

    return p_nodeCir;
}

// Create texture 2D in node
Node* DarkMoonEngine::CreateTexture2D(glm::vec2 position, const char* filePath, Color color, const char* nodeName, Node* parentNode){
    auto p_nodeTex = CreateNode(nodeName, parentNode);

    // Load texture
    auto texture = LoadTexture(filePath);

    // Create texture2D
    auto text2D = std::make_unique<Texture2D>(position, texture, color);
    p_nodeTex->translate({position.x, position.y, 0.0f});
    p_nodeTex->setEntity(std::move(text2D));

    return p_nodeTex;
}

// 3D

// Create point 3D in node
Node* DarkMoonEngine::CreatePoint3D(glm::vec3 position, float pointSize, Color color, const char* nodeName, Node* parentNode){
    auto p_nodePoint3D = CreateNode(nodeName, parentNode);

    // Create point 3D
    auto point3D = std::make_unique<Point3D>(position, pointSize, color);
    p_nodePoint3D->setEntity(std::move(point3D));

    return p_nodePoint3D;
}

// Create line 3D in node
Node* DarkMoonEngine::CreateLine3D(glm::vec3 startPos, glm::vec3 endPos, float lineSize, Color color, const char* nodeName, Node* parentNode){
    auto p_nodeLine3D = CreateNode(nodeName, parentNode);

    // Create line 3D
    auto line3D = std::make_unique<Line3D>(startPos, endPos, lineSize, color);
    p_nodeLine3D->setEntity(std::move(line3D));

    return p_nodeLine3D;
}

// Create grid 3D in node
Node* DarkMoonEngine::CreateGrid(int slices, float spacing, Color color, const char* nodeName, Node* parentNode){
    auto p_nodeGrid = CreateNode(nodeName, parentNode);

    // Create grid 3D
    auto grid3D = std::make_unique<Grid>(slices, spacing, color);
    p_nodeGrid->setEntity(std::move(grid3D));

    return p_nodeGrid;
}

// Create plane 3D (XZ) in node
Node* DarkMoonEngine::CreatePlane(glm::vec3 centerPos, glm::vec2 size, Color color, const char* nodeName, Node* parentNode){
    auto p_nodePlane = CreateNode(nodeName, parentNode);

    // Create plane XZ 3D
    auto plane3D = std::make_unique<Plane>(centerPos, size, color);
    p_nodePlane->setEntity(std::move(plane3D));

    return p_nodePlane;
}

// Create cube in node
Node* DarkMoonEngine::CreateCube(glm::vec3 position, glm::vec3 size, Color color, const char* nodeName, Node* parentNode){
    auto p_nodeCube = CreateNode(nodeName, parentNode);

    // Create cube
    auto cube3D = std::make_unique<Cube>(position, size, color);
    p_nodeCube->translate(position);
    p_nodeCube->setEntity(std::move(cube3D));

    return p_nodeCube;
}

// Create cube wires in node
Node* DarkMoonEngine::CreateCubeWires(glm::vec3 position, glm::vec3 size, Color color, const char* nodeName, Node* parentNode){
    auto p_nodeCubeWires = CreateNode(nodeName, parentNode);

    // Create cube wires
    auto cubeWires3D = std::make_unique<CubeWires>(position, size, color);
    p_nodeCubeWires->translate(position);
    p_nodeCubeWires->setEntity(std::move(cubeWires3D));

    return p_nodeCubeWires;
}

// Create model in node
Node* DarkMoonEngine::CreateModel(const char* , Color , const char* nodeName, Node* parentNode){
    auto p_nodeModel = CreateNode(nodeName, parentNode);

    // Load model
    //auto model = std::make_unique<Model>();
    //model->load(filePath, m_resourceManager);
    //model->setColor(tint);
//
    //p_nodeModel->setEntity(std::move(model));

    return p_nodeModel;
}

// EXTRA

// Create camera in node
Camera* DarkMoonEngine::CreateCamera(const char* nodeName, Node* parentNode){
    auto p_nodeCam = CreateNode(nodeName, parentNode);

    // Create camera
    auto eCamera = std::make_unique<Camera>();
    auto p_eCamera = eCamera.get();
    p_nodeCam->setEntity(std::move(eCamera));

    return p_eCamera;
}

// Create light in node
Node* DarkMoonEngine::CreateLight(glm::vec3, glm::vec3, glm::vec3, glm::vec3, const char* nodeName, Node* parentNode){
    auto p_nodeLight = CreateNode(nodeName, parentNode);

    // Create light
    auto light = std::make_unique<Light>();
    
    return p_nodeLight;
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

        //----- Default Material -----//
        m_renderManager.defaultMaterial = m_resourceManager.loadResource<Material>("defaultMaterial",
                                                                                   glm::vec3(0.2f, 0.2f, 0.2f), 
                                                                                   glm::vec3(0.8f, 0.8f, 0.8f), 
                                                                                   glm::vec3(0.0f, 0.0f, 0.0f), 
                                                                                   0.0f);
        m_renderManager.defaultMaterial->texture = LoadTexture("assets/defaultTexture.png");

        //----- Shaders -----// (std::string(vsFilePath) + " - " + std::string(fsFilePath)).c_str()
        m_renderManager.shaderColor = LoadShader("shaderColor", "src/shaders/color.vs", "src/shaders/color.fs");
        m_renderManager.shaderTexture = LoadShader("shaderTexture", "src/shaders/texture.vs", "src/shaders/texture.fs");
        m_renderManager.shader3D = LoadShader("shader3D", "src/shaders/3D.vs", "src/shaders/3D.fs");

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
    m_renderManager.useShader(m_renderManager.shaderColor);

    m_inputManager.update();
}

// End canvas drawing and swap buffers
void DarkMoonEngine::EndDrawing(){
    m_windowsManager.endDrawing();
}

// ------------------------- //
// Texture loading functions //
// ------------------------- //

// Load texture from file into GPU memory
Texture* DarkMoonEngine::LoadTexture(const char* filePath){
    auto texture = m_resourceManager.loadResource<Texture>(filePath);
    //texture->load(filePath);

    return texture;
}

// Unload texture data from CPU and GPU
void DarkMoonEngine::UnloadTexture(Texture* texture){
    m_resourceManager.unloadResource(texture->getID());
}

// -------------------------------------- //
// Model 3D Loading and Drawing functions //
// -------------------------------------- //

// Unload model data from CPU and GPU
void DarkMoonEngine::UnloadModel(Model*){
    //model->unload(m_resourceManager);
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
    auto font = m_resourceManager.loadResource<Font>(filePath);

    return font;
}

// Load shader from file into GPU memory
Shader* DarkMoonEngine::LoadShader(const char* idShader, const char* vsFilePath, const char* fsFilePath){
    return m_resourceManager.loadResource<Shader>(idShader, vsFilePath, fsFilePath);
}

// Unload shader from CPU and GPU
void DarkMoonEngine::UnloadShader(Shader* shader){
    m_resourceManager.unloadResource(shader->getID());
}

// Load material from file into GPU memory
Material* DarkMoonEngine::LoadMaterial(const char* filePath){
    return m_resourceManager.loadResource<Material>(filePath);
}

// Unload material from CPU and GPU
void DarkMoonEngine::UnloadMaterial(Material* material){
    m_resourceManager.unloadResource(material->getID());
}


// ---------------------- //
// Text drawing functions //
// ---------------------- //

// Draw text (using default font)
void DarkMoonEngine::DrawText(const char* text, int posX, int posY, int fontSize, Color color){
    m_renderManager.drawText(text, {posX, posY}, fontSize, color);
}

// ------------------------ //
// Timing-related functions //
// ------------------------ //

// Set target FPS (max)
void DarkMoonEngine::SetTargetFPS(int fps){
    m_windowsManager.setTargetFPS(fps);
}

// Get time in seconds for last frame drawn
float DarkMoonEngine::GetFrameTime(){
    return m_windowsManager.getFrameTime();
}

// Get elapsed time in seconds
double DarkMoonEngine::GetTime(){
    return m_windowsManager.getTime();
}

// Get current FPS
int DarkMoonEngine::GetFPS(){
    return m_windowsManager.getFPS();
}
