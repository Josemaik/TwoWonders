#pragma once

struct ProjectileComponent {
    float range{ .5f }, elapsed{ 0.0f }; // en segundos
    bool startedFalling{ false };

    bool checkRange(float deltaTime) {
        elapsed += deltaTime;
        return elapsed >= range ? true : false;
    };
};