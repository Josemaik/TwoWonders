#pragma once

enum struct AttackType { Melee, Ranged, Bomb, AttackPlayer, TripleShot};

struct AttackComponent
{
    AttackType type{ AttackType::Ranged };
    u_int16_t damage{};
    float range{}; // en segundos
    bool createAttack{ false };
    float countdown{ 1.0f }, elapsed{ 1.0f }; // en segundos
    vec3f vel{};

    void attack(AttackType typeAttack) {
        if (elapsed >= countdown) {
            createAttack = true;
            elapsed = 0;
            type = typeAttack;
        }
    }

    void decreaseCountdown(float deltaTime) { elapsed += deltaTime; }; // delta time
};