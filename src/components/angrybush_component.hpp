#pragma once

struct AngryBushComponent
{
    bool move{ true }, chargeAttack{ false };
    vec3d dir{}, vel{};
    double max_speed{ 0.4 };
    bool angrySound { false };
    bool angrySoundOneTime { true };
};