#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct WindowsManager{
    GLFWwindow* window;
    
    bool initWindow(int width, int height, const char* title);
    void closeWindow();
    bool windowShouldClose() const;
};