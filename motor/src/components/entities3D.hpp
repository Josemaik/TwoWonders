#pragma once 
#include "entity.hpp"
#include "../utils/color.hpp"
#include "../managers/render_manager.hpp"

#include <GL/gl.h>
#include <glm/glm.hpp>

struct Point3D : Entity {
    glm::vec3 position;
    float pointSize;
    Color color;

    Point3D(glm::vec3 pos = {0.0f, 0.0f, 0.0f}, float pSize = 1.0f, Color c = BLACK)
        : position(pos), pointSize(pSize), color(c) {};

    void draw(glm::mat4 transMatrix) override{
        RenderManager rm = RenderManager::getInstance();

        rm.beginMode3D();

        auto nColor = rm.normalizeColor(color);

        // Define vertices for the point
        float vertices[] = {
            position.x, position.y, position.z,
            nColor.x, nColor.y, nColor.z,
        };

        // Create and configure VAO, VBO
        GLuint VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Colors
        GLint colorUniform = glGetUniformLocation(rm.getShader()->id_shader, "customColor");
        glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

        // Transform
        glm::mat4 model      = glm::mat4(1.0f);
        glm::mat4 view       = rm.m_camera->getViewMatrix();
        glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->id_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->id_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->id_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the point
        glPointSize(pointSize);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 1);
        glPointSize(1.0f);

        // Clean up resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        rm.endMode3D();
    };
};