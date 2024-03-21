#include "render_manager.hpp"

#include <iostream>

// Basic drawing functions

void RenderManager::beginMode3D(){
    useShader(shader3D);
    glEnable(GL_DEPTH_TEST);
}

void RenderManager::endMode3D(){
    //glPopMatrix();
    //glPopAttrib();

    glDisable(GL_DEPTH_TEST);
    useShader(shaderColor);
}

// Basic drawing functions

void RenderManager::clearBackground(Color color){
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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