#include "game_engine.hpp"

ENGI::GameEngine::GameEngine(u16 const width, u16 const height)
    : width_{ width }, height_{ height }
{
    GameEngine::initWindow(width_, height_, "ZeldaWonders");
    GameEngine::setTargetFPS(30);

    GameEngine::setPositionCamera({ 0.0f, 25.0f, 25.0f });
    GameEngine::setTargetCamera({ 0.0f, 03.0f, .0f });
    GameEngine::setUpCamera({ 0.0f, 01.0f, 0.0f });
    GameEngine::setFovyCamera(30.0f);
    GameEngine::setProjectionCamera(CAMERA_PERSPECTIVE);
}

////// DRAWING //////

void ENGI::GameEngine::beginDrawing()
{
    BeginDrawing();
}

void ENGI::GameEngine::clearBackground(Color color)
{
    ClearBackground(color);
}

void ENGI::GameEngine::drawGrid(int slices, float spacing)
{
    DrawGrid(slices, spacing);
}

void ENGI::GameEngine::endDrawing()
{
    EndDrawing();
}

void ENGI::GameEngine::beginMode3D()
{
    BeginMode3D(camera);
}

void ENGI::GameEngine::endMode3D()
{
    EndMode3D();
}

void ENGI::GameEngine::drawCube(vec3f pos, float width, float height, float lenght, Color color)
{
    DrawCube(pos.toRaylib(), width, height, lenght, color);
}

void ENGI::GameEngine::drawCubeWires(vec3f pos, float width, float height, float lenght, Color color)
{
    DrawCubeWires(pos.toRaylib(), width, height, lenght, color);
}

////// WINDOW //////

void ENGI::GameEngine::initWindow(int width, int height, const char* title)
{
    InitWindow(width, height, title);
}

void ENGI::GameEngine::closeWindow()
{
    CloseWindow();
}

void ENGI::GameEngine::setTargetFPS(int fps)
{
    SetTargetFPS(fps);
}

bool ENGI::GameEngine::windowShouldClose()
{
    return WindowShouldClose();
}

int ENGI::GameEngine::getScreenWidth()
{
    return GetScreenWidth();
}

int ENGI::GameEngine::getScreenHeight()
{
    return GetScreenHeight();
}

////// CAMERA //////

void ENGI::GameEngine::setPositionCamera(vec3f pos)
{
    camera.position = pos.toRaylib();
}

void ENGI::GameEngine::setTargetCamera(vec3f tar)
{
    camera.target = tar.toRaylib();
}

void ENGI::GameEngine::setUpCamera(vec3f up)
{
    camera.up = up.toRaylib();
}

void ENGI::GameEngine::setFovyCamera(float fovy)
{
    camera.fovy = fovy;
}

void ENGI::GameEngine::setProjectionCamera(int proj)
{
    camera.projection = proj;
}