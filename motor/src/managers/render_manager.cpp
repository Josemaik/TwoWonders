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

    // Position attribute
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
    float vertex[] = { normalizeX(pos.x), normalizeY(pos.y) };

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
        normalizeX(v1.x), normalizeY(v1.y), 0.0f,
        normalizeX(v2.x), normalizeY(v2.y), 0.0f,
        normalizeX(v3.x), normalizeY(v3.y), 0.0f,
    };
    GLuint indices[] = { 0, 1, 2};

    // Draw triangle
    draw(vertices, 9, indices, 3, color);
}

void RenderManager::drawRectangle(glm::vec2 pos, glm::vec2 size, glm::vec4 color){
    // Define vertices and indices
    float vertices[] = {
        normalizeX(pos.x)         , normalizeY(pos.y)         , 0.0f,
        normalizeX(pos.x + size.x), normalizeY(pos.y)         , 0.0f,
        normalizeX(pos.x)         , normalizeY(pos.y + size.y), 0.0f,
        normalizeX(pos.x + size.x), normalizeY(pos.y + size.y), 0.0f
    };
    GLuint indices[] = { 0, 1, 2, 1, 2, 3};

    // Draw rectangle
    draw(vertices, 12, indices, 6, color);
}

void RenderManager::clearBackground(glm::vec4 color){
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Texture drawing functions

void RenderManager::drawTexture(std::shared_ptr<Texture> texture, glm::vec2 pos, glm::vec4 color){
    // Define vertices and indices
    float vertices[] = {
        // positions                                                                                                                       // colors                    // texture coords
        normalizeX(pos.x)                                          , normalizeY(pos.y)                                           , 0.0f,   color.x, color.y, color.z,   0.0f, 0.0f,
        normalizeX(pos.x + static_cast<float>(texture->getWidth())), normalizeY(pos.y)                                           , 0.0f,   color.x, color.y, color.z,   1.0f, 0.0f,
        normalizeX(pos.x)                                          , normalizeY(pos.y + static_cast<float>(texture->getHeight())), 0.0f,   color.x, color.y, color.z,   0.0f, 1.0f,
        normalizeX(pos.x + static_cast<float>(texture->getWidth())), normalizeY(pos.y + static_cast<float>(texture->getHeight())), 0.0f,   color.x, color.y, color.z,   1.0f, 1.0f
    };

    GLuint indices[] = { 0, 1, 2, 1, 2, 3};

    // Create and configure VAO, VBO and EBO
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Colors
    GLint colorUniform = glGetUniformLocation(m_shaderProgram->id_shader, "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(color));

    // Draw Texture
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}