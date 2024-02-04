#pragma once
#ifndef GAME_ENGINE
#define GAME_ENGINE
#include <cstdint>
#include <iostream>
#include "utils/types.hpp"
#include "utils/vec3D.hpp"

namespace ENGI {

    struct GameEngine
    {
        using CL = MP::TypeList<PhysicsComponent, RenderComponent>;
        using TL = MP::TypeList<>;

        using u16 = std::uint16_t;

        GameEngine(u16 const width, u16 const height);
        Texture2D texture_logo_two_wonders, texture_logo_kaiwa_games;

        // Timing Related Functions
        void setTargetFPS(int fps);
        float getFrameTime();

        // Image and Texture
        Image loadImage(const char* filename);
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
        void drawRectangle(int posX, int posY, int width, int height, Color color);
        void drawTexture(Texture2D texture, int posX, int posY, Color tint);

        // Text
        void drawText(const char* text, int posX, int posY, int fontSize, Color color);

        // Window
        void initWindow(int width, int height, const char* title);
        void closeWindow();
        bool windowShouldClose();
        int getScreenWidth();
        int getScreenHeight();

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

        // Aux
        Mesh genMeshCube(float width, float height, float lenght);
        Model loadModelFromMesh(Mesh m);
        void unloadMesh(Mesh m);
        void unloadModel(Model m);
        float getWorldToScreenX(vec3d pos);
        float getWorldToScreenY(vec3d pos);
        RayCast getMouseRay();

    private:
        u16 const width_{}, height_{};
        Camera3D camera{};
    };

#endif // !GAME_ENGINE

}