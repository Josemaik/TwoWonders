#pragma once
#include <array>

enum struct ElementalType : uint8_t
{
    Neutral = 1,
    Water = 2,
    Fire = 3,
    Ice = 4
};

struct TypeComponent
{
    void addType(ElementalType newType)
    {
        for (auto& t : types)
        {
            if (t == ElementalType::Neutral)
            {
                t = newType;
                changeType();
                break;
            }
        }
    }

    void changeType()
    {
        // Buscamos el índice del tipo actual en el array
        auto currentTypeIt = std::find(types.begin(), types.end(), type);

        // Si el tipo actual no está en el array, lo cambiamos a Neutral
        if (currentTypeIt == types.end()) {
            type = ElementalType::Neutral;
            return;
        }

        // Buscamos el siguiente tipo distinto en el array
        auto nextTypeIt = std::find_if(currentTypeIt + 1, types.end(), [this](ElementalType t) {
            return t != type;
        });

        // Si encontramos un siguiente tipo distinto, lo establecemos como el tipo actual
        // Si no, volvemos al principio del array y buscamos el primer tipo distinto
        if (nextTypeIt != types.end()) {
            type = *nextTypeIt;
        }
        else {
            nextTypeIt = std::find_if(types.begin(), types.end(), [this](ElementalType t) {
                return t != type;
            });
            type = (nextTypeIt != types.end()) ? *nextTypeIt : ElementalType::Neutral;
        }
    }

    bool hasType(ElementalType typeToCheck) const
    {
        for (auto& t : types)
        {
            if (t == typeToCheck)
            {
                return true;
            }
        }
        return false;
    }

    ElementalType type{ ElementalType::Neutral };
    std::array<ElementalType, 4> types = { ElementalType::Neutral, ElementalType::Neutral, ElementalType::Neutral, ElementalType::Neutral };
};