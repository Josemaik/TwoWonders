#pragma once
#include "utils/vec3D.hpp"

struct PhysicsComponent
{
    static constexpr double KGravity{ 1.0 };
    static constexpr double KMinVy{ -0.6 };
    static constexpr double KMaxVy{ 0.6 };
    static constexpr double MAX_SPEED{ 0.6 };

    vec3d position{};
    vec3d velocity{};
    
    double gravity{ KGravity };
    bool alreadyGrounded{ false };

    //IA
    double v_linear  { 0.0 };
    double v_angular { 0.0 };
    double orientation { 0.0 }; //ángulo del vector con respecto al eje de origen
    static constexpr double kMaxVLin { 3 };
    static constexpr double kMaxVAng { 3*PI };
    static constexpr double kEpsilon { 0.1 };
};
