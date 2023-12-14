#pragma once
#ifndef IA_MAN
#define IA_MAN

#include "../utils/types.hpp"
#include <cstdint>
struct Ia_man
{
    // void createMap(EntityManager& em);
    void createEnemiesZone(EntityManager& em,uint16_t zone);
    void deleteEnemiesZone(EntityManager& em,uint16_t zone);
    bool checkEnemiesCreaeted(uint16_t zone);
    // private:
    //     void createWallsOverworld(EntityManager& em);
    //     void createGroundWaterOverworld(EntityManager& em);

    //     void createZonesOverworld(EntityManager& em);
    private:
        bool createdzone2{false},createdzone3{false},createdzone4{false},
        createdzone5{false},createdzone6{false};
        void createEnemiesZone2(EntityManager& em);
        void createEnemiesZone3(EntityManager& em);
        void createEnemiesZone4(EntityManager& em);
        void createEnemiesZone5(EntityManager& em);
        void createEnemiesZone6(EntityManager& em);
};

#endif // !MAP