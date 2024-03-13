#pragma once
#include "entity.hpp"

#include <vector>
#include <glm/glm.hpp>

struct Light : Entity{
private:
    glm::vec3 m_position;

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;

public:
    Light() : m_position(0.0f, 0.0f, 0.0f) {};
};