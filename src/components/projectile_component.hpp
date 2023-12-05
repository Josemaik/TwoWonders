#pragma once

struct ProjectileComponent{
    float range{ 3.0f }, elapsed{ 0.0f }; // en segundos

    bool checkRange() { 
        elapsed += GetFrameTime(); // delta time
        return elapsed >= range ? true : false;
    };
};