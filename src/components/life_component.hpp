#pragma once

struct LifeComponent
{
    int life{ 1 }, maxLife{ 6 };
    float countdown{ 1.0f }, elapsed{ 1.0f }; // En segundos
    bool markedForDeletion{ false };

    void decreaseLife(); // decrease 1 life
    void decreaseLife(int minusLife);

    void increaseLife();
    void increaseMaxLife();
    void decreaseCountdown(float deltaTime) { elapsed += deltaTime; };
    bool vidaMax() { return (life == maxLife); };
    bool decreaseNextFrame{ false };
};