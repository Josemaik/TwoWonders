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
    AirAttack = 0x200,
    CrusherAttack = 0x400,
    WaterBomb = 0x800,
    FireBall = 0x1000,
};

struct AttackComponent
{
    AttackType type{ AttackType::Ranged };
    u_int16_t damage{};
    float range{}; // en segundos
    double scale_to_respawn_attack{ 2.0 };
    float countdown{ 1.0f }, elapsed{ 1.0f }, countdown_air_attk{ 0.2f }, elapsed_air_attk{ 1.0f },
        countdown_warning_airatk{ 1.5 }, elapsed_warning_airatk{ 1.0 }; // en segundos
    vec3d vel{};
    //air attack
    bool warning_created{ false };
    uint16_t air_attack_fases{ 4 };
    bool createAttack{ false };
    vec3d pos_respawn_air_attack{}, pos_respawn_crush_attack{};

    void attack(AttackType typeAttack) {
        if (elapsed >= countdown) {
            createAttack = true;
            elapsed = 0;
            type = typeAttack;
        }
    }

    void decreaseCountdown(float deltaTime, float& elapsed) { elapsed += deltaTime; }; // delta time
};
