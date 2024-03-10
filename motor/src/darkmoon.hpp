#pragma once

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

struct DarkMoonEngine{
public:
    DarkMoonEngine(){};
    ~DarkMoonEngine(){};

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

    // ------------------------- //
    // Texture drawing functions //
    // ------------------------- //

    // Draw a texture
    void DrawTexture(std::shared_ptr<Texture> texture, int posX, int posY, Color tint);
    // Draw a texture (vector version)
    void DrawTextureV(std::shared_ptr<Texture> texture, glm::vec2 pos, Color tint);
    // Draw a texture with extended parameters
    void DrawTextureEx(std::shared_ptr<Texture> texture, glm::vec2 pos, float rotation, float scale, Color tint);

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

    // ----------------------------- //
    // Loading / Unloading functions //
    // ----------------------------- //

    // Load font from file into GPU memory
    std::shared_ptr<Font> LoadFont(const char* filePath);
    // Load texture from file into GPU memory
    std::shared_ptr<Texture> LoadTexture(const char* filePath);
    // Load shader from file into GPU memory
    std::shared_ptr<Shader> LoadShader(const char* vsFilePath, const char* fsFilePath);
    // TODO //
    // Model

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
    RenderManager m_renderManager;
    WindowsManager m_windowsManager;
    ResourceManager m_resourceManager;

    // Shaders
    std::shared_ptr<Shader> m_shaderColor, m_shaderTexture, m_shader3D;
};