#include "input_manager.hpp"

// Update

// TODO: callback
void InputManager::update(){
    int jid = 0;
    // Update gamepad state
    GLFWgamepadstate gamepadState;
    if(glfwGetGamepadState(jid, &gamepadState))
        m_gamepadStates[jid] = gamepadState;
    // else
    //     std::cerr << "Error" << std::endl;
}

// Input-related functions: keyboard

bool InputManager::isKeyPressed(int key){
    return m_keyStates[key];
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

    if(key >= 0){
        im->m_keyStates[key] = action;

        if(action == GLFW_RELEASE)
            im->m_keyReleaseStates[key] = true;
        else
            im->m_keyReleaseStates[key] = false;
    }

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

// TODO
bool InputManager::isGamepadButtonPressed(int, int){
    return true;
}

bool InputManager::isGamepadButtonDown(int gamepad, int button){
    // std::cout << "Gamepad: " << gamepad << " - Button: " << button << std::endl;
    // std::cout << "State: " << m_gamepadStates[gamepad].buttons[button] << std::endl;
    return m_gamepadStates[gamepad].buttons[button] == GLFW_PRESS;
}

// TODO
bool InputManager::isGamepadButtonReleased(int, int){
    return true;
}

// TODO
bool InputManager::isGamepadButtonUp(int gamepad, int button){
    return m_gamepadStates[gamepad].buttons[button] == GLFW_RELEASE;
}

// TODO
int InputManager::getGamepadAxisCount(int){
    return 1;
}

// TODO
float InputManager::getGamepadAxisMovement(int, int){
    return 1.0f;
}

 // Input-related functions: mouse
bool InputManager::isMouseButtonPressed(int button){
    return m_mouseButtonStates[button];
}

bool InputManager::isMouseButtonDown(int button){
    return m_mouseButtonStates[button];
}

bool InputManager::isMouseButtonReleased(int button){
    if(m_mouseButtonReleaseStates[button]){
        m_mouseButtonReleaseStates[button] = false;
        return true;
    }
    return false;
}

bool InputManager::isMouseButtonUp(int button){
    return m_mouseButtonStates[button] == GLFW_RELEASE;
}

int InputManager::getMouseX(GLFWwindow* window){
    double posX, posY;
    glfwGetCursorPos(window, &posX, &posY);
    return static_cast<int>(posX);
}

int InputManager::getMouseY(GLFWwindow* window){
    double posX, posY;
    glfwGetCursorPos(window, &posX, &posY);
    return static_cast<int>(posY);
}

// glm::vec2 InputManager::getMousePosition(){
// 
// }

void InputManager::setMousePosition(GLFWwindow* window, int x, int y){
    glfwSetCursorPos(window, x, y);
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int){
    auto* im = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

    if(button >= 0){
        im->m_mouseButtonStates[button] = action;

        if(action == GLFW_RELEASE)
            im->m_mouseButtonReleaseStates[button] = true;
        else
            im->m_mouseButtonReleaseStates[button] = false;
    }
}