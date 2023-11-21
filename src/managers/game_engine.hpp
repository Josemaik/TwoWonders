#pragma once
#ifndef GAME_ENGINE
#define GAME_ENGINE
#include <cstdint>
#include <iostream>
#include "./entity_manager.hpp"
#include "../entities/entity.hpp"

struct GameEngine
{
    using u16 = std::uint16_t;

    GameEngine(u16 const width, u16 const height);

    void beginFrame();
    void drawAll(EntityManager<PhysicsComponent, RenderComponent, Entity>& em);
    void endFrame();
    void endWindow();
    bool run();

private:
    u16 const width_{}, height_{};
    Camera3D camera{};
};

#endif // !GAME_ENGINE