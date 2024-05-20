#pragma once

#include "../motor/src/darkmoon.hpp"
#include "../utils/vec3D.hpp"

struct Grass
{
    vec3d startPos{};
    vec3d endPos{};
    DarkMoon::Color color;
};

struct GrassComponent
{
    std::vector<Grass> grass{};
    bool created{ false };
};