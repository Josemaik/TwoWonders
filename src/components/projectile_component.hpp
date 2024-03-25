#pragma once

struct ProjectileComponent {
    double range{ .5 }, elapsed{ 0.0 }; // en segundos
    bool startedFalling{ false };

    bool checkRange(double deltaTime) {
        elapsed += deltaTime;
        return elapsed >= range ? true : false;
    };
};