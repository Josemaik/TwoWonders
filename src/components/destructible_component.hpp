#pragma once
#include <vector>
#include "type_component.hpp"

struct DestructibleComponent
{
    bool checkIfDamaged(ElementalType type) const
    {
        for (auto& t : weaknesses)
        {
            if (t == type)
            {
                return true;
            }
        }
        return false;
    }

    void addWeakness(ElementalType type)
    {
        bool alreadyExists = false;
        for (auto& t : weaknesses)
        {
            if (t == type)
            {
                alreadyExists = true;
                break;
            }
        }
        if (!alreadyExists)
        {
            weaknesses.push_back(type);
        }
    }

private:
    std::vector<ElementalType> weaknesses;
};