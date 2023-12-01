#pragma once 
#ifndef LIFE_SYSTEM
#define LIFE_SYSTEM
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"

struct LifeSystem
{
    // using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent>;
    // using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em) {
        for (auto const& e : em.getEntities())
        {
            if (e.hasComponent<LifeComponent>())
            {
                auto& l{ em.getComponent<LifeComponent>(e) };
                l.decreaseCountdown();
            }
        }
    };
};

#endif // !RENDER_SYSTEM