#pragma once
#include <string>
#include "../components/type_component.hpp"

struct Item
{
    std::string name;
    std::string description;
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