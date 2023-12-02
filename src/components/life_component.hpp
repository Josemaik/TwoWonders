#pragma once

struct LifeComponent
{
    int life{};
    float countdown{ 1.0f }, elapsed{ 1.0f }; // En segundos

    void decreaseLife();
    void decreaseCountdown(){ elapsed += GetFrameTime(); }; // delta time
};