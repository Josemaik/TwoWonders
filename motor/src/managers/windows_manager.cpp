#include "windows_manager.hpp"

#include <iostream>

// Windows manager

bool WindowsManager::initWindow(int width, int height, const char* title){
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

void WindowsManager::framebuffer_size_callback(){
    glViewport(0, 0, m_width, m_height);
}

void WindowsManager::closeWindow(){
    if (m_window) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
}

bool WindowsManager::windowShouldClose() const{
    return m_window && glfwWindowShouldClose(m_window);
}

// Drawing-related functions

void WindowsManager::beginDrawing(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_width, 0, m_height, -1, 1);

    // GLenum error = glGetError();
    // if (error != GL_NO_ERROR) {
    //     std::cerr << "Error OpenGL: " << gluErrorString(error) << std::endl;
    // }
}

void WindowsManager::endDrawing(){
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}