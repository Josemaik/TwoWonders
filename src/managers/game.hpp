#pragma once
#ifndef GAME
#define GAME
#include "entity_manager.hpp"
#include "game_engine.hpp"
#include <iostream>
#include <cstdint>
#include "../systems/physics_system.hpp"
#include "../systems/render_system.hpp"
#include "../systems/input_system.hpp"
#include "../systems/collision_system.hpp"
#include "../utils/memory_viewer.hpp"
#include "../utils/slotmap.hpp"
#include "../utils/meta_program.hpp"
#include "../utils/types.hpp"

constexpr uint16_t SCREEN_WIDTH{ 800 };
constexpr uint16_t SCREEN_HEIGHT{ 600 };

void game();

#endif // !GAME