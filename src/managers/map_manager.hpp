#pragma once
#ifndef MAP_MANAGER
#define MAP_MANAGER

#include <map>
#include "../utils/types.hpp"
#include "ia_manager.hpp"
#include "../utils/pf/Graph.hpp"
#include "../utils/bt/behaviourtree.hpp"
#include "../utils/bt/action_patrolNPC.hpp"

enum struct LoadState
{
    LOAD_CHUNKS,
    LOAD_GROUND,
    LOAD_WALLS,
    LOAD_RAMPS,
    LOAD_INTERACTABLES,
    LOAD_OBJECTS,
    LOAD_ENEMIES,
    LOAD_NPCS,
    LOAD_NAVMESHES,
    LOAD_COMPLETE
};

struct MapManager
{
    void createMap(EntityManager& em, uint8_t mapID, Ia_man& iam);
    void reset(EntityManager& em, uint8_t mapID, Ia_man& iam);
    void changeMap(EntityManager& em, uint8_t mapID, Ia_man& iam);
    void spawnReset(EntityManager& em, Ia_man& iam);
    bool isRespawning() const { return reSpawn; }
    bool isComplete() const { return state == LoadState::LOAD_COMPLETE; }

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
    void generateChunkFromJSON(EntityManager& em, const valueType& chunk, Ia_man& iam, uint8_t mapID, rapidjson::SizeType& i, int& j);
    void generateChunkModel(EntityManager& em, rapidjson::SizeType& i);
    void generateGround(EntityManager& em, const valueType& groundArray, int& j);
    void generateWalls(EntityManager& em, const valueType& wallArray);
    void generateRamps(EntityManager& em, const valueType& rampArray);
    void generateObjects(EntityManager& em, const valueType& objectArray, uint8_t mapID);
    void generateEnemies(EntityManager& em, const valueType& enemyArray, Ia_man& iam);
    void generateNPCs(EntityManager& em, const valueType& npcArray);
    void generateInteractables(EntityManager& em, const valueType& interactableArray);
    void generateNavmeshes(EntityManager& em);
    void addToZone(EntityManager& em, Entity& e, InteractableType type);
    void checkDispatcher(EntityManager& em, Entity& e, const valueType& value);
    void addMessageCmp(EntityManager& em, Entity& e, const valueType& value);
    vec3d rotateY(const vec3d& v, double angle);
    vec3d rotateAroundPoint(const vec3d& point, const vec3d& pivot, double angle);
    vec3d rotateScale(const vec3d& v, double angle);

    std::string fileMap{};
    LoadState state{ LoadState::LOAD_CHUNKS };
    mapType map{};
    std::vector<const valueType*> chunksVec{};
    uint8_t unique_ids{ 0 };
    bool reSpawn{ false };
    Graph graph{};

    template <typename... Tags>
    void destroyParts(EntityManager& em)
    {
        using CMPS = MP::TypeList<>;
        auto& li = em.getSingleton<LevelInfo>();

        (em.forEachAny<CMPS, Tags>([&](Entity& entity) { li.insertDeath(entity.getID()); }), ...);
    }

    template <typename Tag>
    inline void resetTag(EntityManager&) {}
    template <typename Tag>
    inline void resetTag(EntityManager& em, Ia_man& iam) {}

    std::vector<std::unique_ptr<BehaviourTree_t>> vec_t{};
};

#endif // !MAP_MANAGER
