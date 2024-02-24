#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct RenderManager{
private:
    // Shader program
    GLuint m_shaderProgram;

public:
    ~RenderManager(){ glDeleteProgram(m_shaderProgram); };

    // Basic drawing functions
    void clearBackground(glm::vec4 color);
    void drawPixel(int x, int y, float red, float green, float blue);
    void drawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec4 color);

    // Shader
    void compilingShaders();
};