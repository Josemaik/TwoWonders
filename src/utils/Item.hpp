#pragma once
#include <string>
#include "../components/type_component.hpp"

inline static std::size_t itemID{ 0 };

struct Item
{
    Item(std::string name, std::string description)
        : name{ name }, description{ description }
    { }
    std::string name;
    std::string description;

    std::size_t getID() const { return id; }
private:
    std::size_t id{ itemID++ };
};

enum struct Spells : uint8_t
{
    None,
    Fire1,
    Fire2,
    Ice1,
    Ice2,
    Water1,
    Water2,
};

struct Spell : public Item
{
    Spell(std::string name, std::string description, Spells spell, ElementalType type, double cost, double damage)
        : Item{ name, description }, spell{ spell }, type{ type }, cost{ cost }, damage{ damage }
    { }

    bool operator==(const Spell& other) const
    {
        return spell == other.spell;
    }

    bool operator==(const Spells& other) const
    {
        return spell == other;
    }
    Spells spell;
    ElementalType type;
    double cost{};
    double damage{};
};

struct Staff : public Item
{
    ElementalType type;
    double damage{};
};

struct Potion : public Item
{
    bool isHealthPotion{ true };
    double value{};
};