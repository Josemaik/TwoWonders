#include "game_engine.hpp"

ENGI::GameEngine::GameEngine(u16 const width, u16 const height)
    : width_{ width }, height_{ height }
{
    InitWindow(width_, height_, "Zelda");
    SetTargetFPS(30);

    setPositionCamera({ 0.0f, 25.0f, 25.0f });
    setTargetCamera({ 0.0f, 03.0f, .0f });
    setUpCamera({ 0.0f, 01.0f, 0.0f });
    setFovyCamera(30.0f);
    setProjectionCamera(CAMERA_PERSPECTIVE);
}

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

void ENGI::GameEngine::closeWindow(){
    CloseWindow();
}

bool ENGI::GameEngine::windowShouldClose(){
    return WindowShouldClose();
}

void ENGI::GameEngine::beginMode3D(){
    BeginMode3D(camera);
}

void ENGI::GameEngine::endMode3D(){
    EndMode3D();
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