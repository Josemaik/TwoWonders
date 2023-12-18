#pragma once
#ifndef IA_MAN
#define IA_MAN

#include "../utils/types.hpp"
#include <cstdint>

#include "../utils/bt/behaviourtree.hpp"
#include "../utils/bt/actionpatrol.hpp"
#include "../utils/bt/actionrandom.hpp"

struct Ia_man
{
    // void createMap(EntityManager& em);
    void createEnemiesZone(EntityManager& em, uint16_t zone);
    bool checkEnemiesCreaeted(uint16_t zone);
private:
    bool createdzone2{ false }, createdzone3{ false }, createdzone4{ false },
        createdzone5{ false }, createdzone6{ false }, createdzone12{ false },
        createdzone11{false};

    void createEnemiesZone2(EntityManager& em);
    void createEnemiesZone3(EntityManager& em);
    void createEnemiesZone4(EntityManager& em);
    void createEnemiesZone5(EntityManager& em);
    void createEnemiesZone6(EntityManager& em);
    void createEnemiesZone12(EntityManager& em);
    void createEnemiesZone11(EntityManager& em);

    void setCreatedtofalse(uint16_t z);
};

#endif // !MAP