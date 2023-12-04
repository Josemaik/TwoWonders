#pragma once
#include "raylib.h"

struct LifeComponent
{
    int life{};
    float countdown{ 1.0f }, elapsed{ 1.0f }; // En segundos
    bool decreaseLifeNextFrame{ false };

    void decreaseLife();
    void decreaseCountdown() { elapsed += GetFrameTime(); }; // delta time
};