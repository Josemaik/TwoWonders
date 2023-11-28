#pragma once
#include "raylib.h"

struct RenderComponent
{

    Vector3 position{};
    Vector3 scale{};
    Color color{};
    void setAll(Vector3 v, Vector3 s, Color c);
    void setPosition(Vector3 v);
    void setScale(Vector3 v);
    void setColor(Color c);
    BoundingBox getBoundingBox();
};