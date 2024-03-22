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
#include "../systems/sound_system.hpp"
#include "../systems/camera_system.hpp"
#include "../systems/lock_system.hpp"
#include "../systems/npc_system.hpp"
#include "game_engine.hpp"
#include "ia_manager.hpp"
#include "map_manager.hpp"

struct Game
{
    static constexpr uint16_t SCREEN_WIDTH{ 1280 };
    static constexpr uint16_t SCREEN_HEIGHT{ 720 };

    Game() = default;
    ~Game() = default;

    void run();

private:
    void createEntities();
    void createSword();
    void createShield(Entity& ent);
    void createCoin();
    void createShop();
    void createEnding();
    void createSound();
    void resetGame();
    void resetDeath();
    Shader createShader();

    GameEngine engine{ SCREEN_WIDTH, SCREEN_HEIGHT };
    EntityManager em{};
    EventManager evm{};
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
    EventSystem event_system{};
    MapManager map{};
    CameraSystem camera_system{};
    LockSystem lock_system{};
    NPCSystem npc_system{};
};


#endif // !GAME
