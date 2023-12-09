#pragma once

enum struct Object_type { Life, Sword, Bomb, Coin, BombExplode };

struct ObjectComponent
{
    Object_type type{ Object_type::Coin };
    float life_time{ 10.0f }, elapsed{ 0.0f };
    bool active{ false };

    bool decreaseLifeTime(float deltaTime){ 
        elapsed += deltaTime; 
        return elapsed >= life_time ? true : false;
    }

    void effect(){ active = true; }
};