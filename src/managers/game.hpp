#pragma once
#ifndef GAME
#define GAME
#include <iostream>
#include <cstdint>
#include "../utils/types.hpp"
#include "../systems/collision_system.hpp"
#include "../systems/render_system.hpp"
#include "../systems/physics_system.hpp"
#include "../systems/input_system.hpp"
#include "../systems/life_system.hpp"
#include "../systems/ai_system.hpp"
#include "../systems/attack_system.hpp"
#include "../systems/projectile_system.hpp"
#include "../systems/object_system.hpp"
#include "../systems/zone_system.hpp"
#include "../systems/shield_system.hpp"

#include "../systems/event_system.hpp"
#include "game_engine.hpp"

#include "map.hpp"

struct Game
{
    static constexpr uint16_t SCREEN_WIDTH{ 800 };
    static constexpr uint16_t SCREEN_HEIGHT{ 600 };

    Game() = default;
    ~Game() = default;

    void run();

private:
    void createEntities(EntityManager& em);

    void createSword(EntityManager& em);
    void createShield(EntityManager& em, Entity& ent);
    void createCoin(EntityManager& em);
    void createShop(EntityManager& em);
    void createEnding(EntityManager& em);
    void normalExecution(EntityManager& em, float deltaTime);
    void debugExecution(EntityManager& em);


void createEntities(EntityManager& em,Eventmanager& evm);
    GameEngine engine{ SCREEN_WIDTH, SCREEN_HEIGHT };
    EntityManager em{};
    Ia_man iam{};
    PhysicsSystem physics_system{};
    RenderSystem render_system{};
    InputSystem input_system{};
    CollisionSystem collision_system{};
    LifeSystem life_system{};
    AISystem   ai_system{};
    // GameTimer gtime{};
    AttackSystem attack_system{};
    ProjectileSystem projectile_system{};
    ObjectSystem object_system{};
    ZoneSystem zone_system{};
    ShieldSystem shield_system{};
    Map map{};
};


#endif // !GAME