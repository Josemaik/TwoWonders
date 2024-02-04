#include "game_engine.hpp"
ENGI::GameEngine::GameEngine(u16 const width, u16 const height)
    : width_{ width }, height_{ height }
{
    ENGI::GameEngine::initWindow(width_, height_, "Two Wonders");

    ENGI::GameEngine::setUpCamera({ 0.0f, 01.0f, 0.0f });
    ENGI::GameEngine::setFovyCamera(20.0f);
    ENGI::GameEngine::setProjectionCamera(CAMERA_PERSPECTIVE);

    // Logo Two Wonders
    Image logo_two_wonders = ENGI::GameEngine::loadImage("assets/logo_two_wonders.png");
    ENGI::GameEngine::imageResize(&logo_two_wonders, width_ - 20, static_cast<int>(height_ / 1.76));
    texture_logo_two_wonders = ENGI::GameEngine::loadTextureFromImage(logo_two_wonders);
    ENGI::GameEngine::unloadImage(logo_two_wonders);

    // Logo Kaiwa Games
    Image logo_kaiwa_games = ENGI::GameEngine::loadImage("assets/logo_kaiwa_games.png");
    ENGI::GameEngine::imageResize(&logo_kaiwa_games, width_, static_cast<int>(height_ / 2)); // 2.49
    texture_logo_kaiwa_games = ENGI::GameEngine::loadTextureFromImage(logo_kaiwa_games);
    ENGI::GameEngine::unloadImage(logo_kaiwa_games);
}

////// IMAGE AND TEXTURE //////

Image ENGI::GameEngine::loadImage(const char* filename) {
    return LoadImage(filename);
}

void ENGI::GameEngine::imageResize(Image* image, int newWidth, int newHeight) {
    ImageResize(image, newWidth, newHeight);
}

void ENGI::GameEngine::unloadImage(Image image) {
    UnloadImage(image);
}

Texture2D ENGI::GameEngine::loadTextureFromImage(Image image) {
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

void ENGI::GameEngine::drawLine3D(vec3d startPos, vec3d endPos, Color color)
{
    DrawLine3D(startPos.toRaylib(), endPos.toRaylib(), color);
}

void ENGI::GameEngine::drawCube(vec3d pos, float width, float height, float lenght, Color color)
{
    DrawCube(pos.toRaylib(), width, height, lenght, color);
}

void ENGI::GameEngine::drawCubeWires(vec3d pos, float width, float height, float lenght, Color color)
{
    DrawCubeWires(pos.toRaylib(), width, height, lenght, color);
}

void ENGI::GameEngine::drawModel(Model model, vec3d position, vec3d rotationAxis, float rotationAngle, vec3d scale, Color tint)
{
    // Matriz de transformación (incluyendo escalado)
    // Matrix transform = MatrixScale(scale.x, scale.y, scale.z);

    // // Dibuja el modelo con la matriz de transformación
    // DrawModelEx(model, (Vector3) { 0.0f, 0.0f, 0.0f }, 0.0f, transform, WHITE);

    DrawModelEx(model, position.toRaylib(), rotationAxis.toRaylib(), rotationAngle, scale.toRaylib(), tint);
}

void ENGI::GameEngine::drawModelWires(Model model, vec3d position, vec3d rotationAxis, float rotationAngle, vec3d scale, Color tint)
{
    DrawModelWiresEx(model, position.toRaylib(), rotationAxis.toRaylib(), rotationAngle, scale.toRaylib(), tint);
}

void ENGI::GameEngine::drawRectangle(int posX, int posY, int width, int height, Color color) {
    DrawRectangle(posX, posY, width, height, color);
}

void ENGI::GameEngine::drawTexture(Texture2D texture, int posX, int posY, Color tint) {
    DrawTexture(texture, posX, posY, tint);
}

void ENGI::GameEngine::drawCircle(int posX, int posY, float radius, Color color) {
    DrawCircle(posX, posY, radius, color);
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

void ENGI::GameEngine::setPositionCamera(vec3d pos)
{
    camera.position = pos.toRaylib();
}

void ENGI::GameEngine::setTargetCamera(vec3d tar)
{
    camera.target = tar.toRaylib();
}

void ENGI::GameEngine::setUpCamera(vec3d up)
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

vec3d ENGI::GameEngine::getPositionCamera()
{
    return vec3d(camera.position.x, camera.position.y, camera.position.z);
}

vec3d ENGI::GameEngine::getTargetCamera()
{
    return vec3d(camera.target.x, camera.target.y, camera.target.z);
}

vec3d ENGI::GameEngine::getUpCamera()
{
    return vec3d(camera.up.x, camera.up.y, camera.up.z);
}

float ENGI::GameEngine::getFovyCamera()
{
    return camera.fovy;
}

////// AUX //////

Mesh ENGI::GameEngine::genMeshCube(float width, float height, float lenght)
{
    return GenMeshCube(width, height, lenght);
}

Model ENGI::GameEngine::loadModelFromMesh(Mesh m)
{
    return LoadModelFromMesh(m);
}

void ENGI::GameEngine::unloadMesh(Mesh m)
{
    UnloadMesh(m);
}

void ENGI::GameEngine::unloadModel(Model m)
{
    UnloadModel(m);
}

float ENGI::GameEngine::getWorldToScreenX(vec3d pos)
{
    return GetWorldToScreen(pos.toRaylib(), camera).x;
}

float ENGI::GameEngine::getWorldToScreenY(vec3d pos)
{
    return GetWorldToScreen(pos.toRaylib(), camera).y;
}

RayCast ENGI::GameEngine::getMouseRay()
{
    Ray r = GetMouseRay(GetMousePosition(), camera);
    return RayCast{ .origin = vec3d(r.position.x, r.position.y, r.position.z), .direction = vec3d(r.direction.x, r.direction.y, r.direction.z) };
}