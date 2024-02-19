#pragma once
#include "resource.hpp"

#include <vector>
#include <memory>
#include <cstdint>
#include <glm/glm.hpp>

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 textCoords;
};

struct Mesh : public Resource{
public:
    // Mesh data
    std::vector<u_int16_t> index;
    std::vector<std::shared_ptr<Vertex>> vertex;
    std::vector<std::shared_ptr<Texture>> texture;

    void draw();
    bool load() override;
    void unload() override;
    bool isLoaded() const override;

private:
    // Render data
    u_int16_t m_VAO, m_VBO, m_EBO;

    void setupMesh();
};