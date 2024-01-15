#pragma once

enum struct ElementalType { Neutral, Agua, Fuego, Hielo };

struct TypeComponent
{
    ElementalType type{ ElementalType::Neutral };

    void changeType(){
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

    void changeType(ElementalType newType){
        type = newType;
    }
};