#pragma once
#include "raylib.h"

struct LifeComponent
{
    int life{}, maxLife{ 3 };
    float countdown{ 1.0f }, elapsed{ 1.0f }; // En segundos
    bool decreaseLifeNextFrame{ false };

    void decreaseLife();
    void increaseLife();
    void decreaseCountdown() { elapsed += GetFrameTime(); }; // delta time
};