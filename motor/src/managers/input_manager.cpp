#include "input_manager.hpp"

// Update
void InputManager::update(){
    /*
    int jid = 0;
    // Update gamepad state
    GLFWgamepadstate gamepadState;
    if(glfwGetGamepadState(jid, &gamepadState))
        m_gamepadStates[jid] = gamepadState;
    else
        std::cerr << "Error" << std::endl;
    */
}

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

    // std::cout << "Key: " << key << " - Action: " << action << std::endl;

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

bool InputManager::isGamepadButtonPressed(int gamepad, int button){
    return true;
}

bool InputManager::isGamepadButtonDown(int gamepad, int button){
    // std::cout << "Gamepad: " << gamepad << " - Button: " << button << std::endl;
    // std::cout << "State: " << m_gamepadStates[gamepad].buttons[button] << std::endl;
    return m_gamepadStates[gamepad].buttons[button] == GLFW_PRESS;
}


bool InputManager::isGamepadButtonReleased(int gamepad, int button){
    return true;
}


bool InputManager::isGamepadButtonUp(int gamepad, int button){
     return m_gamepadStates[gamepad].buttons[button] == GLFW_RELEASE;
}

int InputManager::getGamepadAxisCount(int gamepad){

}

float InputManager::getGamepadAxisMovement(int gamepad, int axis){

}