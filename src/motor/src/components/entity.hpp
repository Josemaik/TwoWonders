#pragma once

#include <glm/glm.hpp>

namespace DarkMoon {
    struct Entity {
        virtual ~Entity() {};

        virtual void draw(glm::mat4) {};
        virtual void unload() {};
    };

    //struct Light : Entity{
    //    void draw(glm::mat4) const override {};
    //};
}