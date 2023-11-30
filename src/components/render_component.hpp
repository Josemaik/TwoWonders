#pragma once
#include <raylib.h>
#include "utils/vec3D.hpp"

struct RenderComponent
{
    void setAll(vec3f p, vec3f s, Color c);
    void setPosition(vec3f p);
    void setScale(vec3f s);
    void setColor(Color c);
    vec3f position{};
    vec3f scale{};
    Color color{};
};