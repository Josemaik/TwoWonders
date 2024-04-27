#include "resource_mesh.hpp"

namespace DarkMoon {
    Mesh::Mesh(std::size_t id, std::vector<Vertex> vertices, std::vector<uint16_t> indices, Material* material) {
        this->m_id = id;
        this->vertices = vertices;
        this->indices = indices;
        this->material = material;
    }

    bool Mesh::load(const char* filePath) {

        setupMesh();

        isLoaded() ? std::cout << "Load a mesh (ID: " << m_id << ") -> " << filePath << std::endl : std::cout << "Error loading a mesh" << std::endl;

        return isLoaded();
    }

    void Mesh::unload() {
        // Delete buffers
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        m_isLoad = false;
        std::cout << "Unload a mesh (ID: " << m_id << ")" << std::endl;
    }

    void Mesh::setupMesh() {
        // Generate vertex array and buffers
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        // Bind vertex array
        glBindVertexArray(m_VAO);

        // Bind buffer and fill with vertex data
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // Bind buffer and fill with index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), &indices[0], GL_STATIC_DRAW);

        // Enable and specify vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // Enable and specify vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // Enable and specify vertex coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textCoords));

        // Unbind vertex array
        glBindVertexArray(0);

        if (m_VAO != 0 && m_VBO != 0 && m_EBO != 0)
            m_isLoad = true;
    }

    void Mesh::draw(glm::mat4 transMatrix, Color color) {
        RenderManager rm = RenderManager::getInstance();

        rm.beginMode3D();

        // Set the uniform color in the shader
        GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Transform
        glm::mat4 model = transMatrix;
        glm::mat4 view = rm.m_camera->getViewMatrix();
        glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());

        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Texture
        if (material->texture)
            glBindTexture(GL_TEXTURE_2D, material->texture->getIDTexture());
        else // Bind default texture
            glBindTexture(GL_TEXTURE_2D, rm.defaultMaterial->texture->getIDTexture());

        // Material - TODO
        // Lights - TODO

        // Draw the triangle of mesh
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);

        // Unbind default texture
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);

        rm.endMode3D();
    }

    void Mesh::drawLines(glm::mat4 transMatrix, Color color) {

        RenderManager rm = RenderManager::getInstance();

        rm.beginMode3D();

        GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Transform
        glm::mat4 model = transMatrix;
        glm::mat4 view = rm.m_camera->getViewMatrix();
        glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());

        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the lines of mesh
        glBindVertexArray(m_VAO);
        glDrawElements(GL_LINES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);

        rm.endMode3D();
    }
}