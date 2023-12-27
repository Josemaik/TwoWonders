#pragma once
#include <raylib.h>
#include "utils/vec3D.hpp"

struct RenderComponent
{
    void setAll(vec3d p, vec3d s, Color c);
    void setPosition(vec3d p);
    void setScale(vec3d s);
    void setColor(Color c);
    vec3d position{};
    vec3d scale{};
    Color color{};
    bool visible{true};
};