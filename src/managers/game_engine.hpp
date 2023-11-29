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

        void beginDrawing();
        void clearBackground(Color color);
        void drawGrid(int slices, float spacing);
        void endDrawing();
        void closeWindow();

        bool windowShouldClose();

        void beginMode3D();
        void endMode3D();

        // Camera
        void setPositionCamera(Vector3 pos);
        void setTargetCamera(Vector3 tar);
        void setUpCamera(Vector3 up);
        void setFovyCamera(float fovy);
        void setProjectionCamera(int proj);

        // void drawAll(EntityManager& em);

    private:
        u16 const width_{}, height_{};
        Camera3D camera{};
    };

#endif // !GAME_ENGINE

}