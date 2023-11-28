#pragma once
#ifndef INPUT_MANAGER
#define INPUT_MANAGER
#include <iostream>
#include <raylib.h>
#include "../managers/entity_manager.hpp"
#include "../managers/game_engine.hpp"

struct InputSystem
{
    using SYSCMPs = MP::TypeList<PhysicsComponent>;
    using SYSTAGs = MP::TypeList<PlayerTag>;
    void update(EntityManager& em)
    {
        em.forEach<SYSCMPs, SYSTAGs>([&em](Entity& e, PhysicsComponent& phy)
        {
            phy.velocity = {};
            if (IsKeyDown(KEY_D)) phy.velocity.x += .4f;
            if (IsKeyDown(KEY_A)) phy.velocity.x -= .4f;
            if (IsKeyDown(KEY_W)) phy.velocity.z -= .4f;
            if (IsKeyDown(KEY_S)) phy.velocity.z += .4f;
        });
    }
};

#endif // !INPUT_MANAGER