#include "windows_manager.hpp"

#include <iostream>

// Windows manager
namespace DarkMoon {
    bool WindowsManager::initWindow(int width, int height, const char* title) {
        this->m_width = width;
        this->m_height = height;

        // glfw: initialize and configure
        // ------------------------------
        if (!glfwInit()) {
            std::cerr << "Error initializing GLFW" << std::endl;
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Deactivate V-Sync
        // glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

        // Enable V-Sync
        // glfwSwapInterval(1);

        // glfw window creation
        // --------------------
        m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_window) {
            std::cerr << "Error creating window" << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(m_window);
        //glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback());

        // glfwSetErrorCallback([](auto error, auto description){
        //     std::fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        //     throw std::runtime_error("Glfw Error");
        // });

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        //if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        //    std::cerr << "Error initilizing GLAD" std::endl;
        //    return false;
        //}

        if (glewInit() != GLEW_OK) {
            std::cerr << "Error initializing GLEW" << std::endl;
            glfwTerminate();
            return false;
        }

        if (!glfwExtensionSupported("GL_ARB_vertex_array_object") || !GLEW_VERSION_3_3) {
            std::cerr << "Error: OpenGL 3.3 and GL_ARB_vertex_array_object are required" << std::endl;
            return false;
        }

        glViewport(0, 0, m_width, m_height);
        //std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

        return true;
    }

    void WindowsManager::framebuffer_size_callback() {
        glViewport(0, 0, m_width, m_height);
    }

    void WindowsManager::closeWindow() {
        if (m_window) {
            glfwDestroyWindow(m_window);
        }
        glfwTerminate();
    }

    bool WindowsManager::windowShouldClose() const {
        return m_window && glfwWindowShouldClose(m_window);
    }

    void WindowsManager::setWindowSize(int width, int height) {
        if (m_window) {
            m_width = width;
            m_height = height;
            glfwSetWindowSize(m_window, m_width, m_height);
            framebuffer_size_callback();
        }
    }

    bool WindowsManager::isWindowFullscreen(){
        int monitorCount;
        // Check if the window is in fullscreen mode
        GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
        for(int i=0; i<monitorCount; i++){
            GLFWmonitor* monitor = monitors[i];
            if(glfwGetWindowMonitor(m_window) == monitor)
                return true;
        }

        return false;
    }

    void WindowsManager::toggleFullscreen() {
        if(m_window){
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();

            // Fullscreen mode -> Windowed mode
            if(isWindowFullscreen()){
                glfwSetWindowMonitor(m_window, nullptr, 100, 100, m_oldWidth, m_oldHeight, GLFW_DONT_CARE);

                m_width = m_oldWidth;
                m_height = m_oldHeight;
            }
            // Windowed mode -> Fullscreen mode
            else{
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

                m_oldWidth = m_width;
                m_oldHeight = m_height;

                m_width = mode->width;
                m_height = mode->height;
            }
        }
    }

    // Drawing-related functions

    void WindowsManager::beginDrawing() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, m_width, 0, m_height, -1, 1);

        glfwPollEvents();

        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // GLenum error = glGetError();
        // if (error != GL_NO_ERROR) {
        //     std::cerr << "Error OpenGL: " << gluErrorString(error) << std::endl;
        // }
    }

    void WindowsManager::endDrawing() {
        controlFrameRate();
        glfwSwapBuffers(m_window);

        m_lastFrameTime = getTime();
    }

    // Timing-related functions
    void WindowsManager::setTargetFPS(int fps) {
        m_targetFrameTime = 1.0 / static_cast<double>(fps);
    }

    float WindowsManager::getFrameTime() {
        double currentFrameTime = glfwGetTime();
        m_deltaTime = currentFrameTime - m_lastFrameTime;
        m_lastFrameTime = currentFrameTime;

        return static_cast<float>(m_deltaTime);
    }

    double WindowsManager::getTime() {
        return glfwGetTime();
    }

    int WindowsManager::getFPS() {
        double currentTime = glfwGetTime();
        ++m_frameCount;

        if (currentTime - m_lastFPSTime >= 1.0) {
            m_fps = m_frameCount;
            m_frameCount = 0;
            m_lastFPSTime += 1.0;
        }

        return m_fps;
    }

    void WindowsManager::controlFrameRate() {
        if (m_targetFrameTime > 0.0) {
            double currentTime = glfwGetTime();
            double elapsedTime = currentTime - m_lastFrameTime;

            if (elapsedTime < m_targetFrameTime) {
                double sleepTime = m_targetFrameTime - elapsedTime;
                int milliseconds = static_cast<int>(sleepTime * 1000);
                if (milliseconds > 0)
                    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
            }
        }
    }
}