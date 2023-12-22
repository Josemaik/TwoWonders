#pragma once 
#ifndef ZONE_SYSTEM
#define ZONE_SYSTEM
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include "../managers/ia_manager.hpp"
#include "../managers/eventmanager.hpp"

struct ZoneSystem
{
    using SYSCMPs = MP::TypeList<ZoneComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, ENGI::GameEngine& engine, Ia_man& iam,Eventmanager& evm);

private:
    void deleteEnemiesinZone(EntityManager& em, uint16_t zone);
};

#endif // !ZONE_SYSTEM