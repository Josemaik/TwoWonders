#pragma once

struct ProjectileComponent {
    float range{ .5 }, elapsed{ 0.0 }; // en segundos
    bool startedFalling{ false };

    bool checkRange(float deltaTime) {
        elapsed += deltaTime;
        return elapsed >= range ? true : false;
    };
};