#pragma once
#include "utils/vec3D.hpp"

struct PreviousPhysicsState
{
    vec3d position{};
    vec3d velocity{};
    vec3d scale{};
    double orientation{ 0.0 };
};

struct PhysicsComponent
{
    static constexpr double KGravity{ 1.0 };
    static constexpr double KMinVy{ -0.5 };
    static constexpr double KMaxVy{ 0.5 };
    static constexpr double MAX_SPEED{ 2.0 };

    vec3d position{};
    vec3d velocity{};
    vec3d scale{};

    PreviousPhysicsState previousState{};

    double gravity{ KGravity };
    bool alreadyGrounded{ false };

    // Ángulo del vector con respecto al eje de origen
    double orientation{ 0.0 };
    //vector de rotacion
    vec3d rotationVec{ 0.0, 0.1, 0.0 };

    // velocidad máxima
    double max_speed{ MAX_SPEED };
    //max valor que puede tomar las velocidades y aceleraciones
    static constexpr double kMaxVLin{ 5.0 };

    //rozamiento por ataque en area
    bool dragActivated{ false };
    //rozamiento por ataque melee de golem
    bool dragActivatedTime{ false };
    double countdown_sttuned{ 6.0 }; // seconds
    double elapsed_stunned{ 1.0 };

    bool stopped{ false };
    double countdown_stopped{ 0.5 }; // seconds
    double elapsed_stopped{ 0.0 };

    void plusdeltatime(double deltaTime, double& elapsed) { elapsed += deltaTime; };

    static constexpr double kDrag{ 3.0 };
};
