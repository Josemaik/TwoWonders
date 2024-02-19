#pragma once

struct SubjectComponent
{
    void setShield(bool active) { activeShield = active; }
    void decreaseShield() { shieldLife--; }

    bool activeShield{ false };
    int shieldLife{ 2 }, shieldLifeWidth{}, maxShieldLife{ shieldLife };
};