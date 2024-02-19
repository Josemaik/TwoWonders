#pragma once 

struct InputInfo
{
    // Teclas de movimiento
    bool up{ false };
    bool down{ false };
    bool left{ false };
    bool right{ false };

    // Teclas de acción
    bool space{ false };
    bool enter{ false };
    bool pause{ false };
    bool inventory{ false };

    // Teclas de debug
    bool debugPhy{ false };
    bool debugAI1{ false };
    bool debugAI2{ false };

    // Teclas de mecánicas
    bool lockOn{ false };
    bool interact{ false };
};