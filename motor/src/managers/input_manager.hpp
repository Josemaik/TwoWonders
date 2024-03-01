#pragma once

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
    void cleanKeyReleaseStates(){ keyReleaseStates.clear(); };

    // Gamepad / Mouse

private:
    std::unordered_map<int, bool> keyStates;
    std::unordered_map<int, bool> keyReleaseStates;
};