#pragma once
#include <utils/vec3D.hpp>

struct ColliderComponent
{
    ColliderComponent() = default;
    ColliderComponent(vec3f pos, vec3f scale);
    void updateBox(vec3f pos, vec3f scale);
    BBox boundingBox{};
};