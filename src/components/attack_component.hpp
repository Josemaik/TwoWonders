#pragma once

struct AttackComponent
{
    enum struct AttackType { Melee, Ranged };
    AttackType type;
    u_int16_t damage{};
    float range{};
    bool createAttack{ false };
    float countdown{ 1.0f }, elapsed{ 1.0f };

    void attack(){
        if(elapsed >= countdown){
            createAttack = true;
            elapsed = 0;
        }
    }

    void decreaseCountdown(){ elapsed += GetFrameTime(); }; // delta time
};