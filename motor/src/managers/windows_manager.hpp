#pragma once

//#include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>

struct WindowsManager{
private:
    GLFWwindow* m_window;
    int m_width, m_height;

    void framebuffer_size_callback();

public:
    // Windows functions
    bool initWindow(int width, int height, const char* title);
    void closeWindow();
    bool windowShouldClose() const;

    // Drawing-related functions
    void beginDrawing();
    void endDrawing();
};