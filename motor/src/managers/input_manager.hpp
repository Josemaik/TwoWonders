#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <unordered_map>

struct InputManager{
public:
    // ------------------------ //
    // Input Handling Functions //
    // ------------------------ //

    // Input-related functions: keyboard
    bool isKeyPressed(int key);
    bool isKeyReleased(int key);
    bool isKeyDown(GLFWwindow* window, int key);
    bool isKeyUp(GLFWwindow* window, int key);

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void cleanm_keyReleaseStates(){ m_keyReleaseStates.clear(); };

    // Input-related functions: gamepad
    bool isGamePadAvailable(int gamepad);
    const char* getGamePadName(int gamepad);
    bool isGamePadButtonPressed(int gamepad, int button);  // TODO
    bool isGamePadButtonReleased(int gamepad, int button); // TODO
    bool isGamePadButtonDown(int gamepad, int button);
    bool isGamePadButtonUp(int gamepad, int button);

    static void joystickCallback(int jid, int event);
    void gamepadButtonCallback(int jid, int event);

    // Mouse

private:
    // Key States
    std::unordered_map<int, bool> m_keyStates;
    std::unordered_map<int, bool> m_keyReleaseStates;

    // Button States
    std::unordered_map<int, std::unordered_map<int, bool>> m_gamepadButtonStates;
    std::unordered_map<int, std::unordered_map<int, bool>> m_gamepadButtonReleaseStates;

    // Instance
    static InputManager& getInstance(){
        static InputManager instance;
        return instance;
    }
};