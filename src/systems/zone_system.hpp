#pragma once 
#ifndef ZONE_SYSTEM
#define ZONE_SYSTEM
#include <fstream>
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include "../managers/ia_manager.hpp"
#include "../managers/event_manager.hpp"
#include "../managers/map_manager.hpp"

struct ZoneSystem
{
    using SYSCMPs = MP::TypeList<ZoneComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, ENGI::GameEngine& engine, Ia_man& iam, EventManager& evm, MapManager& map, float& dt);

private:
    void checkChests(EntityManager& em, EventManager& evm, uint16_t zone);
    void checkSpawns(EntityManager& em, EventManager& evm);
    void checkDoors(EntityManager& em, EventManager& evm);
    void checkDungeonSlimes(EntityManager& em, EventManager& evm);
    void checkTutorialEnemies(EntityManager& em);
};

#endif // !ZONE_SYSTEM