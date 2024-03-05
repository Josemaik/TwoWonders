#pragma once
#include <raylib.h>
#include "utils/BBox.hpp"

struct RenderComponent
{
    void setAll(vec3d p, vec3d s, Color c);
    void setPosition(vec3d p);
    void setScale(vec3d s);
    void setColor(Color c);
    void setOrientation(double o);
    void setRotationVec(vec3d r);
    void destroyMesh();
    void updateBBox();

    vec3d position{};
    vec3d scale{};
    Color color{};
    bool visible{ true };
    double orientation{ 0.0 };
    vec3d rotationVec{ 0.0, 0.1, 0.0 };

    BBox bbox{};

    Model model{};
    Mesh mesh{};
    bool meshLoaded{ false };
};