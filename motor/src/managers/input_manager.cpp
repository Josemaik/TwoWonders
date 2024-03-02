#include "input_manager.hpp"

// Input-related functions: keyboard

bool InputManager::isKeyPressed(int key){
    if(m_keyStates[key] == 1){
        m_keyStates[key] = GLFW_REPEAT;
        return true;
    }
    return false;
}

bool InputManager::isKeyPressedRepeat(int key){
    return m_keyStates[key] == GLFW_PRESS;
}

bool InputManager::isKeyDown(int key){
    return m_keyStates[key] == GLFW_REPEAT;
}

bool InputManager::isKeyReleased(int key){
    if(m_keyReleaseStates[key]){
        m_keyReleaseStates[key] = false;
        return true;
    }
    return false;
}

bool InputManager::isKeyUp(int key){
    return m_keyStates[key] == GLFW_RELEASE;
}

void InputManager::keyCallback(GLFWwindow* window, int key, int, int action, int){
    auto* im = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

    std::cout << "Key: " << key << " - Action: " << action << std::endl;

    im->m_keyStates[key] = action;

    if(action == GLFW_RELEASE)
        im->m_keyReleaseStates[key] = true;
    else
        im->m_keyReleaseStates[key] = false;

    // if(im)
    //     im->m_keyStates[key] = action < ;
}

// Input-related functions: gamepad

bool InputManager::isGamePadAvailable(int gamepad){
    return glfwJoystickPresent(gamepad);
}

const char* InputManager::getGamePadName(int gamepad){
    const char* name = glfwGetJoystickName(gamepad);
    return name ? name : "Uknown";
}

bool InputManager::isGamePadButtonPressed(int gamepad, int button){
    int count;
    auto* buttons =  glfwGetJoystickButtons(gamepad, &count);

    return (button < count) && (buttons[button] == GLFW_PRESS);
}

bool InputManager::isGamePadButtonDown(int gamepad, int button){
    int count;
    auto* buttons =  glfwGetJoystickButtons(gamepad, &count);

    return (button < count) && (buttons[button] == GLFW_PRESS);
}

bool InputManager::isGamePadButtonReleased(int gamepad, int button){
    int count;
    auto* buttons =  glfwGetJoystickButtons(gamepad, &count);

    return (button < count) && (buttons[button] == GLFW_RELEASE);
}

bool InputManager::isGamePadButtonUp(int gamepad, int button){
    int count;
    auto* buttons =  glfwGetJoystickButtons(gamepad, &count);

   return (button >= count) || (buttons[button] == GLFW_RELEASE);
}

void InputManager::joystickCallback(int jid, int event) {
    InputManager& instance = InputManager::getInstance();
    instance.gamepadButtonCallback(jid, event);
}

void InputManager::gamepadButtonCallback(int, int){
    
}