#pragma once

#include <cstdint>
#include <unordered_set>
#include "utils/types.hpp"

// Enum que representa el estado del juego
enum struct GameScreen { LOGO, TITLE, STORY, GAMEPLAY, /*DEAD,*/ OPTIONS, ENDING };

//Memoria global de nivel
struct LevelInfo
{

  // types for unordered_set of pairs
  struct pair_hash
  {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const
    {
      auto h1 = std::hash<T1>{}(p.first);
      auto h2 = std::hash<T2>{}(p.second);

      return h1 ^ h2;
    }
  };

  struct pair_equal
  {
    template <class T1, class T2>
    bool operator () (const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) const
    {
      return lhs.first == rhs.first && lhs.second == rhs.second;
    }
  };

  using NotLoadSet = std::unordered_set<std::pair<uint8_t, uint8_t>, pair_hash, pair_equal>;

  //Referencia al player
  std::size_t playerID;
  std::unordered_set<std::size_t> enemiesID{};
  NotLoadSet notLoadSet{};

  uint16_t num_zone{};
  bool drawzone{ false };
  int segundos{ 1000 };
  uint8_t mapID{ 0 };

  GameScreen currentScreen = GameScreen::LOGO;
};
