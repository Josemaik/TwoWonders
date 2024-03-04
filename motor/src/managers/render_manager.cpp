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

// Drawing

void RenderManager::beginMode3D(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(glm::radians(95.0f), (800.0f / 600.0f), 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_camera->position.x, m_camera->position.y, m_camera->position.z,
              m_camera->target.x, m_camera->target.y, m_camera->target.z,
              m_camera->up.x, m_camera->up.y, m_camera->up.z);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void RenderManager::endMode3D(){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(glm::radians(95.0f), (800.0f / 600.0f), 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void RenderManager::drawGrid(int slices, float spacing){
    glLineWidth(1.0f);

    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);

    // Draw horizontal lines
    for (int i = -slices / 2; i <= slices / 2; i++) {
        glVertex3f(-spacing * slices / 2, 0.0f, i * spacing);
        glVertex3f(spacing * slices / 2, 0.0f, i * spacing);
    }

    // Draw vertical lines
    for (int i = -slices / 2; i <= slices / 2; i++) {
        glVertex3f(i * spacing, -spacing * slices / 2, 0.0f);
        glVertex3f(i * spacing, spacing * slices / 2, 0.0f);
    }

    glEnd();
}

// Basic drawing functions

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

void RenderManager::drawTextureExtra(std::shared_ptr<Texture> texture, glm::vec2 pos, float rotate, float scale, glm::vec4 color){
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

    // Transform
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(normalizeX(pos.x), normalizeY(pos.y), 0.0f));
    trans = glm::rotate(trans, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(scale, scale, 1.0f));
    GLuint transformLoc = glGetUniformLocation(m_shaderProgram->id_shader, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    // Draw Texture
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// 3D
void RenderManager::drawTexture3D(std::shared_ptr<Texture> texture, glm::vec2 pos, float, float, glm::vec4 color){
    // Define vertices and indices
    float vertices[] = {
        // positions                                                                                                                                       // colors                    // texture coords
        normalizeX(pos.x)                                                  , normalizeY(pos.y)                                                   , 0.0f,   color.x, color.y, color.z,   0.0f, 0.0f,
        normalizeX(pos.x + static_cast<float>(texture->getWidth()))        , normalizeY(pos.y)                                                   , 0.0f,   color.x, color.y, color.z,   1.0f, 0.0f,
        normalizeX(pos.x)                                                  , normalizeY(pos.y + static_cast<float>(texture->getHeight()))        , 0.0f,   color.x, color.y, color.z,   0.0f, 1.0f,
        normalizeX(pos.x + static_cast<float>(texture->getWidth()))        , normalizeY(pos.y + static_cast<float>(texture->getHeight()))        , 0.0f,   color.x, color.y, color.z,   1.0f, 1.0f
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

    // Transform
    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model       = glm::rotate(model, (float)glfwGetTime() * glm::radians(100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    view        = m_camera->getViewMatrix();
    projection  = glm::perspective(glm::radians(95.0f), (800.0f / 600.0f), 0.1f, 100.0f);

    GLuint modelLoc = glGetUniformLocation(m_shaderProgram->id_shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLuint viewLoc = glGetUniformLocation(m_shaderProgram->id_shader, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint projectionLoc = glGetUniformLocation(m_shaderProgram->id_shader, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    // Draw Texture
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


// Text drawing functions

void RenderManager::drawText(const char*, glm::vec2, int, glm::vec4){

}