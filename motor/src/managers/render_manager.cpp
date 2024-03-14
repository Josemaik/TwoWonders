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
    glEnable(GL_DEPTH_TEST);
}

void RenderManager::endMode3D(){
    //glPopMatrix();
    //glPopAttrib();

    glDisable(GL_DEPTH_TEST);
}

// Basic drawing functions

void RenderManager::clearBackground(Color color){
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Texture drawing functions

void RenderManager::drawTexture(Texture* texture, glm::vec2 pos, Color color){
    auto nColor = normalizeColor(color);
    float halfWidth = static_cast<float>(texture->getWidth()) / 2.0f;
    float halfHeight = static_cast<float>(texture->getHeight()) / 2.0f;

    // Define vertices and indices
    float vertices[] = {
        // positions                                                           // colors                       // texture coords
        normalizeX(pos.x - halfWidth), normalizeY(pos.y - halfHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   0.0f, 0.0f,
        normalizeX(pos.x + halfWidth), normalizeY(pos.y - halfHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   1.0f, 0.0f,
        normalizeX(pos.x - halfWidth), normalizeY(pos.y + halfHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   0.0f, 1.0f,
        normalizeX(pos.x + halfWidth), normalizeY(pos.y + halfHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   1.0f, 1.0f
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
    glUniform4fv(colorUniform, 1, glm::value_ptr(normalizeColor(color)));

    // Draw Texture
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void RenderManager::drawTextureExtra(Texture* texture, glm::vec2 pos, float rotate, float scale, Color color){
    auto nColor = normalizeColor(color);
    float halfWidth = static_cast<float>(texture->getWidth()) / 2.0f;
    float halfHeight = static_cast<float>(texture->getHeight()) / 2.0f;

    // Define vertices and indices
    float vertices[] = {
        // positions                                                           // colors                    // texture coords
        normalizeX(pos.x - halfWidth), normalizeY(pos.y - halfHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   0.0f, 0.0f,
        normalizeX(pos.x + halfWidth), normalizeY(pos.y - halfHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   1.0f, 0.0f,
        normalizeX(pos.x - halfWidth), normalizeY(pos.y + halfHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   0.0f, 1.0f,
        normalizeX(pos.x + halfWidth), normalizeY(pos.y + halfHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   1.0f, 1.0f
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
    glUniform4fv(colorUniform, 1, glm::value_ptr(normalizeColor(color)));

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

// Basic geometric 3D shapes drawing functions

void RenderManager::drawPoint3D(glm::vec3 position, float pointSize, Color color){
    auto nColor = normalizeColor(color);

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
    GLint colorUniform = glGetUniformLocation(m_shaderProgram->id_shader, "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

    // Transform
    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix(m_width, m_height);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Draw the point
    glPointSize(pointSize);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 1);
    glPointSize(1.0f);

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void RenderManager::drawLine3D(glm::vec3 startPos, glm::vec3 endPos, float lineSize, Color color){
    auto nColor = normalizeColor(color);

    // Define vertices for the line
    float vertices[] = {
        startPos.x, startPos.y, startPos.z, nColor.x, nColor.y, nColor.z,
        endPos.x, endPos.y, endPos.z, nColor.x, nColor.y, nColor.z,
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
    GLint colorUniform = glGetUniformLocation(m_shaderProgram->id_shader, "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

    // Transform
    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix(m_width, m_height);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Draw the line
    glLineWidth(lineSize);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glLineWidth(1.0f);

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void RenderManager::drawGrid(int slices, float spacing, Color color){
    auto nColor = normalizeColor(color);

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
    GLint colorUniform = glGetUniformLocation(m_shaderProgram->id_shader, "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

    // Transform
    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix(m_width, m_height);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Draw the grid
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices.size() / 6));

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void RenderManager::drawPlane(glm::vec3 centerPos, glm::vec2 size, Color color){
    //std::cout << static_cast<float>(color.r) << " - " << static_cast<float>(color.g) << " - " << static_cast<float>(color.b) << " - " << static_cast<float>(color.a) << std::endl;
    auto nColor = normalizeColor(color);
    //std::cout << nColor.x << " - " << nColor.y << " - " << nColor.z << " - " << nColor.w << std::endl;

    // Calculate half size for convenience
    glm::vec2 halfSize = size * 0.5f;

    // Define vertices and indices for the plane
    float vertices[] = {
        // positions                                                       // colors
        centerPos.x - halfSize.x, centerPos.y, centerPos.z - halfSize.y,   nColor.x, nColor.y, nColor.z,
        centerPos.x + halfSize.x, centerPos.y, centerPos.z - halfSize.y,   nColor.x, nColor.y, nColor.z,
        centerPos.x - halfSize.x, centerPos.y, centerPos.z + halfSize.y,   nColor.x, nColor.y, nColor.z,
        centerPos.x + halfSize.x, centerPos.y, centerPos.z + halfSize.y,   nColor.x, nColor.y, nColor.z,
    };

    GLuint indices[] = { 0, 1, 2, 1, 2, 3 };

    // Create and configure VAO, VBO, and EBO
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Colors
    GLint colorUniform = glGetUniformLocation(m_shaderProgram->id_shader, "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

    // Transform
    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix(m_width, m_height);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Draw the plane
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void RenderManager::drawCube(glm::vec3 position, glm::vec3 size, Color color){
    auto nColor = normalizeColor(color);

    // Define vertices and indices for the plane
    float halfSizeX = size.x / 2.0f;
    float halfSizeY = size.y / 2.0f;
    float halfSizeZ = size.z / 2.0f;

    float vertices[] = {
        // positions                        // colors
        -halfSizeX, -halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
         halfSizeX, -halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
         halfSizeX,  halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
        -halfSizeX,  halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
        -halfSizeX, -halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
         halfSizeX, -halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
         halfSizeX,  halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
        -halfSizeX,  halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
    };
    GLuint indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 5, 5, 4, 0,
        1, 2, 6, 6, 5, 1,
        2, 3, 7, 7, 6, 2,
        3, 0, 4, 4, 7, 3 
    };

    // Create and configure VAO, VBO, and EBO
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configurar atributos de vértice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Set the uniform color in the shader
    GLint colorUniform = glGetUniformLocation(m_shaderProgram->id_shader, "customColor");
    glUseProgram(m_shaderProgram->id_shader);
    glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

    // Transform
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    glm::mat4 view       = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix(m_width, m_height);
    
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Draw wirefram cube
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void RenderManager::drawCubeWires(glm::vec3 position, glm::vec3 size, Color color){
    auto nColor = normalizeColor(color);

    // Define vertices and indices for the wireframe cube
    float halfSizeX = size.x / 2.0f;
    float halfSizeY = size.y / 2.0f;
    float halfSizeZ = size.z / 2.0f;

    float vertices[] = {
        // positions                        // colors
        -halfSizeX, -halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
         halfSizeX, -halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
         halfSizeX,  halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
        -halfSizeX,  halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
        -halfSizeX, -halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
         halfSizeX, -halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
         halfSizeX,  halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
        -halfSizeX,  halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
    };

    GLuint indices[] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    // Create and configure VAO, VBO, and EBO
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Set wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Configure model transformation
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

    // Set model matrix in the shader
    GLuint modelLoc = glGetUniformLocation(m_shaderProgram->id_shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draw the wireframe cube
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Reset to fill mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// Model 3D

void RenderManager::drawModel(Model* model, glm::vec3 position, float scale, Color tint){
    drawModelExtra(model, position, scale, {0.0f, 0.0f, 0.0f}, 0.0f, tint);
}

void RenderManager::drawModelExtra(Model* model, glm::vec3 position, float scale, glm::vec3 rotationAxis, float rotationAngle, Color tint){
    if(!model || !model->isLoaded())
        return;

    // Set the uniform color in the shader
    GLint colorUniform = glGetUniformLocation(m_shaderProgram->id_shader, "customColor");
    glUseProgram(m_shaderProgram->id_shader);
    glUniform4fv(colorUniform, 1, glm::value_ptr(normalizeColor(tint)));

    // Transform
    glm::mat4 modelM = glm::mat4(1.0f);
    modelM = glm::translate(modelM, position);
    if (glm::length(rotationAngle) > 0.0f)
        modelM = glm::rotate(modelM, glm::radians(rotationAngle), rotationAxis);
    modelM = glm::scale(modelM, glm::vec3(scale));

    glm::mat4 view       = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix(m_width, m_height);
    
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "model"), 1, GL_FALSE, glm::value_ptr(modelM));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Draw meshes
    for (const auto& mesh : model->getMeshes()) {
        GLuint VAO = mesh->getVAO();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->getNumIndices()), GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
    }
}

void RenderManager::drawModelWires(Model* model, glm::vec3 position, float scale, Color tint){
    drawModelWiresExtra(model, position, scale, {0.0f, 0.0f, 0.0f}, 0.0f, tint);
}

void RenderManager::drawModelWiresExtra(Model* model, glm::vec3 position, float scale, glm::vec3 rotationAxis, float rotationAngle, Color tint){
    if(!model || !model->isLoaded())
    return;

    // Set the uniform color in the shader
    GLint colorUniform = glGetUniformLocation(m_shaderProgram->id_shader, "customColor");
    glUseProgram(m_shaderProgram->id_shader);
    glUniform4fv(colorUniform, 1, glm::value_ptr(normalizeColor(tint)));

    // Transform
    glm::mat4 modelM = glm::mat4(1.0f);
    modelM = glm::translate(modelM, position);
    if (glm::length(rotationAngle) > 0.0f)
        modelM = glm::rotate(modelM, glm::radians(rotationAngle), rotationAxis);
    modelM = glm::scale(modelM, glm::vec3(scale));

    glm::mat4 view       = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix(m_width, m_height);
    
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "model"), 1, GL_FALSE, glm::value_ptr(modelM));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->id_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Draw wires
    for (const auto& mesh : model->getMeshes()) {
        GLuint VAO = mesh->getVAO();
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, static_cast<GLsizei>(mesh->getNumIndices()), GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
    }
}

// Text drawing functions

void RenderManager::drawText(const char*, glm::vec2, int, Color){
    /*
    // Set font size
    FT_Set_Pixel_Sizes(m_defaultFont->getFace(), 0, fontSize);

    // Set pen position
    FT_Vector pen;
    pen.x = pos.x * 64;
    pen.y = pos.y * 64;

    // Loop through each character in the text
    for (const char* c = text; *c; ++c) {
        // Load the glyph
        if (!FT_Load_Char(m_defaultFont->getFace(), *c, FT_LOAD_RENDER)) {
            FT_GlyphSlot glyphSlot = m_defaultFont->getFace()->glyph;

            glRasterPos2i(pen.x, pen.y);
            glColor4f(color.r, color.g, color.b, color.a);
            glDrawPixels(glyphSlot->bitmap.width, 
                         glyphSlot->bitmap.rows, 
                         GL_RED, GL_UNSIGNED_BYTE, 
                         glyphSlot->bitmap.buffer);

            // Move the pen to the next position
            pen.x += glyphSlot->advance.x >> 6;
            pen.y += glyphSlot->advance.y >> 6;
        }else
            std::cerr << "Error loading character: " << *c << std::endl;
    }
    */
}