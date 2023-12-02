#pragma once

struct LifeComponent
{
    int life{};
    float countdown{ 1.0f }, elapsed{ 1.0f }; // En segundos

    void decreaseLife(){
        if((elapsed >= countdown) && (life > 0)){
            life -= 1;
            elapsed = 0;
        }
    }

    void decreaseCountdown(){
        elapsed += GetFrameTime(); // delta time
    }
};