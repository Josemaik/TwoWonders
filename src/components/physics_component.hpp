#pragma once
#include "utils/vec3D.hpp"

struct PhysicsComponent
{
    static constexpr double KGravity{ 1.0 };
    static constexpr double KMinVy{ -0.5 };
    static constexpr double KMaxVy{ 0.5 };
    static constexpr double MAX_SPEED{ 0.5 };

    vec3d position{};
    vec3d velocity{};

    double gravity{ KGravity };
    bool alreadyGrounded{ false };
    //Flag para indicar que estas subiendo escaleras( solo eje y activado)
    bool blockXZ{ false };
    //Velocidad lineal
    double v_linear{ 0.0 };
    //Velocidad angular
    double v_angular{ 0.0 };
    double orientation{ 0.0 }; //ángulo del vector con respecto al eje de origen
    //vector de rotacion
    vec3d rotationVec{ 0.0, 0.1, 0.0 };
    //aceleracion lineal
    double a_linear{ 0.0 };
    //aceleracion angular
    double a_angular{ 0.0 };
    // velocidad máxima
    double max_speed{ MAX_SPEED };
    //max valor que puede tomar las velocidades y aceleraciones
    static constexpr double kMaxVLin{ 5.0 };
    static constexpr double kMaxAlin{ kMaxVLin / 0.5 };
    static constexpr double kMaxVAng{ 3 * PI };
    static constexpr double kMaxAAng{ kMaxVAng / 5.0 };
    //controla la llegada , se ha sustituido por arrivalradius en la ia
    static constexpr double kEpsilon{ 0.1 };
    //rozamiento
    bool dragactivated { false };
    static constexpr double kDrag{ 1.20 };

};
