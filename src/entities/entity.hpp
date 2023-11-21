#pragma once
#include "raylib.h"
#include "../components/physics_component.hpp"
#include "../components/render_component.hpp"

struct Entity
{
    PhysicsComponent physics;
    RenderComponent render;
};