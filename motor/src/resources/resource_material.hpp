#pragma once
#include "resource.hpp"
#include "resource_texture.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

struct Material : public Resource{
public:
    Material(std::size_t);

    bool load() override;
    void unload() override;
    bool isLoaded() const override;

private:
    glm::vec3 m_ambientColor;
    glm::vec3 m_diffuseColor;
    glm::vec3 m_specularColor;
    float m_shininess;
    std::vector<std::shared_ptr<Texture>> m_textures;
};