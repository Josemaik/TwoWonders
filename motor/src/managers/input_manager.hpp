#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <unordered_set>

struct InputManager{
public:
    // Update
    void update();

    // ------------------------ //
    // Input Handling Functions //
    // ------------------------ //

    // Input-related functions: keyboard
    bool isKeyPressed(int key);
    bool isKeyPressedRepeat(int key);
    bool isKeyDown(int key);
    bool isKeyReleased(int key);
    bool isKeyUp(int key);

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // Input-related functions: gamepad
    bool isGamePadAvailable(int gamepad);
    const char* getGamePadName(int gamepad);
    bool isGamepadButtonPressed(int gamepad, int button);
    bool isGamepadButtonDown(int gamepad, int button);
    bool isGamepadButtonReleased(int gamepad, int button);
    bool isGamepadButtonUp(int gamepad, int button);
    int getGamepadAxisCount(int gamepad);
    float getGamepadAxisMovement(int gamepad, int axis);

    // Mouse

private:
    // Key States
    int m_keyStates[GLFW_KEY_LAST] = { GLFW_RELEASE };
    int m_keyReleaseStates[GLFW_KEY_LAST] = { GLFW_RELEASE };

    // Button States
    GLFWgamepadstate m_gamepadStates[GLFW_JOYSTICK_LAST + 1];

    // Instance
    static InputManager& getInstance(){
        static InputManager instance;
        return instance;
    }
};