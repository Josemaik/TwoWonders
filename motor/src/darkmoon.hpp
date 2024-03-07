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
    void ClearBackground(glm::vec4 color);
    // Setup canvas to start drawing
    void BeginDrawing();
    // End canvas drawing and swap buffers
    void EndDrawing();

    
private:
    // Root node 
    std::unique_ptr<Node> m_rootNode;

    // Managers
    InputManager m_inputManager;
    RenderManager m_renderManager;
    WindowsManager m_windowsManager;
    ResourceManager m_resourceManager;
};