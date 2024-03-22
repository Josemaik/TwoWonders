#pragma once

#include <cstdint>
#include <unordered_set>
#include <limits>
#include "utils/types.hpp"
#include "../../systems/sound_system.hpp"

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
    std::size_t playerID{ max };
    bool playerDetected{ false };
    bool isDead{ false };
    bool resetFromDeath{ false };
    uint8_t deathCount{ 0 };

    // Variables de la cámara
    std::vector<vec3d> enemyPositions{};
    bool transition{ false };
    bool cameraChange{ false };
    vec3d viewPoint{};
    bool viewPointSound{ false };

    // Variables de lock on
    std::size_t lockedEnemy{ max };
    std::size_t closestEnemy{ max };

    // Variables de carga de entidades
    notLoadSet dontLoad{};

    // Pantalla de carga
    float loadingLimit{ 3.0f }, loadingTime{ loadingLimit };
    bool loading{ false };

    // Variables relacionadas con los eventos
    std::size_t chestToOpen{ max };
    std::size_t doorToOpen{ max };
    std::size_t npcToTalk{ max };
    bool dungeonKeyCreated{ false };
    vec3d enemyToChestPos{};

    // Variables de debug
    bool debugIA2{ false };
    bool resetGame{ false };

    // Variables de zona y el nivel
    uint16_t num_zone{};
    uint8_t mapID{ 0 };
    bool levelChanged{ false };

    // Para estado de pausa y cerrar el juego
    bool openChest{ false };
    bool gameShouldEnd{ false };

    // Tutorial
    std::vector<std::size_t> tutorialEnemies{};

    // Replay de inputs
    bool replay{ false };

    // Estado del juego
    GameScreen currentScreen = GameScreen::GAMEPLAY;
    GameScreen previousScreen = GameScreen::LOGO;

    bool isCharging()
    {
        return loadingTime < loadingLimit;
    }

    const deathSet& getDeath() const
    {
        return dead_entities;
    }

    void insertDeath(std::size_t id)
    {
        if (id != 0)
            dead_entities.insert(id);
        else if (!isDead)
        {
            isDead = true;
            deathCount += 1;
        }
    }

    void clearDeath()
    {
        dead_entities.clear();
    }

    void deathReset()
    {
        playerDetected = false;
        isDead = false;
        resetFromDeath = false;
        enemyPositions.clear();
        transition = false;
        cameraChange = false;
        lockedEnemy = max;
        closestEnemy = max;
    }

    void reset()
    {
        playerDetected = false;
        isDead = false;
        resetFromDeath = false;
        enemyPositions.clear();
        transition = false;
        cameraChange = false;
        lockedEnemy = max;
        closestEnemy = max;
        loadingTime = loadingLimit;
        loading = false;
        dontLoad.clear();
        dead_entities.clear();
        debugIA2 = false;
        resetGame = false;
        num_zone = 0;
        mapID = 0;
        chestToOpen = max;
        dungeonKeyCreated = false;
        openChest = false;
        gameShouldEnd = false;
        levelChanged = false;
    }

private:
    deathSet dead_entities{};
};
