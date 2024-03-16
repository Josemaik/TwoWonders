#pragma once
#ifndef GAME_ENGINE
#define GAME_ENGINE
#include <cstdint>
#include <iostream>
#include <map>
#include "utils/types.hpp"
#include "utils/vec3D.hpp"

namespace ENGI {

    struct GameEngine
    {
        struct Gif
        {
            std::string name{};
            Image image{};
            Texture2D texture{};
            int currentFrame{};
            int frames{};
            int nextFrameDataOffset{};

            double reScaleX{ 2.0 };
            double reScaleY{ 2.0 };

            // Variable para el update del frame
            int frameCounter{ 0 };
            int frameDelay{ 25 };
        };

        using CL = MP::TypeList<PhysicsComponent, RenderComponent>;
        using TL = MP::TypeList<>;

        using u16 = std::uint16_t;

        GameEngine(u16 const width, u16 const height);

        // Timing Related Functions
        void setTargetFPS(int fps);
        float getFrameTime();

        // Image and Texture
        Image loadImage(const char* filename);
        Image loadImagenAnim(const char* filename, int& frames);
        void imageResize(Image* image, int newWidth, int newHeight);
        void unloadImage(Image image);
        Texture2D loadTextureFromImage(Image image);

        // Drawing
        void beginDrawing();
        void clearBackground(Color color);
        void drawGrid(int slices, float spacing);
        void endDrawing();
        void beginMode3D();
        void endMode3D();
        void drawLine3D(vec3d startPos, vec3d endPos, Color color);
        void drawCube(vec3d pos, float width, float height, float lenght, Color color);
        void drawCubeWires(vec3d pos, float width, float height, float lenght, Color color);
        void drawModel(Model model, vec3d position, vec3d rotationAxis, float rotationAngle, vec3d scale, Color tint);
        void drawModelWires(Model model, vec3d position, vec3d rotationAxis, float rotationAngle, vec3d scale, Color tint);
        void drawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);

        // Rectangle
        void drawRectangle(int posX, int posY, int width, int height, Color color);
        void drawRectangleLinesEx(Rectangle rec, float lineThick, Color color);
        void drawRectangleRec(Rectangle rec, Color color);
        void drawTexture(Texture2D texture, int posX, int posY, Color tint);
        void drawCircle(int posX, int posY, float radius, Color color);

        // Text
        void drawText(const char* text, int posX, int posY, int fontSize, Color color);
        void drawTextEx(Font font, const char* text, Vector2 position, float fontSize, float spacing, Color tint);

        // Window
        void initWindow(int width, int height, const char* title);
        void closeWindow();
        bool windowShouldClose();
        int getScreenWidth();
        int getScreenHeight();
        void setWindowSize(int width, int height);
        void setWindowFullScreen();

        // Camera
        void setPositionCamera(vec3d pos);
        void setTargetCamera(vec3d tar);
        void setUpCamera(vec3d up);
        void setFovyCamera(float fovy);
        void setProjectionCamera(int proj);
        vec3d getPositionCamera();
        vec3d getTargetCamera();
        vec3d getUpCamera();
        float getFovyCamera();

        // Input Handling
        bool isKeyPressed(int key);
        bool isKeyDown(int key);
        bool isKeyReleased(int key);
        bool isMouseButtonPressed(int button);
        bool isMouseButtonDown(int button);
        bool isGamepadAvailable(int gamepad);
        bool isGamepadButtonPressed(int gamepad, int button);
        bool isGamepadButtonDown(int gamepad, int button);
        bool isGamepadButtonReleased(int gamepad, int button);
        float getGamepadAxisMovement(int gamepad, int axis);

        // Mouse collision
        bool checkCollisionPointRec(Vector2 point, Rectangle rec);

        // Shaders
        Shader loadShader(const char* vsFileName, const char* fsFileName);
        void unloadShader(Shader s);
        int getShaderLocation(Shader s, const char* uniformName);
        void setShaderValue(Shader s, int uniformLoc, const void* value, int uniformType);

        // Aux
        Mesh genMeshCube(float width, float height, float lenght);
        Model loadModelFromMesh(Mesh m);
        Model loadModel(const char* filename);
        Texture2D loadTexture(const char* filename);
        void updateTexture(Texture2D texture, const void* data);
        void unloadMesh(Mesh m);
        void unloadModel(Model m);
        float getWorldToScreenX(vec3d pos);
        float getWorldToScreenY(vec3d pos);
        RayCast getMouseRay();
        void loadAndResizeImage(const std::string& name, const std::string& path);
        void loadAndResizeImageGif(const std::string& name, const std::string& path, int frames, int delay = 15, double reScaleX = 2.0, double reScaleY = 2.0);
        void updateGif(Gif& anim);
        void unloadGifs();

        std::map<std::string, Texture2D> textures;
        std::map<std::string, Gif> gifs;

    private:
        u16 const width_{}, height_{};
        Camera3D camera{};
    };

#endif // !GAME_ENGINE

}