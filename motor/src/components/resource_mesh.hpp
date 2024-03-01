#pragma once
#include "resource.hpp"
#include "resource_texture.hpp"

#include <vector>
#include <memory>
#include <cstdint>
#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textCoords;
};

// Forwarding
struct Texture;

struct Mesh : public Resource{
public:
    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<u_int16_t> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    Mesh(std::size_t, std::vector<Vertex>, std::vector<u_int16_t>, std::vector<std::shared_ptr<Texture>>);

    bool load() override;
    void unload() override;
    bool isLoaded() const override;

    void draw();

private:
    // Render data
    GLuint m_VAO; // Vertex Array Object
    GLuint m_VBO; // Vertex Buffer Object
    GLuint m_EBO; // Element Buffer Object

    void setupMesh();
};