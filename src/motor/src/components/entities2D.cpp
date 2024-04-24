#include "entities2D.hpp"

namespace DarkMoon {

    // PIXEL //

    Pixel::Pixel(Color c) 
        : color(c){
        // Create VAO, VBO
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
    };

    Pixel::~Pixel(){
        // Clean up resources
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    };

    void Pixel::changeVAO(glm::mat4& transMatrix){
        RenderManager rm = RenderManager::getInstance();

        m_transMatrix = transMatrix;
        float vertex[] = { rm.normalizeX(m_transMatrix[3][0]), rm.normalizeY(m_transMatrix[3][1]) };

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
        if(m_transMatrix != transMatrix)
            changeVAO(transMatrix);

        // Set the uniform color in the shader
        GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Set the point size
        glPointSize(transMatrix[0][0]);

        // Draw the pixel
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);

        // Reset the point size
        glPointSize(1);
    };

    // LINE //

    Line::Line(glm::vec2 sP, glm::vec2 eP, Color c) 
        : startPos(sP), endPos(eP), color(c) {
        // Create VAO, VBO
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
    };

    Line::~Line(){
        // Clean up resources
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    };

    void Line::changeVAO(glm::mat4& transMatrix){
        RenderManager rm = RenderManager::getInstance();

        m_transMatrix = transMatrix;

        // Apply Transformation Matrix
        auto position = glm::vec2(m_transMatrix[3][0], m_transMatrix[3][1]);
        glm::vec2 aux_size;
        aux_size.x = glm::length(glm::vec2(m_transMatrix[0][0], m_transMatrix[1][0]));
        aux_size.y = glm::length(glm::vec2(m_transMatrix[1][1], m_transMatrix[1][0]));

        // Define the vertices for the line
        float vertices[] = {
            rm.normalizeX(startPos.x * aux_size.x + position.x), rm.normalizeY(startPos.y * aux_size.y + position.y),
            rm.normalizeX(endPos.x * aux_size.x + position.x)  , rm.normalizeY(endPos.y * aux_size.y + position.y)
        };

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Set up vertex attribute pointers
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    };

    void Line::draw(glm::mat4 transMatrix){
        RenderManager rm = RenderManager::getInstance();

        // Apply Transformation Matrix
        if(m_transMatrix != transMatrix)
            changeVAO(transMatrix);

        // Set the uniform color in the shader
        GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Set line width
        glLineWidth(m_transMatrix[0][0]);

        // Draw the line
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);

        // Reset line width
        glLineWidth(1);
    };

    // TRIANGLE //

    Triangle::Triangle(glm::vec2 v_1, glm::vec2 v_2, glm::vec2 v_3, Color c)
        : v1(v_1), v2(v_2), v3(v_3), color(c) {
        // Create VAO, VBO and EBO
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
    };

    Triangle::~Triangle(){
        // Clean up resources
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void Triangle::changeVAO(glm::mat4& transMatrix){
        RenderManager rm = RenderManager::getInstance();

        m_transMatrix =  transMatrix;

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

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Triangle::draw(glm::mat4 transMatrix){
        RenderManager rm = RenderManager::getInstance();

        // Apply Transformation Matrix
        if(m_transMatrix != transMatrix)
            changeVAO(transMatrix);

        // Set the uniform color in the shader
        GLuint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Draw the triangle
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    // RECTANGLE //

    Rectangle::Rectangle(glm::vec2 p, glm::vec2 s, Color c)
        : position(p), size(s), color(c) {
        // Create VAO, VBO and EBO
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
    };

    Rectangle::~Rectangle(){
        // Clean up resources
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    };

    void Rectangle::changeVAO(glm::mat4& transMatrix){
        RenderManager rm = RenderManager::getInstance();

        m_transMatrix = transMatrix;

        // Apply Transformation Matrix
        position = glm::vec2(m_transMatrix[3][0], m_transMatrix[3][1]);
        glm::vec2 aux_size;
        aux_size.x = size.x * glm::length(glm::vec2(m_transMatrix[0][0], m_transMatrix[1][0]));
        aux_size.y = size.y * glm::length(glm::vec2(m_transMatrix[1][1], m_transMatrix[1][0]));

        // Define vertices and indices
        float vertices[] = {
            rm.normalizeX(position.x)             , rm.normalizeY(position.y)             , 0.0f,
            rm.normalizeX(position.x + aux_size.x), rm.normalizeY(position.y)             , 0.0f,
            rm.normalizeX(position.x)             , rm.normalizeY(position.y + aux_size.y), 0.0f,
            rm.normalizeX(position.x + aux_size.x), rm.normalizeY(position.y + aux_size.y), 0.0f
        };
        GLuint indices[] = { 0, 1, 2, 1, 2, 3 };

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };

    void Rectangle::draw(glm::mat4 transMatrix){
        RenderManager rm = RenderManager::getInstance();

        // Apply Transformation Matrix
        if(m_transMatrix != transMatrix)
            changeVAO(transMatrix);

        // Set the uniform color in the shader
        GLuint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Draw the rectangle
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

};