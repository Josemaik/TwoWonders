#pragma once
#include <raylib.h>

// Sujeto a cambios cuando dejemos de usar Raylib
struct InputComponent
{
    // Teclas de movimiento
    int up{ KEY_W };
    int down{ KEY_S };
    int left{ KEY_A };
    int right{ KEY_D };

    // Teclas de acción
    int space{ KEY_SPACE };
    int enter{ KEY_ENTER };
    int pause{ KEY_ESCAPE };
    int inventory{ KEY_I };

    // Teclas de mecánicas
    int air_attack{ KEY_T };
    int lockIn{ KEY_F };
    int interact{ KEY_E };

    // Teclas de debug
    int debugPhy{ KEY_F1 };
    int debugIA1{ KEY_F2 };
    int debugIA2{ KEY_F3 };
};
