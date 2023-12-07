#pragma once

struct LifeComponent
{
    int life{ 1 }, maxLife{ 6 };
    float countdown{ 1.0f }, elapsed{ 1.0f }; // En segundos
    bool decreaseLifeNextFrame{ false };

    void decreaseLife();
    void increaseLife();
    void decreaseCountdown(float deltaTime) { elapsed += deltaTime; };
    bool vidaMax(){ return (life == maxLife); };
};