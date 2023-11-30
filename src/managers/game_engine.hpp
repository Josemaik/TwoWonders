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
        // using CL = MP::TypeList<PhysicsComponent, RenderComponent>;
        // using TL = MP::TypeList<>;

        using u16 = std::uint16_t;

        GameEngine(u16 const width, u16 const height);

        // Drawing
        void beginDrawing();
        void clearBackground(Color color);
        void drawGrid(int slices, float spacing);
        void endDrawing();
        void beginMode3D();
        void endMode3D();
        void drawCube(vec3f pos, float width, float height, float lenght, Color color);
        void drawCubeWires(vec3f pos, float width, float height, float lenght, Color color);

        // Window
        void initWindow(int width, int height, const char *title);
        void closeWindow();
        void setTargetFPS(int fps);
        bool windowShouldClose();
        int getScreenWidth();
        int getScreenHeight();

        // Camera
        void setPositionCamera(vec3f pos);
        void setTargetCamera(vec3f tar);
        void setUpCamera(vec3f up);
        void setFovyCamera(float fovy);
        void setProjectionCamera(int proj);

    private:
        u16 const width_{}, height_{};
        Camera3D camera{};
    };

#endif // !GAME_ENGINE

}