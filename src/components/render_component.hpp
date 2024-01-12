#pragma once
#include <raylib.h>
#include "utils/vec3D.hpp"

struct RenderComponent
{
    ~RenderComponent() { UnloadModel(model); }
    void setAll(vec3d p, vec3d s, Color c);
    void setPosition(vec3d p);
    void setScale(vec3d s);
    void setColor(Color c);
    void setOrientation(double o);

    vec3d position{};
    vec3d scale{};
    double orientation{ 0.0 };
    Color color{};
    bool visible{ true };

    Model model{};

    bool meshLoaded{ false };
};