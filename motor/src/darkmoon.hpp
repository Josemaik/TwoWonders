#pragma once

#include "components/node.hpp"
#include "components/entity.hpp"
#include "components/entity_model.hpp"
#include "components/entity_camera.hpp"
#include "components/entities2D.hpp"

#include "managers/resource_manager.hpp"
#include "managers/windows_manager.hpp"
#include "managers/render_manager.hpp"
#include "managers/input_manager.hpp"
#include "resources/resource_shader.hpp"

#include "utils/keys.hpp"

struct DarkMoonEngine{
public:
    DarkMoonEngine();
    ~DarkMoonEngine(){};

    // ---------------------- //
    // Node-related functions //
    // ---------------------- //

    // Create node in parentNode
    Node* CreateNode(const char* nodeName, Node* parentNode);
    // Create rectangle
    Node* CreateRectangle(glm::vec2 position, glm::vec2 size, Color color, const char* nodeName, Node* parentNode);
    // Create triangle
    Node* CreateTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, Color color, const char* nodeName, Node* parentNode);
    // Create camera
    Camera* CreateCamera(const char* nodeName, Node* parentNode);
    // Get root node
    Node* GetRootNode();

    // ------------------------ //
    // Window-related functions //
    // ------------------------ //
    
    // Initialize window and OpenGL context
    bool InitWindow(int width, int height, const char* title);
    // Close window and unload OpenGL context
    void CloseWindow();
    // Check if application should close
    bool WindowShouldClose();
    // Set window dimensions
    void SetWindowSize(int width, int height);
    // Get current screen width
    int GetScreenWidth();
    // Get current screen height
    int GetScreenHeight();

    // ------------------------- //
    // Drawing-related functions //
    // ------------------------- //

    // Set background color
    void ClearBackground(Color color);
    // Setup canvas to start drawing
    void BeginDrawing();
    // End canvas drawing and swap buffers
    void EndDrawing();
    // Begin 3D mode
    void BeginMode3D();
    // Ends 3D mode
    void EndMode3D();

    // ------------------------------ //
    // Basic shapes drawing functions //
    // ------------------------------ //

    // Draw a pixel
    void DrawPixel(int posX, int posY, Color color);
    // Draw a pixel (vector version)
    void DrawPixelV(glm::vec2 pos, Color color);
    // Draw a line
    void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);
    // Draw a line (vector version)
    void DrawLineV(glm::vec2 startPos, glm::vec2 endPos, Color color);
    // Draw a color-filled triangle
    void DrawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, Color color);
    // Draw triangle outline
    void DrawTriangleLines(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, Color color);
    // Draw a color-filled rectangle
    void DrawRectangle(int x, int y, int sizeX, int sizeY, Color color);
    // Draw a color-filled rectangle (vector version)
    void DrawRectangleV(glm::vec2 pos, glm::vec2 size, Color color);
    // Draw rectangle outline
    void DrawRectangleLines(glm::vec2 pos, glm::vec2 size, Color color);
    // Draw a color-filled circle
    void DrawCircle(int centerX, int centerY, float radius, int segments, Color color);
    // Draw a color-filled circle (vector version)
    void DrawCircleV(glm::vec2 pos, float radius, int segments, Color color);

    // ------------------------------------- //
    // Texture Loading and Drawing functions //
    // ------------------------------------- //

    // Load texture from file into GPU memory
    Texture* LoadTexture(const char* filePath);
    // Unload texture data from CPU and GPU
    void UnloadTexture(Texture* texture);
    // Draw a texture
    void DrawTexture(Texture* texture, int posX, int posY, Color tint);
    // Draw a texture (vector version)
    void DrawTextureV(Texture* texture, glm::vec2 pos, Color tint);
    // Draw a texture with extended parameters
    void DrawTextureEx(Texture* texture, glm::vec2 pos, float rotation, float scale, Color tint);

    // ------------------------------------------- //
    // Basic geometric 3D shapes drawing functions //
    // ------------------------------------------- //

    // Draw a point in 3D space
    void DrawPoint3D(glm::vec3 pos, Color color);
    // Draw a point in 3D space with extended parameters
    void DrawPoint3DExtra(glm::vec3 pos, float pointSize, Color color);
    // Draw a line in 3D space
    void DrawLine3D(glm::vec3 startPos, glm::vec3 endPos, float lineSize, Color color);
    // Draw a grid (centered at (0, 0, 0))
    void DrawGrid(int slices, float spacing, Color color);
    // Draw a plane XZ
    void DrawPlane(glm::vec3 centerPos, glm::vec2 size, Color color);
    // Draw a cube
    void DrawCube(glm::vec3 position, float width, float height, float length, Color color);
    // Draw a cube (vector version)
    void DrawCubeV(glm::vec3 position, glm::vec3 size, Color color);
    // Draw a cube wires
    void DrawCubeWires(glm::vec3 position, float width, float height, float length, Color color);
    // Draw a cube wires (vector version)
    void DrawCubeWiresV(glm::vec3 position, glm::vec3 size, Color color);

    // -------------------------------------- //
    // Model 3D Loading and Drawing functions //
    // -------------------------------------- //

    // Load model from file into GPU memory
    Model* LoadModel(const char* filePath);
    // Load model from generated mesh ?

    // Unload model data from CPU and GPU
    void UnloadModel(Model* model);
    // Draw a model (with texture if set)
    void DrawModel(Model* model, glm::vec3 position, float scale, Color tint);
    // Draw a model with extended parameters
    void DrawModelExtra(Model* model, glm::vec3 position, float scale, glm::vec3 rotationAxis, float rotationAngle, Color tint);
    // Draw a model wires (with textures if set)
    void DrawModelWires(Model* model, glm::vec3 position, float scale, Color tint);
    // Draw a model wires with extended parameters
    void DrawModelWiresExtra(Model* model, glm::vec3 position, float scale, glm::vec3 rotationAxis, float rotationAngle, Color tint);

    // ---------------------------------------- //
    // Mesh management and generation functions //
    // ---------------------------------------- //

    // TODO
    // Generate cuboid mesh
    Mesh* GenerateMeshCube(float width, float height, float length);
    // Unload mesh data from CPU and GPU
    void UnloadMesh(Mesh* mesh);
    // Draw mesh ?


    // MATERIAL

    // --------------------------------- //
    // Input-related functions: keyboard //
    // --------------------------------- //

    // Check if a key has been pressed once
    bool IsKeyPressed(int key);
    // Check if a key has been pressed again
    bool IsKeyPressedRepeat(int key);
    // Check if a key is being pressed
    bool IsKeyDown(int key);
    // Check if a key has not been released once
    bool IsKeyReleased(int key);
    // Check if a key is not being pressed
    bool IsKeyUp(int key);

    // MANDO

    // ------------------------------ //
    // Input-related functions: mouse //
    // ------------------------------ //

    // Check if a mouse button has been pressed once
    bool IsMouseButtonPressed(int button);
    // Check if a mouse button is being pressed
    bool IsMouseButtonDown(int button);
    // Check if a mouse button has been released once
    bool IsMouseButtonReleased(int button);
    // Check if a mouse button is not being pressed
    bool IsMouseButtonUp(int button);
    // Get mouse position X
    int GetMouseX();
    // Get mouse position Y
    int GetMouseY();
    // Set mouse position XY
    void SetMousePosition(int x, int y);

    // -------------------------------- //
    // Input-related functions: gamepad //
    // -------------------------------- //

    // Check if gamepad is available
    bool IsGamepadAvailable(int gamepad);
    // Get gamepad internal name id
    const char* GetGamepadName(int gamepad);
    // Check is a gamepad button has been pressed once
    bool IsGamepadButtonPressed(int gamepad, int button);
    // Check is a gamepad button is being pressed
    bool IsGamepadButtonDown(int gamepad, int button);
    // Check is a gamepad button has been released once
    bool IsGamepadButtonReleased(int gamepad, int button);
    // Check is a gamepad button is not being pressed
    bool IsGamepadButtonUp(int gamepad, int button);
    // Get gamepad axis count for a gamepad
    int GetGamepadAxisCount(int gamepad);
    // Get axis movement value for a gamepad axis
    float GetGamepadAxisMovement(int gamepad, int axis);

    // ----------------------------- //
    // Loading / Unloading functions //
    // ----------------------------- //

    // Load font from file into GPU memory
    Font* LoadFont(const char* filePath);
    // Load shader from file into GPU memory
    Shader* LoadShader(const char* vsFilePath, const char* fsFilePath);

    // ------ //
    // Camera //
    // ------ //
    
    // Create camera
    // Camera* CreateCamera(const char* name);
    // Use camera
    void UseCamera(Camera* newCamera);

    // ---------------------- //
    // Text drawing functions //
    // ---------------------- //

    // Draw text (using default font)
    // TODO
    void DrawText(const char* text, int posX, int posY, int fontSize, Color color);
    
private:
    // Root node 
    std::unique_ptr<Node> m_rootNode;

    // Managers
    InputManager m_inputManager;
    RenderManager& m_renderManager = RenderManager::getInstance();
    WindowsManager m_windowsManager;
    ResourceManager& m_resourceManager = ResourceManager::getInstance();

    // Shaders
    Shader* m_shaderColor;
    Shader* m_shaderTexture;
    Shader* m_shader3D;
};