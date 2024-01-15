#pragma once
#ifndef MAP
#define MAP

#include "../utils/types.hpp"

struct Map
{
    struct EntityData
    {
        vec3d position;
        vec3d scale;
        Color color;
    };

    struct ZoneData
    {
        vec3d position{};
        vec3d scale{};
        uint16_t zone{};
    };

    struct WallData
    {
        vec3d position{};
        vec3d scale{};
        Color color{};
    };

    struct RampData
    {
        vec3d position{};
        vec3d scale{};
        Color color{};
        float orientation{};
        vec3d rotationVec{};
    };

    void createMap(EntityManager& em);

private:
    void createWallsOverworld(EntityManager& em);
    void createGroundWaterOverworld(EntityManager& em);
    void createRamps(EntityManager& em);
    void createZonesOverworld(EntityManager& em);
};

#endif // !MAP
