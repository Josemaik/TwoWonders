#pragma once
#ifndef MAP
#define MAP

#include "../utils/types.hpp"
#include "ia_manager.hpp"

struct Map
{
    void createMap(EntityManager& em, uint8_t mapID, Ia_man& iam);
    void reset(EntityManager& em, uint8_t mapID, Ia_man& iam);

private:
    mapType loadMap(const std::string& path);
    void destroyMap(EntityManager& em);
    void generateMapFromJSON(EntityManager& em, const mapType& map, Ia_man& iam);
};

#endif // !MAP
