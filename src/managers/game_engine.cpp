#include "game_engine.hpp"
ENGI::GameEngine::GameEngine(u16 const width, u16 const height)
    : width_{ width }, height_{ height }
{
    ENGI::GameEngine::initWindow(width_, height_, "Two Wonders");

    ENGI::GameEngine::setUpCamera({ 0.0f, 01.0f, 0.0f });
    ENGI::GameEngine::setProjectionCamera(CAMERA_ORTHOGRAPHIC);

    // Logo Two Wonders
    loadAndResizeImage("logo_twowonders", "assets/logo_two_wonders.png");

    // Logo Kaiwa Games
    loadAndResizeImage("logo_kaiwagames", "assets/logo_kaiwa_games.png");

    // Corazones HUD
    loadAndResizeImage("heart", "assets/HUD/corazon.png");

    // Corazones vacíos HUD
    loadAndResizeImage("empty_heart", "assets/HUD/corazon_vacio.png");

    // Mago Happy HUD
    loadAndResizeImage("mago_happy", "assets/HUD/mago_happy.png");

    // Barra de maná HUD
    loadAndResizeImage("mana", "assets/HUD/mana_bar.png");

    // Destellos HUD
    loadAndResizeImage("destellos", "assets/HUD/destellos.png");

    SetExitKey(KEY_F8);
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

void ENGI::GameEngine::beginDrawing() {
    BeginDrawing();
}

void ENGI::GameEngine::clearBackground(Color color) {
    ClearBackground(color);
}

void ENGI::GameEngine::drawGrid(int slices, float spacing) {
    DrawGrid(slices, spacing);
}

void ENGI::GameEngine::endDrawing() {
    EndDrawing();
}

void ENGI::GameEngine::beginMode3D() {
    BeginMode3D(camera);
}

void ENGI::GameEngine::endMode3D() {
    EndMode3D();
}

void ENGI::GameEngine::drawLine3D(vec3d startPos, vec3d endPos, Color color) {
    DrawLine3D(startPos.toRaylib(), endPos.toRaylib(), color);
}

void ENGI::GameEngine::drawCube(vec3d pos, float width, float height, float lenght, Color color) {
    DrawCube(pos.toRaylib(), width, height, lenght, color);
}

void ENGI::GameEngine::drawCubeWires(vec3d pos, float width, float height, float lenght, Color color) {
    DrawCubeWires(pos.toRaylib(), width, height, lenght, color);
}

void ENGI::GameEngine::drawModel(Model model, vec3d position, vec3d rotationAxis, float rotationAngle, vec3d scale, Color tint) {
    DrawModelEx(model, position.toRaylib(), rotationAxis.toRaylib(), rotationAngle, scale.toRaylib(), tint);
}

void ENGI::GameEngine::drawModelWires(Model model, vec3d position, vec3d rotationAxis, float rotationAngle, vec3d scale, Color tint) {
    DrawModelWiresEx(model, position.toRaylib(), rotationAxis.toRaylib(), rotationAngle, scale.toRaylib(), tint);
}

void ENGI::GameEngine::drawRectangle(int posX, int posY, int width, int height, Color color) {
    DrawRectangle(posX, posY, width, height, color);
}

void ENGI::GameEngine::drawRectangleLinesEx(Rectangle rec, float lineThick, Color color) {
    DrawRectangleLinesEx(rec, lineThick, color);
}

void ENGI::GameEngine::drawRectangleRec(Rectangle rec, Color color) {
    DrawRectangleRec(rec, color);
}

void ENGI::GameEngine::drawTexture(Texture2D texture, int posX, int posY, Color tint) {
    DrawTexture(texture, posX, posY, tint);
}

void ENGI::GameEngine::drawCircle(int posX, int posY, float radius, Color color) {
    DrawCircle(posX, posY, radius, color);
}

void ENGI::GameEngine::drawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color) {
    DrawLine(startPosX, startPosY, endPosX, endPosY, color);
}

////// TEXT //////

void ENGI::GameEngine::drawText(const char* text, int posX, int posY, int fontSize, Color color) {
    DrawText(text, posX, posY, fontSize, color);
}

void ENGI::GameEngine::drawTextEx(Font font, const char* text, Vector2 position, float fontSize, float spacing, Color tint) {
    DrawTextEx(font, text, position, fontSize, spacing, tint);
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

void ENGI::GameEngine::setWindowSize(int width, int height)
{
    if (IsWindowFullscreen())
        ToggleFullscreen();
    SetWindowSize(width, height);
}

void ENGI::GameEngine::setWindowFullScreen()
{
    ToggleFullscreen();
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

////// INPUT HANDLING //////

bool ENGI::GameEngine::isKeyPressed(int key)
{
    return IsKeyPressed(key);
}

bool ENGI::GameEngine::isKeyDown(int key)
{
    return IsKeyDown(key);
}

bool ENGI::GameEngine::isKeyReleased(int key)
{
    return IsKeyReleased(key);
}

bool ENGI::GameEngine::isMouseButtonPressed(int button)
{
    return IsMouseButtonPressed(button);
}

bool ENGI::GameEngine::isMouseButtonDown(int button)
{
    return IsMouseButtonDown(button);
}

bool ENGI::GameEngine::isGamepadAvailable(int gamepad)
{
    return IsGamepadAvailable(gamepad);
}

bool ENGI::GameEngine::isGamepadButtonPressed(int gamepad, int button)
{
    return IsGamepadButtonPressed(gamepad, button);
}

bool ENGI::GameEngine::isGamepadButtonDown(int gamepad, int button)
{
    return IsGamepadButtonDown(gamepad, button);
}

bool ENGI::GameEngine::isGamepadButtonReleased(int gamepad, int button)
{
    return IsGamepadButtonReleased(gamepad, button);
}

float ENGI::GameEngine::getGamepadAxisMovement(int gamepad, int axis)
{
    return GetGamepadAxisMovement(gamepad, axis);
}

////// MOUSE COLLISION //////

bool ENGI::GameEngine::checkCollisionPointRec(Vector2 point, Rectangle rec)
{
    return CheckCollisionPointRec(point, rec);
}

////// SHADERS //////

Shader ENGI::GameEngine::loadShader(const char* vsFileName, const char* fsFileName)
{
    return LoadShader(vsFileName, fsFileName);
}

void ENGI::GameEngine::unloadShader(Shader s)
{
    UnloadShader(s);
}

int ENGI::GameEngine::getShaderLocation(Shader s, const char* uniformName)
{
    return GetShaderLocation(s, uniformName);
}

void ENGI::GameEngine::setShaderValue(Shader s, int uniformLoc, const void* value, int uniformType)
{
    SetShaderValue(s, uniformLoc, value, uniformType);
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

Model ENGI::GameEngine::loadModel(const char* filename)
{
    return LoadModel(filename);
}

Texture2D ENGI::GameEngine::loadTexture(const char* filename)
{
    return LoadTexture(filename);
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

void ENGI::GameEngine::loadAndResizeImage(const std::string& name, const std::string& path) {
    Image image = ENGI::GameEngine::loadImage(path.c_str());
    ENGI::GameEngine::imageResize(&image, static_cast<int>(image.width / 1.3), static_cast<int>(image.height / 1.3));
    textures[name] = ENGI::GameEngine::loadTextureFromImage(image);
    ENGI::GameEngine::unloadImage(image);
}