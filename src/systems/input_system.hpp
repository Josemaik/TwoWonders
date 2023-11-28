#pragma once
#ifndef INPUT_MANAGER
#define INPUT_MANAGER
#include <iostream>
#include <raylib.h>
#include "../managers/entity_manager.hpp"
#include "../managers/game_engine.hpp"

struct InputSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent>;
    using SYSTAGs = MP::TypeList<PlayerTag>;
    void update(EntityManager& em)
    {
        em.forEach<SYSCMPs, SYSTAGs>([&em](Entity&, PhysicsComponent& phy)
        {
            phy.velocity = {};
            if (IsKeyDown(KEY_D)) phy.velocity.x += .3f;
            if (IsKeyDown(KEY_A)) phy.velocity.x -= .3f;
            if (IsKeyDown(KEY_W)) phy.velocity.z -= .3f;
            if (IsKeyDown(KEY_S)) phy.velocity.z += .3f;
        });
    }
};

#endif // !INPUT_MANAGER