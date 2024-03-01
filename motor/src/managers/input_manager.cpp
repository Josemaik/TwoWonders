#include "input_manager.hpp"

bool InputManager::isKeyPressed(int key){
    return keyStates[key];
}

bool InputManager::isKeyReleased(int key){
    return keyReleaseStates[key];
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
        inputManager->keyStates[key] = true;
    else{
        inputManager->keyStates[key] = false;
        inputManager->keyReleaseStates[key] = true;
    }
}
