#pragma once
#include "entity.hpp"
#include "../utils/color.hpp"
#include "../managers/render_manager.hpp"
#include "../resources/resource_texture.hpp"   

#include <GL/gl.h>
#include <glm/glm.hpp>

struct Pixel : Entity{
    Color color;

    Pixel(Color c = BLACK)
        : color(c) {};

    void draw(glm::mat4 transMatrix) override{
        RenderManager rm = RenderManager::getInstance();

        // Apply Transformation Matrix
        auto position = glm::vec2(transMatrix[3][0], transMatrix[3][1]);

        // Define a single vertex for the pixel
        float vertex[] = { rm.normalizeX(position.x), rm.normalizeY(position.y) };

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
        GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Draw the pixel
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);

        // Clean up resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    };
};

struct Line : Entity{
    glm::vec2 startPos, endPos;
    Color color;

    Line(glm::vec2 sP = {0.0f, 0.0f}, glm::vec2 eP = {10.0f, 10.0f}, Color c = BLACK)
        : startPos(sP), endPos(eP), color(c) {};

    void draw(glm::mat4 transMatrix) override{
        RenderManager rm = RenderManager::getInstance();

        // Apply Transformation Matrix
        auto position = glm::vec2(transMatrix[3][0], transMatrix[3][1]);
        glm::vec2 aux_size;
        aux_size.x = glm::length(glm::vec2(transMatrix[0][0], transMatrix[1][0]));
        aux_size.y = glm::length(glm::vec2(transMatrix[1][1], transMatrix[1][0]));

        // Define the vertices for the line
        float vertices[] = {
            rm.normalizeX(startPos.x * aux_size.x + position.x), rm.normalizeY(startPos.y * aux_size.y + position.y),
            rm.normalizeX(endPos.x * aux_size.x + position.x)  , rm.normalizeY(endPos.y * aux_size.y + position.y)
        };

        // Create and configure VAO, VBO
        GLuint VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Set up vertex attribute pointers
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        // Set the uniform color in the shader
        GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Draw the line
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);

        // Clean up resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    };
};

struct Triangle : Entity{
    glm::vec2 v1, v2, v3;
    Color color;

    Triangle(glm::vec2 v_1 = { 0.0f, 0.0f }, glm::vec2 v_2 = { 10.0f, 0.0f }, glm::vec2 v_3 = { 0.0f, 10.0f }, Color c = BLACK)
        : v1(v_1), v2(v_2), v3(v_3), color(c) {};

    void draw(glm::mat4 transMatrix) override{
        RenderManager rm = RenderManager::getInstance();

        // Apply Transformation Matrix
        auto pos = glm::vec2(transMatrix[3][0], transMatrix[3][1]);
        glm::vec2 aux_size;
        aux_size.x = glm::length(glm::vec2(transMatrix[0][0], transMatrix[1][0]));
        aux_size.y = glm::length(glm::vec2(transMatrix[1][1], transMatrix[1][0]));

        // Define vertices and indices
        float vertices[] = {
            rm.normalizeX(v1.x * aux_size.x + pos.x), rm.normalizeY(v1.y * aux_size.y + pos.y), 0.0f,
            rm.normalizeX(v2.x * aux_size.x + pos.x), rm.normalizeY(v2.y * aux_size.y + pos.y), 0.0f,
            rm.normalizeX(v3.x * aux_size.x + pos.x), rm.normalizeY(v3.y * aux_size.y + pos.y), 0.0f,
        };
        GLuint indices[] = { 0, 1, 2 };

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
        GLuint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Draw the triangle
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Clean up resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    };
};

struct Rectangle : Entity{
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
        GLuint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Set model matrix uniform
        //GLuint modelUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "model");
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

struct Circle : Entity{
    glm::vec2 position;
    float radius;
    int segments;
    Color color;

    Circle(glm::vec2 p = {0.0f, 0.0f}, float rad = 10.0f, int seg = 20, Color c = BLACK)
        : position(p), radius(rad), segments(seg), color(c) {};

    void draw(glm::mat4 transMatrix) override{
        RenderManager rm = RenderManager::getInstance();

        // Apply Transformation Matrix
        position = glm::vec2(transMatrix[3][0], transMatrix[3][1]);
        auto scale = glm::length(glm::vec2(transMatrix[0][0], transMatrix[1][0]));

        // Calculate vertices for the circle
        int vertexCount = segments * 2;
        std::vector<float> vertices(vertexCount);

        for (int i = 0; i < vertexCount; i += 2) {
            float theta = static_cast<float>((i / 2) * (2.0f * K_PI / segments));
            vertices[i] = rm.normalizeX(position.x + (radius * scale) * std::cos(theta));
            vertices[i + 1] = rm.normalizeY(position.y + (radius * scale) * std::sin(theta));
        }

        // Create and configure VAO, VBO
        GLuint VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, vertices.data(), GL_STATIC_DRAW);

        // Set up vertex attribute pointers
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        // Set the uniform color in the shader
        GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Draw the circle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, segments);
        glBindVertexArray(0);

        // Clean up resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    };
}; 

struct Texture2D : Entity{
    glm::vec2 position;
    Texture* texture;
    Color color;

    Texture2D(glm::vec2 pos = {0.0f, 0.0f}, Texture* text = nullptr, Color col = WHITE)
        : position(pos), texture(text), color(col) {};

    void draw(glm::mat4 transMatrix) override{
        RenderManager rm = RenderManager::getInstance();

        rm.useShader(rm.shaderTexture);

        auto nColor = rm.normalizeColor(color);

        // Apply Transformation Matrix
        position = glm::vec2(transMatrix[3][0], transMatrix[3][1]);
        float halfWidth = static_cast<float>(texture->getWidth()) * glm::length(glm::vec2(transMatrix[0][0], transMatrix[1][0])) / 2.0f;
        float halfHeight = static_cast<float>(texture->getHeight()) * glm::length(glm::vec2(transMatrix[1][1], transMatrix[1][0])) / 2.0f;
 
        // Define vertices and indices
        float vertices[] = {
            // positions                                                  // colors                       // texture coords
            rm.normalizeX(position.x - halfWidth), rm.normalizeY(position.y - halfHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   0.0f, 0.0f,
            rm.normalizeX(position.x + halfWidth), rm.normalizeY(position.y - halfHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   1.0f, 0.0f,
            rm.normalizeX(position.x - halfWidth), rm.normalizeY(position.y + halfHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   0.0f, 1.0f,
            rm.normalizeX(position.x + halfWidth), rm.normalizeY(position.y + halfHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   1.0f, 1.0f
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
        GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Apply Transformation Matrix
        //GLuint transformLoc = glGetUniformLocation(rm.getShader()->getIDShader(), "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transMatrix));

        // Draw Texture
        glBindTexture(GL_TEXTURE_2D, texture->getIDTexture());
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // Clean up resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        rm.useShader(rm.shaderColor);
    };
};