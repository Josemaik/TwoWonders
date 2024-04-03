#pragma once
#include "resource.hpp"
#include "resource_texture.hpp"
#include "resource_material.hpp"
#include "../utils/color.hpp"
#include "../managers/render_manager.hpp"

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

struct Mesh : public Resource{
public:
    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<u_int16_t> indices;
    Material* material;

    Mesh(std::size_t, std::vector<Vertex>, std::vector<u_int16_t>, Material*);
    ~Mesh(){ unload(); };

    bool load(const char* ) override { return true; };
    void unload() override;
    bool isLoaded() const override;

    void draw(glm::mat4 transMatrix, Color color = GRAY);
    void drawLines(glm::mat4 transMatrix, Color color = BLACK);

private:
    // Render data
    GLuint m_VAO; // Vertex Array Object
    GLuint m_VBO; // Vertex Buffer Object
    GLuint m_EBO; // Element Buffer Object

    void setupMesh();
};