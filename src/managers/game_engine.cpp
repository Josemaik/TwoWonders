#include "game_engine.hpp"

ENGI::GameEngine::GameEngine(u16 const width, u16 const height)
    : width_{ width }, height_{ height }
{
    GameEngine::initWindow(width_, height_, "Zelda");
    GameEngine::setTargetFPS(30);

    GameEngine::setPositionCamera({ 0.0f, 25.0f, 25.0f });
    GameEngine::setTargetCamera({ 0.0f, 03.0f, .0f });
    GameEngine::setUpCamera({ 0.0f, 01.0f, 0.0f });
    GameEngine::setFovyCamera(30.0f);
    GameEngine::setProjectionCamera(CAMERA_PERSPECTIVE);
}

////// DRAWING //////

void ENGI::GameEngine::beginDrawing(){
    BeginDrawing();
}

void ENGI::GameEngine::clearBackground(Color color){
    ClearBackground(color);
}

void ENGI::GameEngine::drawGrid(int slices, float spacing){
    DrawGrid(slices, spacing);
}

void ENGI::GameEngine::endDrawing(){
    EndDrawing();
}

void ENGI::GameEngine::beginMode3D(){
    BeginMode3D(camera);
}

void ENGI::GameEngine::endMode3D(){
    EndMode3D();
}

void ENGI::GameEngine::drawCube(Vector3 pos, float width, float height, float lenght, Color color){
    DrawCube(pos, height, width, lenght, color);
}

void ENGI::GameEngine::drawCubeWires(Vector3 pos, float width, float height, float lenght, Color color){
    DrawCubeWires(pos, height, width, lenght, color);
}

////// WINDOW //////

void ENGI::GameEngine::initWindow(int width, int height, const char *title){
    InitWindow(width, height, title);
}

void ENGI::GameEngine::closeWindow(){
    CloseWindow();
}

void ENGI::GameEngine::setTargetFPS(int fps){
    SetTargetFPS(fps);
}

bool ENGI::GameEngine::windowShouldClose(){
    return WindowShouldClose();
}

////// CAMERA //////

void ENGI::GameEngine::setPositionCamera(Vector3 pos){
    camera.position = pos;
}

void ENGI::GameEngine::setTargetCamera(Vector3 tar){
    camera.target = tar;
}

void ENGI::GameEngine::setUpCamera(Vector3 up){
    camera.up = up;
}

void ENGI::GameEngine::setFovyCamera(float fovy){
    camera.fovy = fovy;
}

void ENGI::GameEngine::setProjectionCamera(int proj){
    camera.projection = proj;
}