#pragma once

#include <glm/glm.hpp>

struct Entity{
    virtual void draw(glm::mat4) {};
    virtual ~Entity() {};
};

//struct Light : Entity{
//    void draw(glm::mat4) const override {};
//};