#pragma once
#include <utils/path.hpp>
#include <array>

// template<std::size_t N>
struct NPCComponent
{
    //path tamaño 4
    Path_t<4> path{ };
    Path_t<4>::iterator pathIt{ };
    bool path_initialized{ false };
};
