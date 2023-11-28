#pragma once
#ifndef GAME_ENGINE
#define GAME_ENGINE
#include <cstdint>
#include <iostream>
#include "../utils/types.hpp"

namespace ENGI {

    struct GameEngine
    {
        using CL = MP::TypeList<PhysicsComponent, RenderComponent>;
        using TL = MP::TypeList<>;

        using u16 = std::uint16_t;

        GameEngine(u16 const width, u16 const height);

        void beginFrame();
        void drawAll(EntityManager& em);
        void endFrame();
        void endWindow();
        bool run();

    private:
        u16 const width_{}, height_{};
        Camera3D camera{};
    };

#endif // !GAME_ENGINE

}