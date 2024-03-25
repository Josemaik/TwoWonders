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

    // Corazones Hielo HUD
    loadAndResizeImage("ice_heart", "assets/HUD/corazon_escudo.png");

    // Mago Happy HUD
    loadAndResizeImage("mago_happy", "assets/HUD/mago_happy.png");

    // Mago Meh HUD
    loadAndResizeImage("mago_meh", "assets/HUD/mago_meh.png");

    // Mago SOS HUD
    loadAndResizeImage("mago_sos", "assets/HUD/mago_sos.png");

    // Barra de maná HUD
    loadAndResizeImage("mana", "assets/HUD/mana_bar.png");

    // Destellos HUD
    loadAndResizeImage("destellos", "assets/HUD/destellos.png");

    // Destello fijado HUD
    loadAndResizeImage("destellin", "assets/HUD/fijado_destellin.png");

    // Candado abierto HUD
    loadAndResizeImage("candado_abierto", "assets/HUD/candado_abierto.png");

    // Candado cerrado HUD
    loadAndResizeImage("candado_cerrado", "assets/HUD/candado_cerrado.png");

    // Diálogo Siguiente HUD
    loadAndResizeImage("sig", "assets/HUD/dialog_siguiente.png");

    // Espacio para hechizos de agua HUD
    loadAndResizeImage("agua_holder", "assets/HUD/item_agua.png");

    // Icono para las pompas de agua HUD
    loadAndResizeImage("pompas", "assets/HUD/pompas.png", 1.5, 1.5);

    // NÚMEROS
    //
    loadAndResizeImage("0", "assets/HUD/numeros/0.png");
    loadAndResizeImage("1", "assets/HUD/numeros/1.png");
    loadAndResizeImage("2", "assets/HUD/numeros/2.png");
    loadAndResizeImage("3", "assets/HUD/numeros/3.png");
    loadAndResizeImage("4", "assets/HUD/numeros/4.png");
    loadAndResizeImage("5", "assets/HUD/numeros/5.png");
    loadAndResizeImage("6", "assets/HUD/numeros/6.png");
    loadAndResizeImage("7", "assets/HUD/numeros/7.png");
    loadAndResizeImage("8", "assets/HUD/numeros/8.png");
    loadAndResizeImage("9", "assets/HUD/numeros/9.png");

    // GIFS
    //
    // Joystick Izquierdo
    loadAndResizeImageGif("joystick_izq", "assets/HUD/botones/joystickL.gif", 0, 35);

    // Fijador de cámara
    loadAndResizeImageGif("fijado", "assets/HUD/gifs/fijado_trama.gif", 0, 35, 1.2, 1.2);

    // Botón X
    loadAndResizeImageGif("x", "assets/HUD/botones/x.gif", 0, 55);

    // Botón Círculo
    loadAndResizeImageGif("circulo", "assets/HUD/botones/circulo.gif", 0);

    // Botón Triángulo
    loadAndResizeImageGif("triangulo", "assets/HUD/botones/triangulo.gif", 0);

    // Botón Cuadrado
    loadAndResizeImageGif("cuadrado", "assets/HUD/botones/cuadrado.gif", 0, 55);

    // Botón Menú
    loadAndResizeImageGif("menu", "assets/HUD/botones/menu.gif", 0);

    // Tecla E
    loadAndResizeImageGif("e", "assets/HUD/teclas/e.gif", 0, 55);

    // Tecla Espacio
    loadAndResizeImageGif("espacio", "assets/HUD/teclas/espacio.gif", 0, 55);

    // Pantalla de carga
    loadAndResizeImageGif("carga", "assets/HUD/gifs/carga_elementos.gif", 0, 55);

    SetExitKey(KEY_F8);
}

////// IMAGE AND TEXTURE //////

Image ENGI::GameEngine::loadImage(const char* filename) {
    return LoadImage(filename);
}

Image ENGI::GameEngine::loadImagenAnim(const char* filename, int& frames) {
    return LoadImageAnim(filename, &frames);
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

void ENGI::GameEngine::drawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color) {
    DrawLine(startPosX, startPosY, endPosX, endPosY, color);
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

void ENGI::GameEngine::drawCircleSector(vec2d center, float radius, float startAngle, float endAngle, int segments, Color color) {
    DrawCircleSector(center.toRaylib(), radius, startAngle, endAngle, segments, color);
}

void ENGI::GameEngine::drawTriangle(vec2d v1, vec2d v2, vec2d v3, Color color) {
    DrawTriangle(v1.toRaylib(), v2.toRaylib(), v3.toRaylib(), color);
}

////// TEXT //////

void ENGI::GameEngine::drawText(const char* text, int posX, int posY, int fontSize, Color color) {
    DrawText(text, posX, posY, fontSize, color);
}

void ENGI::GameEngine::drawTextEx(Font font, const char* text, vec2d position, float fontSize, float spacing, Color tint) {
    DrawTextEx(font, text, position.toRaylib(), fontSize, spacing, tint);
}

vec2d ENGI::GameEngine::measureTextEx(Font font, const char* text, float fontSize, float spacing) {
    Vector2 v = MeasureTextEx(font, text, fontSize, spacing);
    return vec2d(v.x, v.y);
}

Font ENGI::GameEngine::getFontDefault() {
    return GetFontDefault();
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
    if (!replayMode)
    {
        if (IsKeyPressed(key))
        {
            switch (key)
            {
            case KEY_E:
                gameData->addInputEvent(InputEvent::Type::InteractKeyPressed);
                break;
            default:
                break;
            }
            return true;
        }
        return false;
    }
    else
        return gameData->isKeyPressed(key);
}

bool ENGI::GameEngine::isKeyDown(int key)
{
    if (!replayMode)
    {
        if (IsKeyDown(key))
        {
            switch (key)
            {
            case KEY_SPACE:
                gameData->addInputEvent(InputEvent::Type::AttackKeyDown);
                break;
            default:
                break;
            }
            return true;
        }
        return false;
    }
    else
        return gameData->isKeyDown(key);
}

bool ENGI::GameEngine::isKeyReleased(int key)
{
    if (!replayMode)
    {
        if (IsKeyReleased(key))
        {
            switch (key)
            {
            case KEY_ENTER:
                gameData->addInputEvent(InputEvent::Type::EnterReleased);
                break;
            case KEY_ESCAPE:
                gameData->addInputEvent(InputEvent::Type::EscapeReleased);
                break;
            case KEY_I:
                gameData->addInputEvent(InputEvent::Type::InventoryReleased);
                break;
            case KEY_F:
                gameData->addInputEvent(InputEvent::Type::LockInReleased);
                break;
            case KEY_Q:
                gameData->addInputEvent(InputEvent::Type::ChangeSpellReleased);
                break;
            case KEY_E:
                gameData->addInputEvent(InputEvent::Type::InteractKeyReleased);
                break;
            default:
                break;
            }
            return true;
        }
        return false;
    }
    else
        return gameData->isKeyReleased(key);
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
    if (!replayMode)
    {
        if (IsGamepadButtonPressed(gamepad, button))
        {
            switch (button)
            {
            case GAMEPAD_BUTTON_RIGHT_FACE_DOWN:
                gameData->addInputEvent(InputEvent::Type::InteractKeyPressed);
                break;
            case GAMEPAD_BUTTON_RIGHT_FACE_LEFT:
                gameData->addInputEvent(InputEvent::Type::AttackKeyDown);
                break;
            default:
                break;
            }
            return true;
        }
        return false;
    }
    else
    {
        switch (button)
        {
        case GAMEPAD_BUTTON_RIGHT_FACE_DOWN:
            return gameData->isKeyPressed(KEY_E);
            break;
        case GAMEPAD_BUTTON_RIGHT_FACE_LEFT:
            return gameData->isKeyDown(KEY_SPACE);
            break;
        default:
            return false;
        }
    }
}

bool ENGI::GameEngine::isGamepadButtonDown(int gamepad, int button)
{
    return IsGamepadButtonDown(gamepad, button);
}

bool ENGI::GameEngine::isGamepadButtonReleased(int gamepad, int button)
{
    if (!replayMode)
    {
        if (IsGamepadButtonReleased(gamepad, button))
        {
            switch (button)
            {
            case GAMEPAD_BUTTON_MIDDLE_LEFT:
                gameData->addInputEvent(InputEvent::Type::InventoryReleased);
                break;
            case GAMEPAD_BUTTON_RIGHT_FACE_RIGHT:
                gameData->addInputEvent(InputEvent::Type::LockInReleased);
                break;
            case GAMEPAD_BUTTON_RIGHT_FACE_DOWN:
                gameData->addInputEvent(InputEvent::Type::InteractKeyReleased);
                break;
            default:
                break;
            }
            return true;
        }
        return false;
    }
    else
    {
        switch (button)
        {
        case GAMEPAD_BUTTON_MIDDLE_LEFT:
            return gameData->isKeyReleased(KEY_I);
            break;
        case GAMEPAD_BUTTON_RIGHT_FACE_RIGHT:
            return gameData->isKeyReleased(KEY_F);
            break;
        case GAMEPAD_BUTTON_RIGHT_FACE_DOWN:
            return gameData->isKeyReleased(KEY_E);
            break;
        default:
            return false;
        }
    }
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

void ENGI::GameEngine::updateTexture(Texture2D texture, const void* data)
{
    UpdateTexture(texture, data);
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

void ENGI::GameEngine::loadAndResizeImage(const std::string& name, const std::string& path, double reScaleX, double reScaleY) {
    Image image = loadImage(path.c_str());
    imageResize(&image, static_cast<int>(image.width / reScaleX), static_cast<int>(image.height / reScaleY));
    textures[name] = loadTextureFromImage(image);
    unloadImage(image);
}

void ENGI::GameEngine::loadAndResizeImageGif(const std::string& name, const std::string& path, int frames, int delay, double reScaleX, double reScaleY) {
    Gif anim;
    anim.name = name;
    anim.frames = frames;
    anim.frameDelay = delay;
    anim.reScaleX = reScaleX;
    anim.reScaleY = reScaleY;
    anim.image = loadImagenAnim(path.c_str(), anim.frames);
    // imageResize(&image, static_cast<int>(image.width / 1.3), static_cast<int>(image.height / 1.3));
    anim.texture = loadTextureFromImage(anim.image);

    gifs[name] = anim;
}

void ENGI::GameEngine::updateGif(Gif& anim) {
    anim.frameCounter += 1;
    if (anim.frameCounter >= anim.frameDelay)
    {
        anim.frameCounter = 0;
        anim.currentFrame++;

        if (anim.currentFrame >= anim.frames) anim.currentFrame = 0;

        anim.nextFrameDataOffset = anim.image.width * anim.image.height * 4 * anim.currentFrame;

        updateTexture(anim.texture, static_cast<char*>(anim.image.data) + anim.nextFrameDataOffset);
    }
}

void ENGI::GameEngine::unloadGifsAndTextures() {
    for (auto& gif : gifs)
    {
        UnloadTexture(gif.second.texture);
        UnloadImage(gif.second.image);
    }

    for (auto& texture : textures)
    {
        UnloadTexture(texture.second);
    }
}

void ENGI::GameEngine::setReplayMode(bool replay, GameData& gd)
{
    replayMode = replay;
    gameData = &gd;
}