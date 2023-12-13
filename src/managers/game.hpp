#pragma once
#ifndef GAME
#define GAME
#include <iostream>
#include <cstdint>
#include "../utils/types.hpp"
#include "../utils/gametimer.hpp"
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

#include "map.hpp"

constexpr uint16_t SCREEN_WIDTH{ 800 };
constexpr uint16_t SCREEN_HEIGHT{ 600 };

void createEntities(EntityManager& em);
void createWalls(EntityManager& em);
void createEnemies(EntityManager& em);
void createGroundWater(EntityManager& em);
void game();

void createWallsZelda(EntityManager& em);
void createGroundWaterZelda(EntityManager& em);
void createEnemiesZelda(EntityManager& em);
struct EnemyData {
    vec3f position;
    vec3f velocity;
    std::array<vec3f, 10> route;
    float stop{};
    float detect_radius;
    int num_lifes;
    float Xmin{},Xmax{},Zmin{},Zmax{};
    bool visible{};
};
void createEnemiesofType(EntityManager& em,std::vector<EnemyData>vec,uint16_t type);
#endif // !GAME