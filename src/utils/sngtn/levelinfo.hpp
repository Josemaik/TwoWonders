#pragma once

#include <cstdint>
#include <unordered_set>
#include <limits>
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
  using deathSet = std::set<std::size_t, std::greater<std::size_t>>;

  //Referencia al player
  std::size_t playerID;
  bool playerDetected{ false };
  std::unordered_set<std::size_t> enemiesID{};
  std::vector<vec3d> enemyPositions{};
  bool transition{ false };
  bool cameraChange{ false };
  const std::size_t max = std::numeric_limits<std::size_t>::max();
  std::size_t lockedEnemy{ max };
  std::size_t closestEnemy{ max };
  NotLoadSet notLoadSet{};
  deathSet dead_entities{};

  std::size_t chestToOpen{ max };
  bool debugIA2{ false };
  bool resetGame{ false };

  uint16_t num_zone{};
  bool drawzone{ false };
  int segundos{ 1000 };
  uint8_t mapID{ 0 };

  GameScreen currentScreen = GameScreen::GAMEPLAY;

  void reset()
  {
    playerDetected = false;
    enemiesID.clear();
    enemyPositions.clear();
    transition = false;
    cameraChange = false;
    lockedEnemy = max;
    closestEnemy = max;
    notLoadSet.clear();
    dead_entities.clear();
    debugIA2 = false;
    resetGame = false;
    num_zone = 0;
    drawzone = false;
    segundos = 1000;
    mapID = 0;
  }

};
