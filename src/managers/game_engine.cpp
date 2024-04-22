#include "game_engine.hpp"
#include <chrono>

ENGI::GameEngine::GameEngine(u16 const width, u16 const height)
    : width_{ width }, height_{ height }
{
    ENGI::GameEngine::initWindow(width_, height_, "Two Wonders");

    camera = dmeg.GetCamera();

    ENGI::GameEngine::setUpCamera({ 0.0f, 1.0f, 0.0f });
    ENGI::GameEngine::setProjectionCamera(DarkMoon::CameraProjection::CAMERA_ORTHOGRAPHIC);

    node_scene3D = dmeg.CreateNode("Scene 3D", dmeg.GetRootNode());
    node_scene2D = dmeg.CreateNode("Scene 2D", dmeg.GetRootNode());
    node_sceneTextures = dmeg.CreateNode("Texturas", node_scene2D);

    ENGI::GameEngine::setExitKey(D_KEY_F8);

    // // Diálogo Siguiente HUD
    // loadAndResizeImage("sig", "assets/HUD/dialog_siguiente.png");

    // // Espacio para hechizos de agua HUD
    // loadAndResizeImage("agua_holder", "assets/HUD/item_agua.png");

    // // Icono para las pompas de agua HUD
    // loadAndResizeImage("pompas", "assets/HUD/pompas.png", 1.5, 1.5);

    // // NÚMEROS
    // //
    // loadAndResizeImage("0", "assets/HUD/numeros/0.png");
    // loadAndResizeImage("1", "assets/HUD/numeros/1.png");
    // loadAndResizeImage("2", "assets/HUD/numeros/2.png");
    // loadAndResizeImage("3", "assets/HUD/numeros/3.png");
    // loadAndResizeImage("4", "assets/HUD/numeros/4.png");
    // loadAndResizeImage("5", "assets/HUD/numeros/5.png");
    // loadAndResizeImage("6", "assets/HUD/numeros/6.png");
    // loadAndResizeImage("7", "assets/HUD/numeros/7.png");
    // loadAndResizeImage("8", "assets/HUD/numeros/8.png");
    // loadAndResizeImage("9", "assets/HUD/numeros/9.png");

    // --- GIFS --- //
    auto node_texturas_animadas = createNode("texturas_animadas", node_scene2D);
    std::vector<DarkMoon::Texture*> aux_textures;

    // [NUEVAS IMAGENES Y TEXTURAS]
    // Medio Corazón HUD
    // loadAndResizeImage("half_heart", "assets/HUD/corazon_medio.png");

    // // Corazones vacíos HUD
    // loadAndResizeImage("empty_heart", "assets/HUD/corazon_vacio.png");

    // // Corazones Hielo HUD
    // loadAndResizeImage("ice_heart", "assets/HUD/corazon_escudo2.png");

    // // Medio Corazón Hielo HUD
    // loadAndResizeImage("half_ice_heart", "assets/HUD/corazon_escudo_medio2.png");

    // // Corazones vacíos Hielo HUD
    // loadAndResizeImage("empty_ice_heart", "assets/HUD/corazon_escudo_v2.png");

    // // Mago Happy HUD
    // loadAndResizeImage("mago_happy", "assets/HUD/caras/mago_happy.png");

    // // Mago Meh HUD
    // loadAndResizeImage("mago_meh", "assets/HUD/caras/mago_meh.png");

    // // Mago SOS HUD
    // loadAndResizeImage("mago_sos", "assets/HUD/caras/mago_sos.png");

    // // Nómada HUD
    // loadAndResizeImage("nomada", "assets/HUD/caras/calabaza3.png");

    // // Investigador HUD
    // loadAndResizeImage("investigador", "assets/HUD/caras/investigador.png");

    // Fijador de camara
    {
        aux_textures.push_back(loadTexture2D("assets/HUD/gifs/fijado_trama/fijado_trama-0.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/gifs/fijado_trama/fijado_trama-1.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/gifs/fijado_trama/fijado_trama-2.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/gifs/fijado_trama/fijado_trama-3.png"));

        createAnimatedTexture2D(aux_textures, D_WHITE, 1.0f, 0, "fijado", node_texturas_animadas);
        aux_textures.clear();
    }

    // // Botón Triángulo
    // loadAndResizeImageGif("triangulo", "assets/HUD/botones/triangulo.gif", 0);

    // // Botón Menú
    // loadAndResizeImageGif("menu", "assets/HUD/botones/menu.gif", 0);

    // Pantalla de carga
    {
        aux_textures.push_back(loadTexture2D("assets/HUD/gifs/carga_elementos/carga_elementos-0.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/gifs/carga_elementos/carga_elementos-1.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/gifs/carga_elementos/carga_elementos-2.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/gifs/carga_elementos/carga_elementos-3.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/gifs/carga_elementos/carga_elementos-4.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/gifs/carga_elementos/carga_elementos-5.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/gifs/carga_elementos/carga_elementos-6.png"));

        createAnimatedTexture2D(aux_textures, D_WHITE, 1.0f, 0, "carga", node_texturas_animadas);
        aux_textures.clear();
    }
    // // Candado abierto HUD
    // loadAndResizeImage("candado_abierto", "assets/HUD/candado_abierto.png");

    // // Candado cerrado HUD
    // loadAndResizeImage("candado_cerrado", "assets/HUD/candado_cerrado.png");

    // // Diálogo Siguiente HUD
    // loadAndResizeImage("sig", "assets/HUD/dialog_siguiente.png");

    // // Espacio para hechizos de agua HUD
    // loadAndResizeImage("placeholder", "assets/HUD/item_agua.png");

    // // Icono de palo HUD
    // loadAndResizeImage("palo", "assets/HUD/palo.png");

    // // Icono para las pompas de agua HUD
    // loadAndResizeImage("pompas", "assets/HUD/pompas.png", 1.5, 1.5);

    // // Icono para el dash de agua HUD
    // loadAndResizeImage("dash", "assets/HUD/dash.png");

    // // Icono para la bola de fuego HUD
    // loadAndResizeImage("bola_fuego", "assets/HUD/bolafuego.png");

    // // Icono para meteoritos HUD
    // loadAndResizeImage("meteoritos", "assets/HUD/meteoritos.png");

    // // Icono para escudo de hielo HUD
    // loadAndResizeImage("escudo_hielo", "assets/HUD/escudo.png");

    // // Icono para las estacas de hielo HUD
    // loadAndResizeImage("estacas", "assets/HUD/estacas.png");

    // // Libro para enseñar hechizos HUD
    // loadAndResizeImage("libro", "assets/HUD/Libro.png");

    // // Icono de detección HUD
    // loadAndResizeImage("detectionicon", "assets/HUD/detectionicon.png");

    // // Barco para el nivel del volcán HUD
    // loadAndResizeImage("barco", "assets/HUD/barco_piezas.png");

    // // Icono Batalla HUD
    // loadAndResizeImage("batalla", "assets/HUD/batalla.png");

    // // Botón círculo HUD 
    // loadAndResizeImage("boton_circulo", "assets/HUD/botones/cont_circulo.png");

    // // Botón cuadrado HUD
    // loadAndResizeImage("boton_cuadrado", "assets/HUD/botones/cont_cuadrado.png");

    // // Botón triángulo HUD
    // loadAndResizeImage("boton_triangulo", "assets/HUD/botones/cont_triangulo.png");

    // // Botón R2 HUD
    // loadAndResizeImage("boton_r2", "assets/HUD/botones/cont_R2.png");

    // // Tecla J HUD
    // loadAndResizeImage("tecla_j", "assets/HUD/teclas/cont_J.png");

    // // Tecla K HUD
    // loadAndResizeImage("tecla_k", "assets/HUD/teclas/cont_K.png");

    // // Tecla L HUD
    // loadAndResizeImage("tecla_l", "assets/HUD/teclas/cont_L.png");

    // // Tecla Espacio HUD
    // loadAndResizeImage("tecla_espacio", "assets/HUD/teclas/cont_space.png");

    // // Imagen Mando HUD
    // loadAndResizeImage("mando_explicacion", "assets/HUD/botones/Mando_botones.png");

    // // Imagen Teclado HUD
    // loadAndResizeImage("teclado_explicacion", "assets/HUD/teclas/teclado_ctrl.png");

    // NÚMEROS Y SÍMBOLOS NUMÉRICOS
    //
    // loadAndResizeImage("0", "assets/HUD/numeros/0.png");
    // loadAndResizeImage("1", "assets/HUD/numeros/1.png");
    // loadAndResizeImage("2", "assets/HUD/numeros/2.png");
    // loadAndResizeImage("3", "assets/HUD/numeros/3.png");
    // loadAndResizeImage("4", "assets/HUD/numeros/4.png");
    // loadAndResizeImage("5", "assets/HUD/numeros/5.png");
    // loadAndResizeImage("6", "assets/HUD/numeros/6.png");
    // loadAndResizeImage("7", "assets/HUD/numeros/7.png");
    // loadAndResizeImage("8", "assets/HUD/numeros/8.png");
    // loadAndResizeImage("9", "assets/HUD/numeros/9.png");
    // loadAndResizeImage("+", "assets/HUD/numeros/mas.png");
    // loadAndResizeImage("-", "assets/HUD/numeros/-.png");
    // loadAndResizeImage("barra", "assets/HUD/barra.png");

    // GIFS
    //
    // // Joystick Izquierdo
    // loadAndResizeImageGif("joystick_izq", "assets/HUD/botones/joystickL.gif", 0, 18);

    // // Fijador de cámara
    // loadAndResizeImageGif("fijado", "assets/HUD/gifs/fijado_trama.gif", 0, 18, 1.2, 1.2);

    // // Botón X
    // loadAndResizeImageGif("x", "assets/HUD/botones/x.gif", 0, 28);

    // // Botón Círculo
    // loadAndResizeImageGif("circulo", "assets/HUD/botones/circulo.gif", 0, 28);

    // // Botón Triángulo
    // loadAndResizeImageGif("triangulo", "assets/HUD/botones/triangulo.gif", 0);

    // // Botón Cuadrado
    // loadAndResizeImageGif("cuadrado", "assets/HUD/botones/cuadrado.gif", 0, 28);

    // // Botón Menú
    // loadAndResizeImageGif("menu", "assets/HUD/botones/menu.gif", 0);

    // // Tecla E
    // loadAndResizeImageGif("e", "assets/HUD/teclas/e.gif", 0, 28);

    // // Tecla F
    // loadAndResizeImageGif("f", "assets/HUD/teclas/f.gif", 0, 28);

    // // Tecla Q
    // loadAndResizeImageGif("q", "assets/HUD/teclas/q.gif", 0, 28);

    // // Tecla I
    // loadAndResizeImageGif("i", "assets/HUD/teclas/i.gif", 0, 28);

    // // Tecla H
    // loadAndResizeImageGif("h", "assets/HUD/teclas/H.gif", 0, 28);

    // // Tecla L
    // loadAndResizeImageGif("l", "assets/HUD/teclas/L.gif", 0, 28);

    // // Tecla J
    // loadAndResizeImageGif("j", "assets/HUD/teclas/J.gif", 0, 28);

    // // Tecla K
    // loadAndResizeImageGif("k", "assets/HUD/teclas/k.gif", 0, 28);

    // // Tecla O
    // loadAndResizeImageGif("o", "assets/HUD/teclas/o.gif", 0, 28);

    // // Tecla U
    // loadAndResizeImageGif("u", "assets/HUD/teclas/u.gif", 0, 28);

    // // Tecla Espacio
    // loadAndResizeImageGif("espacio", "assets/HUD/teclas/espacio.gif", 0, 28);

    // // Pantalla de carga
    // loadAndResizeImageGif("carga", "assets/HUD/gifs/carga_elementos.gif", 0, 28);

    // // Teclas WASD
    // loadAndResizeImageGif("wasd", "assets/HUD/teclas/WASD.gif", 0, 28);

    // // Ejemplo pompa
    // loadAndResizeImageGif("exp_pompa", "assets/HUD/gifs/hechizos/exp_pompa.gif", 0, 28);

    // // Ejemplo dash de agua
    // loadAndResizeImageGif("exp_dash", "assets/HUD/gifs/hechizos/exp_dash.gif", 0, 28);

    // // Ejemplo bola de fuego
    // loadAndResizeImageGif("exp_bola_f", "assets/HUD/gifs/hechizos/exp_bola_f.gif", 0, 28);

    // // Botón L2
    // loadAndResizeImageGif("l2", "assets/HUD/botones/L2.gif", 0, 28);

    // // Botón R2
    // loadAndResizeImageGif("r2", "assets/HUD/botones/R2.gif", 0, 28);

    // // Botón L1
    // loadAndResizeImageGif("l1", "assets/HUD/botones/L1.gif", 0, 28);

    // // Botón R1
    // loadAndResizeImageGif("r1", "assets/HUD/botones/R1.gif", 0, 28);

    // //Detección por oído
    // loadAndResizeImageGif("Oido_parp1", "assets/HUD/gifs/Oido_parp1.gif", 0, 28);
    // loadAndResizeImageGif("Oido_parp2", "assets/HUD/gifs/Oido_parp2.gif", 0, 28);


// --- MANDO --- //

// Joystick Izquierdo
    {
        aux_textures.push_back(loadTexture2D("assets/HUD/botones/joystickL/joystickL-0.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/botones/joystickL/joystickL-1.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/botones/joystickL/joystickL-2.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/botones/joystickL/joystickL-3.png"));

        createAnimatedTexture2D(aux_textures, D_WHITE, 1.0f, 0, "joystickL", node_texturas_animadas);
        aux_textures.clear();
    }

    // Botón X
    {
        aux_textures.push_back(loadTexture2D("assets/HUD/botones/x/x-0.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/botones/x/x-1.png"));

        createAnimatedTexture2D(aux_textures, D_WHITE, 1.0f, 0, "x", node_texturas_animadas);
        aux_textures.clear();
    }

    // Botón Círculo
    {
        aux_textures.push_back(loadTexture2D("assets/HUD/botones/circulo/circulo-0.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/botones/circulo/circulo-1.png"));

        createAnimatedTexture2D(aux_textures, D_WHITE, 1.0f, 0, "circulo", node_texturas_animadas);
        aux_textures.clear();
    }

    // Botón Cuadrado
    {
        aux_textures.push_back(loadTexture2D("assets/HUD/botones/cuadrado/cuadrado-0.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/botones/cuadrado/cuadrado-1.png"));

        createAnimatedTexture2D(aux_textures, D_WHITE, 1.0f, 0, "cuadrado", node_texturas_animadas);
        aux_textures.clear();
    }

    // --- TECLADO --- //

    // Tecla E
    {
        aux_textures.push_back(loadTexture2D("assets/HUD/teclas/e/e-0.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/teclas/e/e-1.png"));

        createAnimatedTexture2D(aux_textures, D_WHITE, 1.0f, 0, "e", node_texturas_animadas);
        aux_textures.clear();
    }

    // Tecla Espacio
    {
        aux_textures.push_back(loadTexture2D("assets/HUD/teclas/espacio/espacio-0.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/teclas/espacio/espacio-1.png"));

        createAnimatedTexture2D(aux_textures, D_WHITE, 1.0f, 0, "espacio", node_texturas_animadas);
        aux_textures.clear();
    }

    // Tecla F
    {
        aux_textures.push_back(loadTexture2D("assets/HUD/teclas/f/f-0.png"));
        aux_textures.push_back(loadTexture2D("assets/HUD/teclas/f/f-1.png"));

        createAnimatedTexture2D(aux_textures, D_WHITE, 1.0f, 0, "f", node_texturas_animadas);
        aux_textures.clear();
    }
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
    return dmeg.CreateModel(filename, D_WHITE, filename, node_scene3D);
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
    return RayCast{{ray.origin.x, ray.origin.y, ray.origin.z}, {ray.direction.x, ray.direction.y, ray.direction.z}};
    //Ray r = GetMouseRay(GetMousePosition(), camera);
    //return RayCast{ .origin = vec3d(r.position.x, r.position.y, r.position.z), .direction = vec3d(r.direction.x, r.direction.y, r.direction.z) };
}

void ENGI::GameEngine::loadAndResizeImage(const std::string&, const std::string&, double, double) {
    //Image image = loadImage(path.c_str());
    //imageResize(&image, static_cast<int>(image.width / reScaleX), static_cast<int>(image.height / reScaleY));
    //textures[name] = loadTextureFromImage(image);
    //unloadImage(image);
}

void ENGI::GameEngine::loadAndResizeImageGif(const std::string&, const std::string&, int, int, double, double) {
    //Gif anim;
    //anim.name = name;
    //anim.frames = frames;
    //anim.frameDelay = delay;
    //anim.reScaleX = reScaleX;
    //anim.reScaleY = reScaleY;
    //anim.image = loadImagenAnim(path.c_str(), anim.frames);
    //// imageResize(&image, static_cast<int>(image.width / 1.3), static_cast<int>(image.height / 1.3));
    //anim.texture = loadTextureFromImage(anim.image);
//
    //gifs[name] = anim;
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

DarkMoon::Node* ENGI::GameEngine::createNode(const std::string& name, DarkMoon::Node* parentNode)
{
    if (!nodes_scene[name.c_str()])
        nodes_scene[name.c_str()] = dmeg.CreateNode(name.c_str(), parentNode);

    return nodes_scene[name.c_str()];
}

DarkMoon::Node* ENGI::GameEngine::createAnimatedTexture2D(std::vector<DarkMoon::Texture*> filePaths, DarkMoon::Color color, float frameDuration, int currentFrame, const std::string& name, DarkMoon::Node* parentNode)
{
    if (!nodes_sceneAnimatedTexture[name.c_str()])
        nodes_sceneAnimatedTexture[name.c_str()] = dmeg.CreateAnimatedTexture2D({ 0.0f, 0.0f }, filePaths, color, frameDuration, currentFrame, name.c_str(), parentNode);

    return nodes_sceneAnimatedTexture[name.c_str()];
}

float ENGI::GameEngine::getAspectRat()
{
    return static_cast<float>(width_) / static_cast<float>(height_);
}