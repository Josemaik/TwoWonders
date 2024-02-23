#pragma once

//#include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

struct WindowsManager{
private:
    GLFWwindow* m_window;
    int m_width, m_height;

public:

    // Windows functions
    bool initWindow(int width, int height, const char* title);
    void closeWindow();
    bool windowShouldClose() const;

    // Drawing functions
    void beginDrawing();
    void endDrawing();
    void clearBackground(float red, float green, float blue);

    void drawPixel(int x, int y, float red, float green, float blue);

    void prueba();
};