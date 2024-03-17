#pragma once
#ifndef MAP_MANAGER
#define MAP_MANAGER

#include "../utils/types.hpp"
#include "ia_manager.hpp"
#include "../utils/pf/Graph.hpp"

struct MapManager
{
    void createMap(EntityManager& em, uint8_t mapID, Ia_man& iam);
    void createNavmesh(EntityManager& em);
    void reset(EntityManager& em, uint8_t mapID, Ia_man& iam);
    void spawnReset(EntityManager& em, Ia_man& iam);

    template <typename... Tags>
    inline void resetParts(EntityManager& em)
    {
        // Primero, destruye las partes
        destroyParts<Tags...>(em);

        // Luego, resetea cada parte
        (resetTag<Tags>(em), ...);
    }
  
  private:
    mapType loadMap(const std::string& path);
    void destroyMap(EntityManager& em);
    void generateMapFromJSON(EntityManager& em, const mapType& map, Ia_man& iam);
    void generateChunkFromJSON(EntityManager& em, const rapidjson::Value& chunk, Ia_man& iam, uint8_t mapID, int& j);
    void generateGround(EntityManager& em, const rapidjson::Value& groundArray, int& j);
    void generateWalls(EntityManager& em, const rapidjson::Value& wallArray);
    void generateRamps(EntityManager& em, const rapidjson::Value& rampArray);
    void generateDestructibles(EntityManager& em, const rapidjson::Value& destructibleArray);
    void generateObjects(EntityManager& em, const rapidjson::Value& objectArray, uint8_t mapID);
    void generateEnemies(EntityManager& em, const rapidjson::Value& enemyArray, Ia_man& iam);
    void generateInteractables(EntityManager& em, const rapidjson::Value& interactableArray);

    template <typename... Tags>
    void destroyParts(EntityManager& em)
    {
        using CMPS = MP::TypeList<>;
        auto& li = em.getSingleton<LevelInfo>();

        (em.forEachAny<CMPS, Tags>([&](Entity& entity) { li.dead_entities.insert(entity.getID()); }), ...);
    }

    template <typename Tag>
    inline void resetTag(EntityManager&) {}
    template <typename Tag>
    inline void resetTag(EntityManager& em, Ia_man& iam) {}



    vec3d groundPos{};
    std::string fileMap{};
};

#endif // !MAP_MANAGER
