#pragma once

enum struct ObjectType { None, Life, Sword, Bomb, Coin, BombExplode, Coin30, ShopItem_Bomb, ShopItem_ExtraLife, ShopItem_Life, Key, Heal_Spell, Mana_Potion, AreaAttack, Spiderweb };

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

    uint8_t mapID{ 255 };
    uint8_t objID{ 255 };

    void effect() { active = true; }
};
