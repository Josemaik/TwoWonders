#include "render_manager.hpp"

#include <iostream>

// Basic drawing functions

// private:

void RenderManager::draw(float vertices[], std::size_t vertSize, GLuint indices[], std::size_t indSize,glm::vec4 color){
    // Create and configure VAO, VBO and EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set the uniform color in the shader
    GLuint colorUniform = glGetUniformLocation(m_shaderProgram->id_shader, "customColor");
    glUseProgram(m_shaderProgram->id_shader);
    glUniform4fv(colorUniform, 1, glm::value_ptr(color));

    // Draw the triangle
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// public:

void RenderManager::drawPixel(glm::vec2 pos, glm::vec4 color){
    // Define a single vertex for the pixel
    float vertex[] = { pos.x, pos.y };

    // Create and configure VAO, VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Set the uniform color in the shader
    GLint colorUniform = glGetUniformLocation(m_shaderProgram->id_shader, "customColor");
    glUseProgram(m_shaderProgram->id_shader);
    glUniform4fv(colorUniform, 1, glm::value_ptr(color));

    // Draw the pixel
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void RenderManager::drawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec4 color){
    // Define vertices and indices
    float vertices[] = {
        v1.x, v1.y, 0.0f,
        v2.x, v2.y, 0.0f,
        v3.x, v3.y, 0.0f,
    };
    GLuint indices[] = { 0, 1, 2};

    // Draw triangle
    draw(vertices, 9, indices, 3, color);
}

void RenderManager::drawRectangle(glm::vec2 pos, glm::vec2 size, glm::vec4 color){
    // Define vertices and indices
    float vertices[] = {
        pos.x         , pos.y         , 0.0f,
        pos.x + size.x, pos.y         , 0.0f,
        pos.x         , pos.y + size.y, 0.0f,
        pos.x + size.x, pos.y + size.y, 0.0f
    };
    GLuint indices[] = { 0, 1, 2, 1, 2, 3};

    // Draw rectangle
    draw(vertices, 12, indices, 6, color);
}

void RenderManager::clearBackground(glm::vec4 color){
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}