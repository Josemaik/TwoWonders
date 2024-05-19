#pragma once

enum struct ObjectType
{
    None,
    Life,
    Sword,
    Bomb,
    Coin,
    Coin30,
    ShopItem_ExtraLife,
    ShopItem_Life,
    Key,
    Mana_Potion,
    AreaAttack,
    Ice_Spell,
    Water_Spell,
    Basic_Staff,
    GoodBoots,
    RegularHat,
    AttackUpgrade,
    WarningZone
};

struct ObjectComponent
{
    ObjectType type{ ObjectType::Coin };
    float life_time{ 10.0f }, elapsed{ 0.0f };
    bool active{ false };

    bool inmortal{ false };

    bool decreaseLifeTime(float deltaTime) {
        elapsed += deltaTime;
        return elapsed >= life_time ? true : false;
    }

    // ID del objeto para que no vuelva a cargar cuando se recargue el nivel
    uint8_t objID{ 255 };

    void effect() { active = true; }
};
