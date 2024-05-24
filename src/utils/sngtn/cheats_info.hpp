#pragma once
#include <map>
#include <string>
#include "../vec3D.hpp"

struct CheatsInfo
{
    std::map<std::size_t, std::tuple<uint8_t, std::string, vec3d>> cheatPositions{};
};