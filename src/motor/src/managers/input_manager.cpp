#include "input_manager.hpp"

// Input-related functions: keyboard
namespace DarkMoon {
    bool InputManager::isKeyPressed(int key) { return m_keyStates[key]; }
    bool InputManager::isKeyPressedRepeat(int key) { return m_keyStates[key] == GLFW_PRESS; }

    bool InputManager::isKeyDown(int key) { 
        return m_keyStates[key] == GLFW_REPEAT || m_keyStates[key] == GLFW_PRESS; 
    }

    bool InputManager::isKeyReleased(int key) {
        if (m_keyReleaseStates[key]) {
            m_keyReleaseStates[key] = false;
            return true;
        }
        return false;
    }

    bool InputManager::isKeyUp(int key) { return m_keyStates[key] == GLFW_RELEASE; }
    void InputManager::setExitKey(int key){ m_exitKey = key; }

    void InputManager::keyCallback(GLFWwindow* window, int key, int, int action, int) {
        auto* im = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

        // Update the state of the key
        if (key >= 0) {
            im->m_keyStates[key] = action;

            if (action == GLFW_RELEASE)
                im->m_keyReleaseStates[key] = true;
            else
                im->m_keyReleaseStates[key] = false;
        }

        // Mark the GLFW window to be closed
        if (key == im->m_exitKey)
            glfwSetWindowShouldClose(WindowsManager::getInstance().getWindow(), GLFW_TRUE);
    }

    // Input-related functions: gamepad

    bool InputManager::isGamePadAvailable(int gamepad) {
        return glfwJoystickPresent(gamepad);
    }

    const char* InputManager::getGamePadName(int gamepad) {
        const char* name = glfwGetJoystickName(gamepad);
        return name ? name : "Uknown";
    }

    bool InputManager::isGamepadButtonPressed(int gamepad, int button) {
        if(gamepad < 0 || gamepad >= GLFW_JOYSTICK_LAST || button < 0 || button >= GLFW_GAMEPAD_BUTTON_LAST)
            return false;

        return m_gamepadStates[gamepad].buttons[button] == GLFW_PRESS;
    }

    bool InputManager::isGamepadButtonDown(int gamepad, int button) {
        if(gamepad < 0 || gamepad >= GLFW_JOYSTICK_LAST || button < 0 || button >= GLFW_GAMEPAD_BUTTON_LAST)
            return false;

        return m_gamepadStates[gamepad].buttons[button] == GLFW_PRESS;
    }

    bool InputManager::isGamepadButtonReleased(int gamepad, int button) {
        if(gamepad < 0 || gamepad >= GLFW_JOYSTICK_LAST || button < 0 || button >= GLFW_GAMEPAD_BUTTON_LAST)
            return false;
        
        return m_gamepadReleaseStates[gamepad].buttons[button] == GLFW_PRESS;
    }

    bool InputManager::isGamepadButtonUp(int gamepad, int button) {
        if(gamepad < 0 || gamepad >= GLFW_JOYSTICK_LAST || button < 0 || button >= GLFW_GAMEPAD_BUTTON_LAST)
            return false;
        
        return m_gamepadStates[gamepad].buttons[button] == GLFW_RELEASE;
    }

    int InputManager::getGamepadAxisCount(int gamepad) {
        int axes;
        glfwGetJoystickAxes(gamepad, &axes);
        return axes;
    }

    float InputManager::getGamepadAxisMovement(int gamepad, int axis) {
        return m_gamepadStates[gamepad].axes[axis];
    }

    // Update

    void InputManager::update() {
        int jid = 0;
        // Update gamepad state
        GLFWgamepadstate gamepadState;
        if (glfwGetGamepadState(jid, &gamepadState)) {
            m_gamepadStates[jid] = gamepadState;
            // m_gamepadReleaseStates[jid] = gamepadState;

            for (int i = 0; i < GLFW_JOYSTICK_LAST + 1; i++) {
                if (m_gamepadStates[jid].buttons[i] == GLFW_RELEASE)
                    m_gamepadReleaseStates[jid].buttons[i] = 1;
                else
                    m_gamepadReleaseStates[jid].buttons[i] = 0;
            }
        }
    }

    // Input-related functions: mouse
    bool InputManager::isMouseButtonPressed(int button) {
        return m_mouseButtonPressedStates[button] == GLFW_PRESS;
    }

    bool InputManager::isMouseButtonDown(int button) {
        return m_mouseButtonStates[button];
    }

    bool InputManager::isMouseButtonReleased(int button) {
        return m_mouseButtonReleaseStates[button];
    }

    bool InputManager::isMouseButtonUp(int button) {
        return m_mouseButtonStates[button] == GLFW_RELEASE;
    }

    int InputManager::getMouseX(GLFWwindow* window) {
        double posX, posY;
        glfwGetCursorPos(window, &posX, &posY);
        return static_cast<int>(posX);
    }

    int InputManager::getMouseY(GLFWwindow* window) {
        double posX, posY;
        glfwGetCursorPos(window, &posX, &posY);
        return static_cast<int>(posY);
    }

    // glm::vec2 InputManager::getMousePosition(){
    // 
    // }

    void InputManager::setMousePosition(GLFWwindow* window, int x, int y) {
        glfwSetCursorPos(window, x, y);
    }

    void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int) {
        auto* im = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

        if (button >= 0) {
            // PRESS //

            if (action == GLFW_PRESS){
                std::cout << "Boton: " << button << " apretado\n";
                if (im->m_mouseButtonStates[button] == GLFW_PRESS 
                    && im->m_mouseButtonPressedStates[button] == GLFW_RELEASE)
                {
                    std::cout << "Estado anterior del boton - apretado\n";
                    im->m_mouseButtonPressedStates[button] = GLFW_PRESS;
                }
                else{
                    im->m_mouseButtonPressedStates[button] = GLFW_RELEASE;
                }
            }

            /*
             && im->m_mouseButtonPressedStates[button] == GLFW_RELEASE)
                im->m_mouseButtonPressedStates[button] = GLFW_PRESS;
            else
                im->m_mouseButtonPressedStates[button] = GLFW_RELEASE;
            */

            // RELEASE //

            //if (action == 0 && im->m_mouseButtonStates[button])
            //    im->m_mouseButtonReleaseStates[button] = true;
            //else
            //    im->m_mouseButtonReleaseStates[button] = false;

            im->m_mouseButtonStates[button] = action;
        }
    }
}