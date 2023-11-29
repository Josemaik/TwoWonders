#pragma once
#ifndef GAME_ENGINE
#define GAME_ENGINE
#include <cstdint>
#include <iostream>
#include "../utils/types.hpp"

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
        void drawCube(Vector3 pos, float width, float height, float lenght, Color color);
        void drawCubeWires(Vector3 pos, float width, float height, float lenght, Color color);

        // Window
        void initWindow(int width, int height, const char *title);
        void closeWindow();
        void setTargetFPS(int fps);
        bool windowShouldClose();

        // Camera
        void setPositionCamera(Vector3 pos);
        void setTargetCamera(Vector3 tar);
        void setUpCamera(Vector3 up);
        void setFovyCamera(float fovy);
        void setProjectionCamera(int proj);

    private:
        u16 const width_{}, height_{};
        Camera3D camera{};
    };

#endif // !GAME_ENGINE

}