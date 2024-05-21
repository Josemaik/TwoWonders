#pragma once

struct LifeComponent
{
    void decreaseLife(); // decrease 1 life
    void decreaseLife(int minusLife);

    void increaseLife(int i = 2);
    void increaseMaxLife();
    void decreaseCountdown(float deltaTime) { elapsed += deltaTime; };
    bool vidaMax() { return (life == maxLife + maxLifeAdd); };
    bool decreaseNextFrame{ false };

    int life{ 1 }, maxLife{ life }, lifeLost{ 0 }, maxLifeAdd{ 0 };
    float countdown{ 0.5 }, elapsed{ countdown }, life_width{}; // En segundos
    bool markedForDeletion{ false }, invulnerable{ false }, onDeathAnim{ false };
};