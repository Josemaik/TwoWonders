#pragma once
#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include "../systems/sound_system.hpp"

struct RenderSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, GameEngine& engine, double dt);

    void drawLogoGame(ENGI::GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawLogoKaiwa(ENGI::GameEngine& engine);
    void drawOptions(ENGI::GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawEnding(ENGI::GameEngine& engine);
    void drawStory(ENGI::GameEngine& engine);
    void unloadModels(EntityManager& em, ENGI::GameEngine& engine);
    void drawEditorInGameIA(ENGI::GameEngine& engine, EntityManager& em);
    void drawDebuggerInGameIA(ENGI::GameEngine& engine, EntityManager& em, double dt);
    void drawPauseMenu(ENGI::GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawInventory(ENGI::GameEngine& engine, EntityManager& em);

    // Funciones double dtprivadas para organizar el codigo
private:
    void beginFrame(ENGI::GameEngine& engine);
    void endFrame(ENGI::GameEngine& engine, EntityManager& em, double dt);
    void drawHUD(EntityManager& em, ENGI::GameEngine& engine, bool debugphy);
    void drawEntities(EntityManager& em, ENGI::GameEngine& engine);
    void drawDeath(ENGI::GameEngine& engine);
    void drawCoinBar(ENGI::GameEngine& engine, EntityManager& em);
    void drawHealthBar(ENGI::GameEngine& engine, EntityManager& em, const Entity& e);
    void drawManaBar(ENGI::GameEngine& engine, EntityManager& em);

    bool isSelected{ false };
    bool isSelectedfordebug{ false };
    std::size_t pointedEntity{ std::numeric_limits<std::size_t>::max() };
    bool jaja{ false };
    bool jaja2{ false };
};

#endif // !RENDER_SYSTEM
