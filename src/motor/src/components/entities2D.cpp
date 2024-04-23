#include "entities2D.hpp"

namespace DarkMoon {

    // PIXEL //

    Pixel::Pixel(Color c) : color(c){
        // Create VAO, VBO
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        // ChangePosition
        changePosition(0.0f, 0.0f);
    };

    Pixel::~Pixel(){
        // Clean up resources
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    };

    void Pixel::changePosition(float newPosX, float newPosY){
        RenderManager rm = RenderManager::getInstance();

        m_position.x = newPosX;
        m_position.y = newPosY;

        float vertex[] = { rm.normalizeX(m_position.x), rm.normalizeY(m_position.y) };

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    };

    void Pixel::draw(glm::mat4 transMatrix){
        RenderManager rm = RenderManager::getInstance();

        // Apply Transformation Matrix
        auto newPosX = transMatrix[3][0];
        auto newPosY = transMatrix[3][1];
        if(m_position.x != newPosX || m_position.y != newPosY)
            changePosition(newPosX, newPosY);

        // Set the uniform color in the shader
        GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Draw the pixel
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
    };

};