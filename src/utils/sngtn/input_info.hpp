#pragma once 

struct InputInfo
{
    void setAttackFalse()
    {
        melee = false;
        spell1 = false;
        spell2 = false;
        spell3 = false;
    }

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
    bool pathfind{ false };

    // Teclas de mecánicas
    bool lockOn{ false };
    bool interact{ false };

    // Bools de ataque
    bool melee{ false };
    bool spell1{ false };
    bool spell2{ false };
    bool spell3{ false };

    // Menú
    std::size_t currentButton{ 0 };
    bool mouseClick{ false };
};