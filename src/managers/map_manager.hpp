#pragma once
#ifndef MAP_MANAGER
#define MAP_MANAGER

#include "../utils/types.hpp"
#include "ia_manager.hpp"

enum InteractableType
{
    Chest = 0x01,
};

struct MapManager
{
    void createMap(EntityManager& em, uint8_t mapID, Ia_man& iam);
    void reset(EntityManager& em, uint8_t mapID, Ia_man& iam);
    mapType loadMap(const std::string& path);
private:
    void destroyMap(EntityManager& em);
    void generateMapFromJSON(EntityManager& em, const mapType& map, Ia_man& iam);
};

#endif // !MAP_MANAGER
