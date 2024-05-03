#pragma once
#include "../motor/src/darkmoon.hpp"

// Sujeto a cambios cuando dejemos de usar Raylib
struct InputComponent
{
    // Teclas de movimiento
    int up{ D_KEY_W };
    int down{ D_KEY_S };
    int left{ D_KEY_A };
    int right{ D_KEY_D };

    // Teclas de acción
    int space{ D_KEY_SPACE };
    int enter{ D_KEY_ENTER };
    int pause{ D_KEY_ESCAPE };
    int inventory{ D_KEY_I };

    int spell1{ D_KEY_J };
    int spell2{ D_KEY_K };
    int spell3{ D_KEY_L };

    // Teclas de mecánicas
    int air_attack{ D_KEY_T };
    int lockIn{ D_KEY_Q };
    int interact{ D_KEY_E };

    // Teclas de debug
    int debugPhy{ D_KEY_F1 };
    int debugIA1{ D_KEY_F2 };
    int debugIA2{ D_KEY_F3 };
    int pathfinfing{ D_KEY_F4 };

    // Botones de Mando
    int m_up{ GLFW_GAMEPAD_BUTTON_DPAD_UP };
    int m_down{ GLFW_GAMEPAD_BUTTON_DPAD_DOWN };
    int m_left{ GLFW_GAMEPAD_BUTTON_DPAD_LEFT };
    int m_right{ GLFW_GAMEPAD_BUTTON_DPAD_RIGHT };

    int m_interact{ GLFW_GAMEPAD_BUTTON_A };
    int m_space{ D_GAMEPAD_AXIS_RIGHT_TRIGGER };
    int m_lockIn{ D_GAMEPAD_AXIS_LEFT_TRIGGER };

    int m_spell1{ GLFW_GAMEPAD_BUTTON_X };
    int m_spell2{ GLFW_GAMEPAD_BUTTON_B };
    int m_spell3{ GLFW_GAMEPAD_BUTTON_Y };

    // Joysticks
    float m_joystickX{ 0 };
    float m_joystickY{ 0 };
};
