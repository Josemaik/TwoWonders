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
        void drawCube(vec3f pos, float width, float height, float lenght, Color color);
        void drawCubeWires(vec3f pos, float width, float height, float lenght, Color color);
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
        void setPositionCamera(vec3f pos);
        void setTargetCamera(vec3f tar);
        void setUpCamera(vec3f up);
        void setFovyCamera(float fovy);
        void setProjectionCamera(int proj);

        // Aux 
        float getWorldToScreenX(vec3f pos);
        float getWorldToScreenY(vec3f pos);
        RayCast getMouseRay();

    private:
        u16 const width_{}, height_{};
        Camera3D camera{};
    };

#endif // !GAME_ENGINE

}