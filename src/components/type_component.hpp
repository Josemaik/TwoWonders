#pragma once

enum ElementalType
{
    Neutral = 0x01,
    Agua = 0x02,
    Fuego = 0x04,
    Hielo = 0x08
};

struct TypeComponent
{
    ElementalType type{ ElementalType::Neutral };

    void changeType() {
        switch (type)
        {
        case ElementalType::Neutral:
            type = ElementalType::Agua;
            break;

        case ElementalType::Agua:
            type = ElementalType::Fuego;
            break;

        case ElementalType::Fuego:
            type = ElementalType::Hielo;
            break;

        case ElementalType::Hielo:
            type = ElementalType::Neutral;
            break;

        default:
            break;
        }
    }

    void changeType(ElementalType newType) {
        type = newType;
    }
};