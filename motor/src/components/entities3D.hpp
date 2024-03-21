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
            position.x, position.y, position.z, nColor.x, nColor.y, nColor.z,
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
        glm::mat4 model      = transMatrix; // Apply transformation matrix
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

struct Line3D : Entity {
    glm::vec3 startPos, endPos;
    float lineSize; 
    Color color;

    Line3D(glm::vec3 sP = {0.0f, 0.0f, 0.0f}, glm::vec3 eP = {1.0f, 0.0f, 0.0f}, float lSize = 1.0f, Color c = BLACK)
        : startPos(sP), endPos(eP), lineSize(lSize), color(c) {};

    void draw(glm::mat4 transMatrix) override{
        RenderManager rm = RenderManager::getInstance();

        rm.beginMode3D();

        auto nColor = rm.normalizeColor(color);

        // Define vertices for the line
        float vertices[] = {
            // Position                           // Color
            startPos.x, startPos.y, startPos.z,   nColor.x, nColor.y, nColor.z,
              endPos.x,   endPos.y,   endPos.z,   nColor.x, nColor.y, nColor.z,
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
        glm::mat4 model      = transMatrix; // Apply Transformation Matrix
        glm::mat4 view       = rm.m_camera->getViewMatrix();
        glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->id_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->id_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->id_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the line
        glLineWidth(lineSize);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
        glLineWidth(1.0f);

        // Clean up resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        rm.endMode3D();
    }
};

struct Grid : Entity {
    int slices;
    float spacing;
    Color color;

    Grid(int sli = 10, float spac = 1.0f, Color c = GRAY)
        : slices(sli), spacing(spac), color(c) {};

    void draw(glm::mat4 transMatrix) override{
        RenderManager rm = RenderManager::getInstance();

        rm.beginMode3D();

        auto nColor = rm.normalizeColor(color);
        
        // Define vertices for the grid
        std::vector<float> vertices;
        float halfSize = static_cast<float>(slices) * spacing * 0.5f;
        for (int i = 0; i < slices + 1; ++i) {
            float x = static_cast<float>(i) * spacing - halfSize;
            vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(-halfSize);
            vertices.push_back(nColor.x); vertices.push_back(nColor.y); vertices.push_back(nColor.z);

            vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(halfSize);
            vertices.push_back(nColor.x); vertices.push_back(nColor.y); vertices.push_back(nColor.z);

            float z = static_cast<float>(i) * spacing - halfSize;
            vertices.push_back(-halfSize); vertices.push_back(0.0f); vertices.push_back(z);
            vertices.push_back(nColor.x); vertices.push_back(nColor.y); vertices.push_back(nColor.z);

            vertices.push_back(halfSize); vertices.push_back(0.0f); vertices.push_back(z);
            vertices.push_back(nColor.x); vertices.push_back(nColor.y); vertices.push_back(nColor.z);
        }

        // Create and configure VAO, VBO
        GLuint VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

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
        glm::mat4 model      = transMatrix; // Apply Transformation Matrix
        glm::mat4 view       = rm.m_camera->getViewMatrix();
        glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->id_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->id_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->id_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the grid
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices.size() / 6));

        // Clean up resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        rm.endMode3D();
    };
};