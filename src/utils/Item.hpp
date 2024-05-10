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

static std::map<Spells, std::pair<ElementalType, AttackType>> spellType
{
    { Spells::FireBall, {ElementalType::Fire, AttackType::FireBallShot } },
    { Spells::FireMeteorites, {ElementalType::Fire, AttackType::MeteoritePlayer } },
    { Spells::IceShards, {ElementalType::Ice, AttackType::IceShard } },
    { Spells::IceShield, {ElementalType::Ice, AttackType::MeleePlayer } },
    { Spells::WaterBomb, {ElementalType::Water, AttackType::WaterBombShot } },
    { Spells::WaterDash, {ElementalType::Water, AttackType::WaterDashArea } },
};

struct Spell : public Item
{

    Spell(std::string name, std::string description, Spells spell, double cost, uint16_t damage)
        : Item{ name, description }, spell{ spell }, cost{ cost }, damage{ damage }
    {
        type = spellType[spell].first;
        atkType = spellType[spell].second;
    }

    ~Spell() override = default;

    bool operator==(const Spell& other) const
    {
        return spell == other.spell;
    }

    bool operator==(const Spells& other) const
    {
        return spell == other;
    }
    Spells spell{};
    AttackType atkType{};
    ElementalType type{};
    double cost{};
    uint16_t damage{};
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