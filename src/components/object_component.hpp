#pragma once

enum struct ObjectType
{
    None,
    Life,
    Sword,
    Bomb,
    Coin,
    BombExplode,
    Coin30,
    ShopItem_Bomb,
    ShopItem_ExtraLife,
    ShopItem_Life,
    Key,
    Heal_Spell,
    Mana_Potion,
    AreaAttack,
    Spiderweb,
    Meteorit,
    Fire_Spell,
    Ice_Spell,
    Water_Spell,
    Basic_Staff,
    CrusherAttack
};

struct ObjectComponent
{
    ObjectType type{ ObjectType::Coin };
    double life_time{ 10.0 }, elapsed{ 0.0 };
    bool active{ false };

    bool inmortal{ false };

    bool decreaseLifeTime(double deltaTime) {
        elapsed += deltaTime;
        return elapsed >= life_time ? true : false;
    }

    // ID del objeto para que no vuelva a cargar cuando se recargue el nivel
    uint8_t objID{ 255 };

    void effect() { active = true; }
};
