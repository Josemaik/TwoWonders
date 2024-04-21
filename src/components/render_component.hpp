#pragma once
#include <raylib.h>
#include "../motor/src/darkmoon.hpp"
#include "../utils/BBox.hpp"

struct RenderComponent
{
    using ModelType = Model;
    using MeshType = Mesh;
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
    double offset{};
    BBox bbox{};

    DarkMoon::Node* node{ nullptr };
    ModelType model{};
    MeshType mesh{};
    bool meshLoaded{ false };
};