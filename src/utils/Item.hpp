#pragma once
#include <string>
#include "../components/type_component.hpp"

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

struct Spell : public Item
{
    Spell(std::string name, std::string description, Spells spell, double cost, uint16_t damage)
        : Item{ name, description }, spell{ spell }, cost{ cost }, damage{ damage }
    {
        switch (spell)
        {
        case Spells::FireBall:
            type = ElementalType::Fire;
            break;
        case Spells::FireMeteorites:
            type = ElementalType::Fire;
            break;
        case Spells::IceShards:
            type = ElementalType::Ice;
            break;
        case Spells::IceShield:
            type = ElementalType::Ice;
            break;
        case Spells::WaterBomb:
            type = ElementalType::Water;
            break;
        case Spells::WaterDash:
            type = ElementalType::Water;
            break;
        default:
            break;
        }
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