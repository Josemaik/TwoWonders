#pragma once

#include <cstdint>
#include <unordered_set>
#include "utils/types.hpp"

// Enum que representa el estado del juego
enum struct GameScreen { LOGO, TITLE, GAMEPLAY, /*DEAD,*/ ENDING };

//Memoria global de nivel
struct LevelInfo
{
  //Referencia al player
  std::size_t playerID;
  std::unordered_set<std::size_t> enemiesID;

  uint16_t num_zone{};

  GameScreen currentScreen = GameScreen::LOGO;
};