#pragma once

#include "../components/resource_shader.hpp"

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct RenderManager{
private:
    std::shared_ptr<Shader> m_shaderProgram;
    void draw(float vertices[], std::size_t vertSize, GLuint indices[], std::size_t indSize,glm::vec4 color);

public:
    // Basic drawing functions
    void clearBackground(glm::vec4 color);
    void drawPixel(glm::vec2 pos, glm::vec4 color);
    void drawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec4 color);
    void drawRectangle(glm::vec2 pos, glm::vec2 size, glm::vec4 color);

    // ChangeShader
    void useShader(std::shared_ptr<Shader> shader){ m_shaderProgram = shader; };
};