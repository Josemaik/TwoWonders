#pragma once

//#include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>

#include <thread>
#include <chrono>

struct WindowsManager{
private:
    GLFWwindow* m_window;
    int m_width, m_height;

    double m_lastFrameTime = 0.0;
    double m_deltaTime = 0.0;
    double m_targetFrameTime = 0.0;

    int m_frameCount = 0;
    double m_lastFPSTime = 0.0;
    int m_fps = 0;

    void controlFrameRate();
    void framebuffer_size_callback();

public:
    // Window-related functions
    bool initWindow(int width, int height, const char* title);
    void closeWindow();
    bool windowShouldClose() const;
    int getScreenWidth(){ return m_width; };
    int getScreenHeight(){ return m_height; };
    GLFWwindow* getWindow(){ return m_window; };
    void setWindowSize(int width, int height);

    // Drawing-related functions
    void beginDrawing();
    void endDrawing();

    // Timing-related functions
    void setTargetFPS(int fps); // TODO
    float getFrameTime();
    double getTime();
    int getFPS();
};