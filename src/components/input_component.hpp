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

    int spell1{ KEY_J };
    int spell2{ KEY_K };
    int spell3{ KEY_L };

    // Teclas de mecánicas
    int air_attack{ KEY_T };
    int lockIn{ KEY_Q };
    int interact{ KEY_E };

    // Teclas de debug
    int debugPhy{ KEY_F1 };
    int debugIA1{ KEY_F2 };
    int debugIA2{ KEY_F3 };
    int pathfinfing{ KEY_F4 };

    // Botones de Mando
    int m_up{ GAMEPAD_BUTTON_LEFT_FACE_UP };
    int m_down{ GAMEPAD_BUTTON_LEFT_FACE_DOWN };
    int m_left{ GAMEPAD_BUTTON_LEFT_FACE_LEFT };
    int m_right{ GAMEPAD_BUTTON_LEFT_FACE_RIGHT };

    int m_interact{ GAMEPAD_BUTTON_RIGHT_FACE_DOWN };
    int m_space{ GAMEPAD_AXIS_RIGHT_TRIGGER };
    int m_lockIn{ GAMEPAD_AXIS_LEFT_TRIGGER };

    int m_spell1{ GAMEPAD_BUTTON_RIGHT_FACE_LEFT };
    int m_spell2{ GAMEPAD_BUTTON_RIGHT_FACE_RIGHT };
    int m_spell3{ GAMEPAD_BUTTON_RIGHT_FACE_UP };

    // Joysticks
    float m_joystickX{ 0 };
    float m_joystickY{ 0 };
};
