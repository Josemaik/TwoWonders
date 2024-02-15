#pragma once
#include <raylib.h>
#include "utils/vec3D.hpp"

struct RenderComponent
{
    void setAll(vec3d p, vec3d s, Color c);
    void setPosition(vec3d p);
    void setScale(vec3d s);
    void setColor(Color c);
    void setOrientation(double o);
    void setRotationVec(vec3d r);
    void destroyMesh();

    vec3d position{};
    vec3d scale{};
    double orientation{ 0.0 };
    Color color{};
    bool visible{ true };
    vec3d rotationVec{ 0.0, 0.1, 0.0 };

    Model model{};
    Mesh mesh{};
    bool meshLoaded{ false };
};