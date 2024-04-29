#pragma once

#include "render_manager.hpp"

//#include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>

#include <thread>
#include <chrono>

namespace DarkMoon {
    struct WindowsManager {
    private:
        GLFWwindow* m_window;
        int m_width, m_height;
        int m_oldWidth, m_oldHeight;

        void controlFrameRate();
        void framebuffer_size_callback();

    public:
        // Window-related functions
        bool initWindow(int width, int height, const char* title);
        void closeWindow();
        bool windowShouldClose() const;
        int getScreenWidth() { return m_width; };
        int getScreenHeight() { return m_height; };
        GLFWwindow* getWindow() { return m_window; };
        void setWindowSize(int width, int height);
        bool isWindowFullscreen();
        void toggleFullscreen();
        float getWidthRatio();
        float getHeightRatio();
        glm::vec2 getMonitorSize();

        // Drawing-related functions
        void beginDrawing();
        void endDrawing();

        // Timing-related functions
        void setTargetFPS(int fps); // TODO
        float getFrameTime();
        double getTime();
        int getFPS();

        static WindowsManager& getInstance() {
            static WindowsManager instance;
            return instance;
        }
    };
}