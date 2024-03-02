#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <unordered_set>

struct InputManager{
public:
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
    bool isGamePadButtonPressed(int gamepad, int button);
    bool isGamePadButtonDown(int gamepad, int button);
    bool isGamePadButtonReleased(int gamepad, int button);
    bool isGamePadButtonUp(int gamepad, int button);

    static void joystickCallback(int jid, int event);
    void gamepadButtonCallback(int jid, int event);

    // Mouse

private:
    // Key States
    int m_keyStates[GLFW_KEY_LAST] = { GLFW_RELEASE };
    int m_keyReleaseStates[GLFW_KEY_LAST] = { GLFW_RELEASE };

    // Button States
    std::unordered_map<int, std::unordered_map<int, bool>> m_gamepadButtonStates;
    std::unordered_map<int, std::unordered_map<int, bool>> m_gamepadButtonReleaseStates;

    // Instance
    static InputManager& getInstance(){
        static InputManager instance;
        return instance;
    }
};