#pragma once 
#ifndef ZONE_SYSTEM
#define ZONE_SYSTEM
#include <fstream>
#include <unordered_set>
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include "../managers/ia_manager.hpp"
#include "../managers/event_manager.hpp"
#include "../managers/map_manager.hpp"

struct ZoneSystem
{
    using SYSCMPs = MP::TypeList<ZoneComponent>;
    using SYSTAGs = MP::TypeList<>;
    using checkType = const std::unordered_set<uint8_t>&;
    using checkFuncType = std::function<void(EntityManager&, EventManager&)>;

    void update(EntityManager& em, ENGI::GameEngine& engine, Ia_man& iam, EventManager& evm, MapManager& map);
    void insertZone(uint8_t zone, InteractableType type);
private:
    void checkZones(EntityManager& em, EventManager& evm, checkType zones, checkFuncType checkFunction);
    void checkChests(EntityManager& em, EventManager& evm);
    void checkLevers(EntityManager& em, EventManager& evm);
    void checkLadders(EntityManager& em);
    void checkSigns(EntityManager& em);
    void checkMissionObjs(EntityManager& em, EventManager& evm);
    void checkNPCs(EntityManager& em, EventManager& evm);
    void checkDoors(EntityManager& em, EventManager& evm);
    // void checkSpawns(EntityManager& em, EventManager& evm);
    // void checkDungeonSlimes(EntityManager& em, EventManager& evm);
    void checkTutorialEnemies(EntityManager& em);
    void checkVolcanoLava(EntityManager& em);
    void openDoorsZone(EntityManager& em, EventManager& evm, vec3d& leverPos);

    float elapsedEnding{}, countdownEnding{ 1.5f };
};

#endif // !ZONE_SYSTEM