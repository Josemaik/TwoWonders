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
#include "../systems/ai.hpp"
#include "../systems/attack_system.hpp"

constexpr uint16_t SCREEN_WIDTH{ 800 };
constexpr uint16_t SCREEN_HEIGHT{ 600 };

void createEntities(EntityManager& em);
void createWalls(EntityManager& em);
void createEnemies(EntityManager& em);
void createGroundWater(EntityManager& em);
void game();

#endif // !GAME