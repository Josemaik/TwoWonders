#pragma once

#include <cstdint>

//Memoria global de nivel
struct LevelInfo
{
  //Referencia al player
  std::size_t playerID;

  uint16_t num_zone{};
};
