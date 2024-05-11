#pragma once
#include <cstdint>
#include "../utils/vec3D.hpp"
#include "../components/type_component.hpp"

enum AttackType
{
    None = 0x00,
    MeleeEnemy = 0x01,
    RangedEnemy = 0x02,
    AreaCrusher = 0x04,
    MeleePlayer = 0x08,
    HealSpellSetup = 0x10,
    HealSpell = 0x20,
    AreaAttack = 0x40,
    Spiderweb = 0x80,
    GollemAttack = 0x100,
    AirAttack = 0x200,
    CrusherAttack = 0x400,
    WaterBomb = 0x800,
    FireBall = 0x1000,
    FireBallShot = 0x2000,
    WaterBombShot = 0x4000,
    IceShield = 0x8000,
    SpiderShot = 0x10000,
    TripleShot = 0x20000,
    WaterDashArea = 0x40000,
    MeteoritePlayer = 0x80000,
    IceShard = 0x100000,
    SnowmanBall = 0x200000
};

struct AttackComponent
{
    AttackType atkType{ AttackType::None };
    uint16_t damage{};
    ElementalType type{ ElementalType::Neutral };
    float lifeTime{ 0.5f };

    bool doEffect{ false };
    std::vector<std::size_t> targets{};

    float resolveType(ElementalType t)
    {
        if (t == ElementalType::Neutral || type == t)
            return 1.f;
        else if ((type == ElementalType::Fire && t == ElementalType::Water)
            || (type == ElementalType::Water && t == ElementalType::Ice)
            || (type == ElementalType::Ice && t == ElementalType::Fire))
            return .5f;
        else if ((type == ElementalType::Fire && t == ElementalType::Ice)
            || (type == ElementalType::Water && t == ElementalType::Fire)
            || (type == ElementalType::Ice && t == ElementalType::Water))
            return 2.f;

        return 1.f;
    }
};

struct AttackerComponent
{
    AttackType type{ AttackType::None };
    float countdown{ 0.5f }, elapsed{ countdown };
    bool createAttack{ false };
    vec3d vel{};

    void attack(AttackType typeAttack) {
        if (isAttackReady()) {
            createAttack = true;
            elapsed = 0;
            type = typeAttack;
        }
    }

    bool isAttackReady() {
        return elapsed >= countdown;
    }

    void decreaseCountdown(float deltaTime, float& elapsed) { elapsed += deltaTime; }; // delta time
};
