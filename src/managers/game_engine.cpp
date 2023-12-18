#include "game_engine.hpp"
#include "utils/gametimer.hpp"
ENGI::GameEngine::GameEngine(u16 const width, u16 const height)
    : width_{ width }, height_{ height }
{
    ENGI::GameEngine::initWindow(width_, height_, "ZeldaWonders");

    ENGI::GameEngine::setPositionCamera({ 0.0f, 30.0f, 12.0f });
    ENGI::GameEngine::setTargetCamera({ 0.0f, 03.0f, .0f });
    ENGI::GameEngine::setUpCamera({ 0.0f, 01.0f, 0.0f });
    ENGI::GameEngine::setFovyCamera(30.0f);
    ENGI::GameEngine::setProjectionCamera(CAMERA_PERSPECTIVE);

    // Logo Two Wonders
    Image logo_two_wonders = ENGI::GameEngine::loadImage("assets/logo_two_wonders.png");
    ENGI::GameEngine::imageResize(&logo_two_wonders, width_ - 20, static_cast<int>(height_/1.76));
    texture_logo_two_wonders = ENGI::GameEngine::loadTextureFromImage(logo_two_wonders);
    ENGI::GameEngine::unloadImage(logo_two_wonders);

    // Logo Kaiwa Games
    Image logo_kaiwa_games = ENGI::GameEngine::loadImage("assets/logo_kaiwa_games.png");
    ENGI::GameEngine::imageResize(&logo_kaiwa_games, width_, static_cast<int>(height_/2)); // 2.49
    texture_logo_kaiwa_games = ENGI::GameEngine::loadTextureFromImage(logo_kaiwa_games);
    ENGI::GameEngine::unloadImage(logo_kaiwa_games);
}

////// IMAGE AND TEXTURE //////

Image ENGI::GameEngine::loadImage(const char *filename){
    return LoadImage(filename);
} 

void ENGI::GameEngine::imageResize(Image* image, int newWidth, int newHeight){
    ImageResize(image, newWidth, newHeight);
}

void ENGI::GameEngine::unloadImage(Image image){
    UnloadImage(image);
}

Texture2D ENGI::GameEngine::loadTextureFromImage(Image image){
    return LoadTextureFromImage(image);
}

////// TIMIING RELATED FUNCTIONS //////

void ENGI::GameEngine::setTargetFPS(int fps) {
    SetTargetFPS(fps);
}

float ENGI::GameEngine::getFrameTime() {
    return GetFrameTime();
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

void ENGI::GameEngine::drawRectangle(int posX, int posY, int width, int height, Color color) {
    DrawRectangle(posX, posY, width, height, color);
}

void ENGI::GameEngine::drawTexture(Texture2D texture, int posX, int posY, Color tint){
    DrawTexture(texture, posX, posY, tint);
}

////// TEXT //////

void ENGI::GameEngine::drawText(const char* text, int posX, int posY, int fontSize, Color color) {
    DrawText(text, posX, posY, fontSize, color);
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

////// AUX //////

float ENGI::GameEngine::getWorldToScreenX(vec3f pos)
{
    return GetWorldToScreen(pos.toRaylib(), camera).x;
}

float ENGI::GameEngine::getWorldToScreenY(vec3f pos)
{
    return GetWorldToScreen(pos.toRaylib(), camera).y;
}