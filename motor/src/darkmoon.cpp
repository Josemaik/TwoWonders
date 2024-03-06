#include "darkmoon.hpp"

// ------------------------ //
// Window-related functions //
// ------------------------ //

// Initialize window and OpenGL context
bool DarkMoonEngine::InitWindow(int width, int height, const char* title){
   return m_windowsManager.initWindow(width, height, title);
}

// Close window and unload OpenGL context
void DarkMoonEngine::CloseWindow(){
    m_windowsManager.closeWindow();
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
