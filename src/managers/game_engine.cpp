#include "game_engine.hpp"
#include <chrono>

ENGI::GameEngine::GameEngine(u16 const width, u16 const height)
    : width_{ width }, height_{ height }
{
    ENGI::GameEngine::initWindow(width_, height_, "Two Wonders");

    camera = dmeg.GetCamera();

    ENGI::GameEngine::setUpCamera({ 0.0f, 1.0f, 0.0f });
    ENGI::GameEngine::setProjectionCamera(DarkMoon::CameraProjection::CAMERA_ORTHOGRAPHIC);

    nodes["3D"] = dmeg.CreateNode("Scene 3D", dmeg.GetRootNode());
    nodes["2D"] = dmeg.CreateNode("Scene 2D", dmeg.GetRootNode());
    nodes["HUD"] = dmeg.CreateNode("HUD", nodes["2D"]);
    nodes["Menu"] = dmeg.CreateNode("Menu", nodes["2D"]);
    nodes["Dialog"] = dmeg.CreateNode("Dialog", nodes["HUD"]);
    nodes["Gifs"] = dmeg.CreateNode("Gifs", nodes["2D"]);
    nodes["Nums"] = dmeg.CreateNode("Nums", nodes["HUD"]);
    nodes["Faces"] = dmeg.CreateNode("Faces", nodes["HUD"]);
    nodes["Hearts"] = dmeg.CreateNode("Hearts", nodes["HUD"]);
    nodes["ManaBar"] = dmeg.CreateNode("Mana", nodes["HUD"]);
    nodes["CoinBar"] = dmeg.CreateNode("Coins", nodes["HUD"]);
    nodes["Copy"] = dmeg.CreateNode("Copy", nodes["HUD"]);
    nodes["AnimTextures"] = dmeg.CreateNode("AnimTextures", nodes["HUD"]);

    ENGI::GameEngine::setExitKey(D_KEY_F8);

    // Logo TwoWonders
    loadAndResizeImage("logo_twowonders", "assets/logo_two_wonders.png", nodes["2D"]);

    // Logo Kaiwa
    loadAndResizeImage("logo_kaiwa", "assets/logo_kaiwa_games.png", nodes["2D"]);

    // Corazón HUD
    loadAndResizeImage("heart", "assets/HUD/corazon.png", nodes["Hearts"]);

    // Medio Corazón HUD
    loadAndResizeImage("half_heart", "assets/HUD/corazon_medio.png", nodes["Hearts"]);

    // Corazones vacíos HUD
    loadAndResizeImage("empty_heart", "assets/HUD/corazon_vacio.png", nodes["Hearts"]);;

    // Corazones Hielo HUD
    loadAndResizeImage("ice_heart", "assets/HUD/corazon_escudo2.png", nodes["Hearts"]);;

    // Medio Corazón Hielo HUD
    loadAndResizeImage("half_ice_heart", "assets/HUD/corazon_escudo_medio2.png", nodes["Hearts"]);

    // Corazones vacíos Hielo HUD
    loadAndResizeImage("empty_ice_heart", "assets/HUD/corazon_escudo_v2.png", nodes["Hearts"]);

    // Mago Happy HUD
    loadAndResizeImage("mago_happy", "assets/HUD/caras/mago_happy.png", nodes["Faces"]);

    // Mago Meh HUD
    loadAndResizeImage("mago_meh1", "assets/HUD/caras/mago_meh.png", nodes["Faces"]);

    // Mago SOS HUD
    loadAndResizeImage("mago_sos1", "assets/HUD/caras/mago_sos.png", nodes["Faces"]);

    // Nómada HUD
    loadAndResizeImage("nomada", "assets/HUD/caras/calabaza3.png", nodes["Faces"]);

    // Investigador HUD
    loadAndResizeImage("investigador", "assets/HUD/caras/investigador.png", nodes["Faces"]);

    // Candado abierto HUD
    loadAndResizeImage("candado_abierto", "assets/HUD/candado_abierto.png", nodes["HUD"]);

    // Candado cerrado HUD
    loadAndResizeImage("candado_cerrado", "assets/HUD/candado_cerrado.png", nodes["HUD"]);

    // Fijado Destello HUD
    loadAndResizeImage("destellin", "assets/HUD/fijado_destellin.png", nodes["HUD"]);

    // Barra de destellos HUD
    loadAndResizeImage("destellos", "assets/HUD/destellos.png", nodes["CoinBar"]);

    // Rectángulo de la barra de maná
    createRectangle({ 0.0f, 0.0f }, { 1.0f, 35 }, { 154, 222, 235, 255 }, "mana_rect", nodes["ManaBar"]);

    // Borde Barra Maná
    loadAndResizeImage("borde_mana", "assets/HUD/mana_bar.png", nodes["ManaBar"]);

    // Cuadro de diálogo
    createTextBox({ 0, 0 }, { 600, 120 }, D_WHITE, "", dmeg.GetDefaultFont(),
        20, D_BLACK, DarkMoon::Aligned::CENTER, DarkMoon::Aligned::CENTER, "cuadroDialogo", nodes["Dialog"]);

    // Diálogo Siguiente HUD
    loadAndResizeImage("sig", "assets/HUD/dialog_siguiente.png", nodes["Dialog"]);

    // Espacio para hechizos de agua HUD
    loadAndResizeImage("placeholder", "assets/HUD/item_agua.png", nodes["AnimTextures"]);

    // Icono de palo HUD
    loadAndResizeImage("palo", "assets/HUD/palo.png", nodes["AnimTextures"]);

    // Icono para las pompas de agua HUD
    loadAndResizeImage("pompas", "assets/HUD/pompas.png", nodes["AnimTextures"]);

    // Icono para el dash de agua HUD
    loadAndResizeImage("dash", "assets/HUD/dash.png", nodes["AnimTextures"]);

    // Icono para la bola de fuego HUD
    loadAndResizeImage("bola_fuego", "assets/HUD/bolafuego.png", nodes["AnimTextures"]);

    // Icono para meteoritos HUD
    loadAndResizeImage("meteoritos", "assets/HUD/meteoritos.png", nodes["AnimTextures"]);

    // Icono para escudo de hielo HUD
    loadAndResizeImage("escudo_hielo", "assets/HUD/escudo.png", nodes["AnimTextures"]);

    // Icono para las estacas de hielo HUD
    loadAndResizeImage("estacas", "assets/HUD/estacas.png", nodes["AnimTextures"]);

    // Libro para enseñar hechizos HUD
    loadAndResizeImage("libro", "assets/HUD/Libro.png", nodes["AnimTextures"]);

    // Icono de detección HUD
    loadAndResizeImage("detectionIcon", "assets/HUD/detectionicon.png", nodes["HUD"]);

    // Barco para el nivel del volcán HUD
    loadAndResizeImage("barco", "assets/HUD/barco_piezas.png", nodes["HUD"]);

    // Icono Batalla HUD
    loadAndResizeImage("batalla", "assets/HUD/batalla.png", nodes["HUD"]);

    // Botón círculo HUD 
    loadAndResizeImage("boton_circulo", "assets/HUD/botones/cont_circulo.png", nodes["AnimTextures"]);

    // Botón cuadrado HUD
    loadAndResizeImage("boton_cuadrado", "assets/HUD/botones/cont_cuadrado.png", nodes["AnimTextures"]);

    // Botón triángulo HUD
    loadAndResizeImage("boton_triangulo", "assets/HUD/botones/cont_triangulo.png", nodes["AnimTextures"]);

    // Botón R2 HUD
    loadAndResizeImage("boton_r2", "assets/HUD/botones/cont_R2.png", nodes["AnimTextures"]);

    // Tecla J HUD
    loadAndResizeImage("tecla_j", "assets/HUD/teclas/cont_J.png", nodes["AnimTextures"]);

    // Tecla K HUD
    loadAndResizeImage("tecla_k", "assets/HUD/teclas/cont_K.png", nodes["AnimTextures"]);

    // Tecla L HUD
    loadAndResizeImage("tecla_l", "assets/HUD/teclas/cont_L.png", nodes["AnimTextures"]);

    // Tecla Espacio HUD
    loadAndResizeImage("tecla_espacio", "assets/HUD/teclas/cont_space.png", nodes["AnimTextures"]);

    // Imagen Mando HUD
    loadAndResizeImage("mando_explicacion", "assets/HUD/botones/Mando_botones.png", nodes["2D"]);

    // Imagen Teclado HUD
    loadAndResizeImage("teclado_explicacion", "assets/HUD/teclas/teclado_ctrl.png", nodes["2D"]);

    // NÚMEROS Y SÍMBOLOS NUMÉRICOS
    //
    loadAndResizeImage("0", "assets/HUD/numeros/0.png", nodes["Nums"]);
    loadAndResizeImage("1", "assets/HUD/numeros/1.png", nodes["Nums"]);
    loadAndResizeImage("2", "assets/HUD/numeros/2.png", nodes["Nums"]);
    loadAndResizeImage("3", "assets/HUD/numeros/3.png", nodes["Nums"]);
    loadAndResizeImage("4", "assets/HUD/numeros/4.png", nodes["Nums"]);
    loadAndResizeImage("5", "assets/HUD/numeros/5.png", nodes["Nums"]);
    loadAndResizeImage("6", "assets/HUD/numeros/6.png", nodes["Nums"]);
    loadAndResizeImage("7", "assets/HUD/numeros/7.png", nodes["Nums"]);
    loadAndResizeImage("8", "assets/HUD/numeros/8.png", nodes["Nums"]);
    loadAndResizeImage("9", "assets/HUD/numeros/9.png", nodes["Nums"]);
    loadAndResizeImage("+", "assets/HUD/numeros/mas.png", nodes["Nums"]);
    loadAndResizeImage("-", "assets/HUD/numeros/-.png", nodes["Nums"]);
    loadAndResizeImage("barra", "assets/HUD/barra.png", nodes["Nums"]);

    // GIFS
    //
    // Joystick Izquierdo
    loadAndResizeImageGif("joystick_izq", "assets/HUD/botones/joystickL.gif");

    // Fijador de cámara
    loadAndResizeImageGif("fijado", "assets/HUD/gifs/fijado_trama.gif");

    // Botón X
    loadAndResizeImageGif("x", "assets/HUD/botones/x.gif");

    // Botón Círculo
    loadAndResizeImageGif("circulo", "assets/HUD/botones/circulo.gif");

    // Botón Triángulo
    loadAndResizeImageGif("triangulo", "assets/HUD/botones/triangulo.gif");

    // Botón Cuadrado
    loadAndResizeImageGif("cuadrado", "assets/HUD/botones/cuadrado.gif");

    // Botón Menú
    loadAndResizeImageGif("menu", "assets/HUD/botones/menu.gif");

    // Tecla E
    loadAndResizeImageGif("e", "assets/HUD/teclas/e.gif");

    // Tecla F
    loadAndResizeImageGif("f", "assets/HUD/teclas/f.gif");

    // Tecla Q
    loadAndResizeImageGif("q", "assets/HUD/teclas/q.gif");

    // Tecla I
    loadAndResizeImageGif("i", "assets/HUD/teclas/i.gif");

    // Tecla H
    loadAndResizeImageGif("h", "assets/HUD/teclas/H.gif");

    // Tecla L
    loadAndResizeImageGif("l", "assets/HUD/teclas/L.gif");

    // Tecla J
    loadAndResizeImageGif("j", "assets/HUD/teclas/J.gif");

    // Tecla K
    loadAndResizeImageGif("k", "assets/HUD/teclas/k.gif");

    // Tecla O
    loadAndResizeImageGif("o", "assets/HUD/teclas/o.gif");

    // Tecla U
    loadAndResizeImageGif("u", "assets/HUD/teclas/u.gif");

    // Tecla Espacio
    loadAndResizeImageGif("espacio", "assets/HUD/teclas/espacio.gif");

    // Pantalla de carga
    loadAndResizeImageGif("carga", "assets/HUD/gifs/carga_elementos.gif");

    // Teclas WASD
    loadAndResizeImageGif("wasd", "assets/HUD/teclas/WASD.gif");

    // Ejemplo pompa
    loadAndResizeImageGif("exp_pompa", "assets/HUD/gifs/hechizos/exp_pompa.gif");

    // Ejemplo dash de agua
    loadAndResizeImageGif("exp_dash", "assets/HUD/gifs/hechizos/exp_dash.gif");

    // Ejemplo bola de fuego
    loadAndResizeImageGif("exp_bola_f", "assets/HUD/gifs/hechizos/exp_bola_f.gif");

    // Botón L2
    loadAndResizeImageGif("l2", "assets/HUD/botones/L2.gif");

    // Botón R2
    loadAndResizeImageGif("r2", "assets/HUD/botones/R2.gif");

    // Botón L1
    loadAndResizeImageGif("l1", "assets/HUD/botones/L1.gif");

    // Botón R1
    loadAndResizeImageGif("r1", "assets/HUD/botones/R1.gif");

    //Detección por oído
    loadAndResizeImageGif("Oido_parp1", "assets/HUD/gifs/Oido_parp1.gif");
    loadAndResizeImageGif("Oido_parp2", "assets/HUD/gifs/Oido_parp2.gif");
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

TextureType ENGI::GameEngine::loadTextureFromImage(Image image) {
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
    dmeg.BeginDrawing();
}

void ENGI::GameEngine::clearBackground(DarkMoon::Color color) {
    dmeg.ClearBackground(color);
}

void ENGI::GameEngine::drawGrid(int slices, float spacing) {
    DrawGrid(slices, spacing);
}

void ENGI::GameEngine::endDrawing() {
    dmeg.EndDrawing();
    // EndDrawing();
}

void ENGI::GameEngine::beginMode3D() {
    // BeginMode3D(camera);
}

void ENGI::GameEngine::endMode3D() {
    // EndMode3D();
}

void ENGI::GameEngine::drawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color) {
    DrawLine(startPosX, startPosY, endPosX, endPosY, color);
}

void ENGI::GameEngine::drawLine3D(vec3d startPos, vec3d endPos, Color color) {
    DrawLine3D(startPos.toRaylib(), endPos.toRaylib(), color);
}

void ENGI::GameEngine::drawPoint3D(vec3d pos, Color color) {
    DrawPoint3D(pos.toRaylib(), color);
}

void ENGI::GameEngine::drawCube(vec3d pos, float width, float height, float lenght, Color color) {
    DrawCube(pos.toRaylib(), width, height, lenght, color);
}

void ENGI::GameEngine::drawCubeWires(vec3d pos, float width, float height, float lenght, Color color) {
    DrawCubeWires(pos.toRaylib(), width, height, lenght, color);
}

// void ENGI::GameEngine::drawModel(ModelType model, vec3d position, vec3d rotationAxis, float rotationAngle, vec3d scale, Color tint) {
//     DrawModelEx(model, position.toRaylib(), rotationAxis.toRaylib(), rotationAngle, scale.toRaylib(), tint);
// }

// void ENGI::GameEngine::drawModelWires(ModelType model, vec3d position, vec3d rotationAxis, float rotationAngle, vec3d scale, Color tint) {
//     DrawModelWiresEx(model, position.toRaylib(), rotationAxis.toRaylib(), rotationAngle, scale.toRaylib(), tint);
// }

void ENGI::GameEngine::drawRectangle(int, int, int, int, Color) {
    // dmeg.drawRectangle(posX, posY, width, height, color);
}

void ENGI::GameEngine::drawRectangleLinesEx(Rectangle, float, Color) {
    // DrawRectangleLinesEx(rec, lineThick, color);
}

void ENGI::GameEngine::drawRectangleRec(Rectangle, Color) {
    // DrawRectangleRec(rec, color);
}

void ENGI::GameEngine::drawTexture(TextureType, int, int, Color) {
    //DrawTexture(texture, posX, posY, tint);
}

void ENGI::GameEngine::drawTexture(TextureType, int, int, Color, float) {
    //DrawTextureEx(texture, { static_cast<float>(posX), static_cast<float>(posY) }, 0.0f, scale, tint);
}

void ENGI::GameEngine::drawNode(DarkMoon::Node* node, vec2d pos, vec2f scale) {
    if (pos != vec2d(0, 0))
        node->setTranslation({ pos.x, pos.y, 0.0f });

    node->setScale({ scale.x, scale.y, 1.0f });
    node->setVisibleOne(true);
}

void ENGI::GameEngine::drawCircle(int, int, float, Color) {
    //DrawCircle(posX, posY, radius, color);
}

void ENGI::GameEngine::drawCircleSector(vec2d, float, float, float, int, Color) {
    //DrawCircleSector(center.toRaylib(), radius, startAngle, endAngle, segments, color);
}

void ENGI::GameEngine::drawTriangle(vec2d, vec2d, vec2d, Color) {
    //DrawTriangle(v1.toRaylib(), v2.toRaylib(), v3.toRaylib(), color);
}

////// TEXT //////

void ENGI::GameEngine::drawText(const char*, int, int, int, Color) {
    //DrawText(text, posX, posY, fontSize, color);
}

void ENGI::GameEngine::drawTextEx(Font, const char*, vec2d, float, float, Color) {
    //DrawTextEx(font, text, position.toRaylib(), fontSize, spacing, tint);
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
    dmeg.InitWindow(width, height, title);
}

void ENGI::GameEngine::closeWindow()
{
    dmeg.CloseWindow();
}

bool ENGI::GameEngine::windowShouldClose()
{
    return dmeg.WindowShouldClose();
}

int ENGI::GameEngine::getScreenWidth()
{
    return dmeg.GetScreenWidth();
}

int ENGI::GameEngine::getScreenHeight()
{
    return dmeg.GetScreenHeight();
}

void ENGI::GameEngine::setWindowSize(int width, int height)
{
    if (dmeg.IsWindowFullscreen())
        dmeg.ToggleFullscreen();

    dmeg.SetWindowSize(width, height);
}

void ENGI::GameEngine::setWindowFullScreen()
{
    dmeg.ToggleFullscreen();
}

void ENGI::GameEngine::setExitKey(int key)
{
    dmeg.SetExitKey(key);
}

////// CAMERA //////

void ENGI::GameEngine::setPositionCamera(vec3d pos)
{
    camera->position = { pos.x(), pos.y(), pos.z() };
}

void ENGI::GameEngine::setTargetCamera(vec3d tar)
{
    camera->target = { tar.x(), tar.y(), tar.z() };
}

void ENGI::GameEngine::setUpCamera(vec3d up)
{
    camera->up = { up.x(), up.y(), up.z() };
}

void ENGI::GameEngine::setFovyCamera(float fovy)
{
    camera->fovy = fovy;
}

void ENGI::GameEngine::setProjectionCamera(DarkMoon::CameraProjection proj)
{
    camera->cameraProjection = proj;
}

vec3d ENGI::GameEngine::getPositionCamera()
{
    return vec3d(camera->position.x, camera->position.y, camera->position.z);
}

vec3d ENGI::GameEngine::getTargetCamera()
{
    return vec3d(camera->target.x, camera->target.y, camera->target.z);
}

vec3d ENGI::GameEngine::getUpCamera()
{
    return vec3d(camera->up.x, camera->up.y, camera->up.z);
}

float ENGI::GameEngine::getFovyCamera()
{
    return camera->fovy;
}

////// INPUT HANDLING //////

bool ENGI::GameEngine::isKeyPressed(int key)
{
    if (!replayMode)
    {
        if (dmeg.IsKeyPressed(key))
        {
            switch (key)
            {
            case D_KEY_E:
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
        if (dmeg.IsKeyDown(key))
        {
            switch (key)
            {
            case D_KEY_SPACE:
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
        if (dmeg.IsKeyReleased(key))
        {
            switch (key)
            {
            case D_KEY_ENTER:
                gameData->addInputEvent(InputEvent::Type::EnterReleased);
                break;
            case D_KEY_ESCAPE:
                gameData->addInputEvent(InputEvent::Type::EscapeReleased);
                break;
            case D_KEY_I:
                gameData->addInputEvent(InputEvent::Type::InventoryReleased);
                break;
            case D_KEY_F:
                gameData->addInputEvent(InputEvent::Type::LockInReleased);
                break;
            case D_KEY_Q:
                gameData->addInputEvent(InputEvent::Type::ChangeSpellReleased);
                break;
            case D_KEY_E:
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
    return dmeg.IsMouseButtonPressed(button);
}

bool ENGI::GameEngine::isMouseButtonDown(int button)
{
    return dmeg.IsMouseButtonDown(button);
}

bool ENGI::GameEngine::isGamepadAvailable(int gamepad)
{
    return dmeg.IsGamepadAvailable(gamepad);
}

bool ENGI::GameEngine::isGamepadButtonPressed(int gamepad, int button)
{
    if (!replayMode)
    {
        if (dmeg.IsGamepadButtonPressed(gamepad, button))
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
            return gameData->isKeyPressed(D_KEY_E);
            break;
        case GAMEPAD_BUTTON_RIGHT_FACE_LEFT:
            return gameData->isKeyDown(D_KEY_SPACE);
            break;
        default:
            return false;
        }
    }
}

bool ENGI::GameEngine::isGamepadButtonDown(int gamepad, int button)
{
    return dmeg.IsGamepadButtonDown(gamepad, button);
}

bool ENGI::GameEngine::isGamepadButtonReleased(int gamepad, int button)
{
    if (!replayMode)
    {
        if (dmeg.IsGamepadButtonReleased(gamepad, button))
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
            return gameData->isKeyReleased(D_KEY_I);
            break;
        case GAMEPAD_BUTTON_RIGHT_FACE_RIGHT:
            return gameData->isKeyReleased(D_KEY_F);
            break;
        case GAMEPAD_BUTTON_RIGHT_FACE_DOWN:
            return gameData->isKeyReleased(D_KEY_E);
            break;
        default:
            return false;
        }
    }
}

float ENGI::GameEngine::getGamepadAxisMovement(int gamepad, int axis)
{
    return dmeg.GetGamepadAxisMovement(gamepad, axis);
}

////// MOUSE COLLISION //////

bool ENGI::GameEngine::checkCollisionPointRec(Vector2 point, Rectangle rec)
{
    return CheckCollisionPointRec(point, rec);
}

////// SHADERS //////

// ShaderType ENGI::GameEngine::loadShader(const char* vsFileName, const char* fsFileName)
// {
//     return *dmeg.LoadShader("holiiiiii :3", vsFileName, fsFileName);
// }

// void ENGI::GameEngine::unloadShader(ShaderType s)
// {
//     UnloadShader(s);
// }

// int ENGI::GameEngine::getShaderLocation(ShaderType s, const char* uniformName)
// {
//     return GetShaderLocation(s, uniformName);
// }

// void ENGI::GameEngine::setShaderValue(ShaderType s, int uniformLoc, const void* value, int uniformType)
// {
//     SetShaderValue(s, uniformLoc, value, uniformType);
// }

////// AUX //////

MeshType ENGI::GameEngine::genMeshCube(float width, float height, float lenght)
{
    return GenMeshCube(width, height, lenght);
}

Model ENGI::GameEngine::loadModelFromMesh(MeshType m)
{
    return LoadModelFromMesh(m);
}

DarkMoon::Node* ENGI::GameEngine::loadModel(const char* filename)
{
    return dmeg.CreateModel(filename, D_WHITE, filename, nodes["3D"]);
}

// ModelType ENGI::GameEngine::loadModelRaylib(const char* filename)
// {
//     return LoadModel(filename);
// }

TextureType ENGI::GameEngine::loadTexture(const char* filename)
{
    return LoadTexture(filename);
}

DarkMoon::Texture* ENGI::GameEngine::loadTexture2D(const char* filePath)
{
    return dmeg.LoadTexture2D(filePath);
}

std::vector<DarkMoon::Texture*> ENGI::GameEngine::loadTextures2DAnim(const char* filePath)
{
    return dmeg.LoadTexture2DAnim(filePath);
}

void ENGI::GameEngine::updateTexture(TextureType texture, const void* data)
{
    UpdateTexture(texture, data);
}

void ENGI::GameEngine::unloadMesh(MeshType m)
{
    UnloadMesh(m);
}

// void ENGI::GameEngine::unloadModel(ModelType m)
// {
//     UnloadModel(m);
// }

float ENGI::GameEngine::getWorldToScreenX(vec3d pos)
{
    return dmeg.GetWorldToScreen({ pos.x(), pos.y(), pos.z() }).x;
}

float ENGI::GameEngine::getWorldToScreenY(vec3d pos)
{
    return dmeg.GetWorldToScreen({ pos.x(), pos.y(), pos.z() }).y;
}

RayCast ENGI::GameEngine::getMouseRay()
{
    auto ray = dmeg.GetMouseRay();
    return RayCast{ {ray.origin.x, ray.origin.y, ray.origin.z}, {ray.direction.x, ray.direction.y, ray.direction.z} };
    //Ray r = GetMouseRay(GetMousePosition(), camera);
    //return RayCast{ .origin = vec3d(r.position.x, r.position.y, r.position.z), .direction = vec3d(r.direction.x, r.direction.y, r.direction.z) };
}

void ENGI::GameEngine::loadAndResizeImage(const char* name, const char* path, DarkMoon::Node* parentNode) {
    if (!nodes[name])
        nodes[name] = dmeg.CreateTexture2D({ 0, 0 }, path, D_WHITE, name, parentNode);
}

void ENGI::GameEngine::loadAndResizeImageGif(const char* name, const char* filePath) {

    auto textures = loadTextures2DAnim(filePath);

    if (!nodes[name])
        nodes[name] = dmeg.CreateAnimatedTexture2D({ 0.0f, 0.0f }, textures, D_WHITE, 0.5f, 0, name, nodes["Gifs"]);
}

void ENGI::GameEngine::updateGif(Gif&) {
    //anim.frameCounter += 1;
    //if (anim.frameCounter >= anim.frameDelay)
    //{
    //    anim.frameCounter = 0;
    //    anim.currentFrame++;
//
    //    if (anim.currentFrame >= anim.frames) anim.currentFrame = 0;
//
    //    anim.nextFrameDataOffset = anim.image.width * anim.image.height * 4 * anim.currentFrame;
//
    //    updateTexture(anim.texture, static_cast<char*>(anim.image.data) + anim.nextFrameDataOffset);
    //}
}

void ENGI::GameEngine::unloadGifsAndTextures() {
    //for (auto& gif : gifs)
    //{
    //    UnloadTexture(gif.second.texture);
    //    UnloadImage(gif.second.image);
    //}
//
    //for (auto& texture : textures)
    //{
    //    UnloadTexture(texture.second);
    //}
}

void ENGI::GameEngine::setReplayMode(bool replay, GameData& gd)
{
    replayMode = replay;
    gameData = &gd;
}

double ENGI::GameEngine::getTime()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    return static_cast<double>(microseconds.count()) / 1e6;
}

DarkMoon::Node* ENGI::GameEngine::createNode(const char* name, DarkMoon::Node* parentNode)
{
    if (!nodes[name])
        nodes[name] = dmeg.CreateNode(name, parentNode);

    return nodes[name];
}

DarkMoon::Node* ENGI::GameEngine::createNode(DarkMoon::Node* copyNode, DarkMoon::Node* parentNode)
{
    return dmeg.CreateNodeCopy(copyNode, parentNode);
}

DarkMoon::Node* ENGI::GameEngine::createRectangle(vec2d pos, vec2d size, DarkMoon::Color color, const char* name, DarkMoon::Node* parentNode)
{
    if (!nodes[name])
        nodes[name] = dmeg.CreateRectangle({ pos.x, pos.y }, { size.x, size.y }, color, name, parentNode);

    return nodes[name];
}

DarkMoon::Node* ENGI::GameEngine::createTextBox(glm::vec2 position, glm::vec2 size, DarkMoon::Color boxColor, std::string text, DarkMoon::Font* font, int fontSize, DarkMoon::Color textColor, DarkMoon::Aligned verticalAligned, DarkMoon::Aligned horizontalAligned, const char* nodeName, DarkMoon::Node* parentNode)
{
    if (!nodes[nodeName])
        nodes[nodeName] = dmeg.CreateTextBox(position, size, boxColor, text, font, fontSize, textColor, verticalAligned, horizontalAligned, nodeName, parentNode);

    return nodes[nodeName];
}

float ENGI::GameEngine::getAspectRat()
{
    return static_cast<float>(width_) / static_cast<float>(height_);
}

DarkMoon::Node* ENGI::GameEngine::get2D()
{
    return nodes["2D"];
}

DarkMoon::Node* ENGI::GameEngine::get3D()
{
    return nodes["3D"];
}