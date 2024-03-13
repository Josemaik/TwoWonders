#pragma once

#include <cstdint>
#include <unordered_set>
#include <limits>
#include "utils/types.hpp"
#include "../../systems/sound_system.h"

// Enum que representa el estado del juego
enum struct GameScreen { LOGO, TITLE, STORY, GAMEPLAY, /*DEAD,*/ OPTIONS, ENDING, PAUSE };

//Memoria global de nivel
struct LevelInfo
{
  static constexpr std::size_t max = std::numeric_limits<std::size_t>::max();

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

  using notLoadSet = std::unordered_set<std::pair<uint8_t, uint8_t>, pair_hash, pair_equal>;
  using deathSet = std::set<std::size_t, std::greater<std::size_t>>;

  // Referencia al player
  std::size_t playerID;
  bool playerDetected{ false };

  // Variables de la cámara
  std::vector<vec3d> enemyPositions{};
  bool transition{ false };
  bool cameraChange{ false };
  vec3d viewPoint{};

  // Variables de lock on
  std::size_t lockedEnemy{ max };
  std::size_t closestEnemy{ max };

  // Variables de carga de entidades
  notLoadSet dontLoad{};
  deathSet dead_entities{};

  // Variables relacionadas con los eventos
  vec3d posWall{}, scaleWall{};
  std::size_t chestToOpen{ max };
  std::size_t doorToOpen{ max };
  bool dungeonKeyCreated{ false };

  // Variables de debug
  bool debugIA2{ false };
  bool resetGame{ false };

  // Variables de zona y el nivel
  uint16_t num_zone{};
  uint8_t mapID{ 0 };

  // Para estado de pausa y cerrar el juego
  SoundSystem* sound_system{ nullptr };
  bool gameShouldEnd{ false };

  // Tutorial
  std::vector<std::size_t> tutorialEnemies{};

  // Estado del juego
  GameScreen currentScreen = GameScreen::TITLE;
  GameScreen previousScreen = GameScreen::LOGO;

  void reset()
  {
    playerDetected = false;
    enemyPositions.clear();
    transition = false;
    cameraChange = false;
    lockedEnemy = max;
    closestEnemy = max;
    dontLoad.clear();
    dead_entities.clear();
    debugIA2 = false;
    resetGame = false;
    num_zone = 0;
    mapID = 0;
    chestToOpen = max;
    dungeonKeyCreated = false;
    sound_system = nullptr;
    gameShouldEnd = false;
  }
};
