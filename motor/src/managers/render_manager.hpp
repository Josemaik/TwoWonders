#pragma once

#include "../components/resource_shader.hpp"
#include "../components/resource_texture.hpp"

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

    // TODO: 800 / 600 --> getScreenWidth() / getScreenHeight()
    float normalizeX(float x){ return (x / 800.0f) * 2 - 1; };
    float normalizeY(float y){ return -((y / 600.0f) * 2 - 1); };

public:
    // Basic drawing functions
    void clearBackground(glm::vec4 color);
    void drawPixel(glm::vec2 pos, glm::vec4 color);
    void drawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec4 color);
    void drawRectangle(glm::vec2 pos, glm::vec2 size, glm::vec4 color);

    // Texture drawing functions
    void drawTexture(std::shared_ptr<Texture> texture, glm::vec2 pos, glm::vec4 color);
    // TODO: rotation
    void drawTextureExtra(std::shared_ptr<Texture> texture, glm::vec2 pos, float rotation, float scale, glm::vec4 color);

    // ChangeShader
    void useShader(std::shared_ptr<Shader> shader){ 
        m_shaderProgram = shader; 
        glUseProgram(m_shaderProgram->id_shader);
    };

    // Text
    void drawText(const char*, glm::vec2, int, glm::vec4);
};