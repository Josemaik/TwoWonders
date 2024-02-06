#pragma once
#ifndef MAP_MANAGER
#define MAP_MANAGER

#include "../utils/types.hpp"
#include "ia_manager.hpp"

struct MapManager
{
    void createMap(EntityManager& em, uint8_t mapID, Ia_man& iam);
    void reset(EntityManager& em, uint8_t mapID, Ia_man& iam);

private:
    mapType loadMap(const std::string& path);
    void destroyMap(EntityManager& em);
    void generateMapFromJSON(EntityManager& em, const mapType& map, Ia_man& iam);
};

#endif // !MAP_MANAGER
