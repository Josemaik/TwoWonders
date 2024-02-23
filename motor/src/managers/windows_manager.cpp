#include "windows_manager.hpp"

#include <iostream>

bool WindowsManager::initWindow(int width, int height, const char* title){
    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Error creating window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing GLEW" << std::endl;
        glfwTerminate();
        return false;
    }

    return true;
}

void WindowsManager::closeWindow(){
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

bool WindowsManager::windowShouldClose() const{
    return window && glfwWindowShouldClose(window);
}