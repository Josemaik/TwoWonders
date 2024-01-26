#pragma once 
#ifndef ZONE_SYSTEM
#define ZONE_SYSTEM
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include "../managers/ia_manager.hpp"
#include "../managers/eventmanager.hpp"
#include "../managers/map.hpp"

struct ZoneSystem
{
    using SYSCMPs = MP::TypeList<ZoneComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, ENGI::GameEngine& engine, Ia_man& iam, Eventmanager& evm, Map& map);
    void reset();

private:
    void updateZoneEnemies(EntityManager& em);
    void deleteZoneEnemies(EntityManager& em);
    void createKey(EntityManager& em);

    bool keyCreated{ false };
    deathSet dead_entities{};
};

#endif // !ZONE_SYSTEM