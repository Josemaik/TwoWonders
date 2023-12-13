#pragma once
#ifndef MAP
#define MAP

#include "../utils/types.hpp"

struct Map
{
    void createMap(EntityManager& em);

    private:
        void createWallsOverworld(EntityManager& em);
        void createGroundWaterOverworld(EntityManager& em);

        void createZonesOverworld(EntityManager& em);
};

#endif // !MAP