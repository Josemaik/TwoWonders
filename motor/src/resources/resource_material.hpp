#pragma once
#include "resource.hpp"
#include "resource_texture.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

struct Material : public Resource{
public:
    Texture* texture;

    Material();
    Material(std::size_t id, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess){
        this->id = id;
        this->m_ambientColor = ambientColor;
        this->m_diffuseColor = diffuseColor;
        this->m_specularColor = specularColor;
        this->m_shininess = shininess;
    };

    bool load() override{ 
        std::cout << "Load a material (ID:" << id << ")" << std::endl;
        return true; 
    };
    void unload() override {
        std::cout << "Unload a material (ID:" << id << ")" << std::endl;
    };
    bool isLoaded() const override { return true; };

private:
    glm::vec3 m_ambientColor;
    glm::vec3 m_diffuseColor;
    glm::vec3 m_specularColor;
    float m_shininess;
};