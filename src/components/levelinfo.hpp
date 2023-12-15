#pragma once

#include <cstdint>
#include <unordered_set>
#include "utils/types.hpp"

//Memoria global de nivel
struct LevelInfo
{
  //Referencia al player
  std::size_t playerID;
  std::unordered_set<std::size_t> enemiesID;

  uint16_t num_zone{};
};
