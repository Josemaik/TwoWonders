#pragma once
#include "resource.hpp"

#include <vector>
#include <memory>
#include <cstdint>
#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 textCoords;
};

struct Mesh : public Resource{
public:
    // Mesh data
    std::vector<u_int16_t> index;
    std::vector<Vertex> vertex;
    std::vector<Texture> texture;

    bool load() override;
    void unload() override;
    bool isLoaded() const override;

    void draw();

    void rellenar(std::vector<Vertex> vertices, std::vector<u_int16_t> indices, std::vector<Texture> textures);

private:
    // Render data
    std::shared_ptr<GLuint> m_VAO; // Vertex Array Object
    std::shared_ptr<GLuint> m_VBO; // Vertex Buffer Object
    std::shared_ptr<GLuint> m_EBO; // Element Buffer Object

    // GLuint m_VAO;
    // GLuint m_VBO;
    // GLuint m_EBO;

    void setupMesh();
};