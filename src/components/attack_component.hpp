#pragma once

enum AttackType
{
    None = 0x00,
    Melee = 0x01,
    Ranged = 0x02,
    Bomb = 0x04,
    AttackPlayer = 0x08,
    TripleShot = 0x10,
    HealSpell = 0x20,
    AreaAttack = 0x40,
    Spiderweb = 0x80,
    GollemAttack = 0x100,
    AirAttack = 0x200
};

struct AttackComponent
{
    AttackType type{ AttackType::Ranged };
    u_int16_t damage{};
    float range{}; // en segundos
    double scale_to_respawn_attack{ 2.0 };
    bool createAttack{ false };
    float countdown{ 1.0f }, elapsed{ 1.0f }; // en segundos
    vec3d vel{};
    uint16_t air_attack_fases{4};

    void attack(AttackType typeAttack) {
        if (elapsed >= countdown) {
            createAttack = true;
            elapsed = 0;
            type = typeAttack;
        }
    }

    void decreaseCountdown(float deltaTime) { elapsed += deltaTime; }; // delta time
};
