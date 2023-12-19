#pragma once

enum struct Object_type { Life, Sword, Bomb, Coin, BombExplode, Coin30, ShopItem_Bomb, ShopItem_ExtraLife, ShopItem_Life, Key };

struct ObjectComponent
{
    Object_type type{ Object_type::Coin };
    float life_time{ 10.0f }, elapsed{ 0.0f };
    bool active{ false };

    bool inmortal{ false };

    bool decreaseLifeTime(float deltaTime) {
        elapsed += deltaTime;
        return elapsed >= life_time ? true : false;
    }

    void effect() { active = true; }
};