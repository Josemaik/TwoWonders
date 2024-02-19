#pragma once
#include <glm/glm.hpp>

struct Entity{
    virtual void draw(glm::mat4) const {};
};

struct Light:Entity{
    void draw(glm::mat4) const override {};
};

struct Camera:Entity{
    void draw(glm::mat4) const override {};
};