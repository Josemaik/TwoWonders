#include "input_manager.hpp"

// Input-related functions: keyboard

bool InputManager::isKeyPressed(int key){
    return m_keyStates[key];
}

bool InputManager::isKeyReleased(int key){
    return m_keyReleaseStates[key];
}

bool InputManager::isKeyDown(GLFWwindow* window, int key){
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool InputManager::isKeyUp(GLFWwindow* window, int key){
    return glfwGetKey(window, key) == GLFW_RELEASE;
}

void InputManager::keyCallback(GLFWwindow* window, int key, int, int action, int){
    auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

    if(action == GLFW_PRESS)
        inputManager->m_keyStates[key] = true;
    else{
        inputManager->m_keyStates[key] = false;
        inputManager->m_keyReleaseStates[key] = true;
    }
}

// Input-related functions: gamepad

bool InputManager::isGamePadAvailable(int gamepad){
    return glfwJoystickPresent(gamepad);
}

const char* InputManager::getGamePadName(int gamepad){
    const char* name = glfwGetJoystickName(gamepad);
    return name ? name : "Uknown";
}

bool InputManager::isGamePadButtonPressed(int, int){
    return true;
}

bool InputManager::isGamePadButtonReleased(int, int){
    return true;
}

bool InputManager::isGamePadButtonDown(int gamepad, int button){
    int count;
    auto* buttons =  glfwGetJoystickButtons(gamepad, &count);
    if(buttons && button < count)
        return buttons[button] == GLFW_PRESS;

    return false;
}

bool InputManager::isGamePadButtonUp(int gamepad, int button){
    return !isGamePadButtonDown(gamepad, button);
}

void InputManager::joystickCallback(int jid, int event) {
    InputManager& instance = InputManager::getInstance();
    instance.gamepadButtonCallback(jid, event);
}

void InputManager::gamepadButtonCallback(int, int){
    
}