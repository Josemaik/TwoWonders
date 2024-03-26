#pragma once

struct LifeComponent
{
    int life{ 1 }, maxLife{ life }, life_width{}, lifeLost{ 0 };
    float countdown{ 0.5 }, elapsed{ countdown }; // En segundos
    bool markedForDeletion{ false }, invulnerable{ false };

    void decreaseLife(); // decrease 1 life
    void decreaseLife(int minusLife);

    void increaseLife(int i = 2);
    void increaseMaxLife();
    void decreaseCountdown(float deltaTime) { elapsed += deltaTime; };
    bool vidaMax() { return (life == maxLife); };
    bool decreaseNextFrame{ false };
};