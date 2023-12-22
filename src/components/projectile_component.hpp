#pragma once

struct ProjectileComponent {
    float range{ 3.0f }, elapsed{ 0.0f }; // en segundos

    bool checkRange(float deltaTime) {
        elapsed += deltaTime;
        return elapsed >= range ? true : false;
    };
};