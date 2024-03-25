#pragma once

struct LifeComponent
{
    int life{ 1 }, maxLife{ life }, life_width{}, lifeLost{ 0 };
    double countdown{ 1.0 }, elapsed{ 1.0 }; // En segundos
    bool markedForDeletion{ false }, invulnerable{ false };

    void decreaseLife(); // decrease 1 life
    void decreaseLife(int minusLife);

    void increaseLife(int i = 2);
    void increaseMaxLife();
    void decreaseCountdown(double deltaTime) { elapsed += deltaTime; };
    bool vidaMax() { return (life == maxLife); };
    bool decreaseNextFrame{ false };
};