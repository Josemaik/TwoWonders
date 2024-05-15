#pragma once
#include <string>
#include <map>
#include "../components/attack_component.hpp"

inline static std::size_t itemID{ 0 };

struct Item
{
    Item(std::string name, std::string description)
        : name{ name }, description{ description }
    { }

    virtual ~Item() = default;

    std::string name;
    std::string description;

    std::size_t getID() const { return id; }
private:
    std::size_t id{ itemID++ };
};

enum struct Spells : uint8_t
{
    None,
    FireBall,
    FireMeteorites,
    IceShards,
    IceShield,
    WaterBomb,
    WaterDash,
};

static std::map<AttackType, double> spellType = {
    { AttackType::None, 0 },
    { AttackType::FireBallShot, 20 },
    { AttackType::MeteoritePlayer, 20 },
    { AttackType::IceShard, 20 },
    { AttackType::IceShield, 20 },
    { AttackType::WaterBombShot, 20 },
    { AttackType::WaterDashArea, 20 },
};

struct Spell : public Item
{

    Spell(std::string name, std::string description, AttackType atkType)
        : Item{ name, description }, atkType{ atkType }
    {
        cost = spellType[atkType];
    }

    ~Spell() override = default;

    bool operator==(const Spell& other) const
    {
        return atkType == other.atkType;
    }

    bool operator==(const AttackType& other) const
    {
        return atkType == other;
    }

    AttackType atkType{};
    double cost{};
};

struct Staff : public Item
{
    Staff(std::string name, std::string description, ElementalType type, double damage)
        : Item{ name, description }, type{ type }, damage{ damage }
    { }

    ~Staff() override = default;

    ElementalType type;
    double damage{};
};

enum struct PotionType : uint8_t
{
    Health,
    Mana,
};

struct Potion : public Item
{
    Potion(std::string name, std::string description, PotionType type, double value)
        : Item{ name, description }, type{ type }, value{ value }
    { }

    ~Potion() override = default;

    PotionType type;
    double value{};
};