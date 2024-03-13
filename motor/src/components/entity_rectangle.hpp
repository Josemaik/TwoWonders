#pragma once
#include "entity.hpp"
#include "../utils/color.hpp"
#include "../managers/render_manager.hpp"

#include <GL/gl.h>
#include <glm/glm.hpp>

struct Rectangle : Entity{
public:
    glm::vec2 position;
    glm::vec2 size;
    Color color;

    Rectangle(glm::vec2 p = { 0.0f, 0.0f }, glm::vec2 s = {10.0f, 10.0f}, Color c = BLACK)
        : position(p), size(s), color(c) {};

    void draw(glm::mat4 transMatrix) override{
        RenderManager rm = RenderManager::getInstance();

        // Apply Transformation Matrix
        position = glm::vec2(transMatrix[3][0], transMatrix[3][1]);
        glm::vec2 aux_size;
        aux_size.x = size.x * glm::length(glm::vec2(transMatrix[0][0], transMatrix[1][0]));
        aux_size.y = size.y * glm::length(glm::vec2(transMatrix[1][1], transMatrix[1][0]));

        // Define vertices and indices
        float vertices[] = {
            rm.normalizeX(position.x)             , rm.normalizeY(position.y)             , 0.0f,
            rm.normalizeX(position.x + aux_size.x), rm.normalizeY(position.y)             , 0.0f,
            rm.normalizeX(position.x)             , rm.normalizeY(position.y + aux_size.y), 0.0f,
            rm.normalizeX(position.x + aux_size.x), rm.normalizeY(position.y + aux_size.y), 0.0f
        };
        GLuint indices[] = { 0, 1, 2, 1, 2, 3};

        // Create and configure VAO, VBO and EBO
        GLuint VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Set the uniform color in the shader
        GLuint colorUniform = glGetUniformLocation(rm.getShader()->id_shader, "customColor");
        glUseProgram(rm.getShader()->id_shader);
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Set model matrix uniform
        //GLuint modelUniform = glGetUniformLocation(rm.getShader()->id_shader, "model");
        //glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(transMatrix));

        // Draw the rectangle
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Clean up resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    };
};