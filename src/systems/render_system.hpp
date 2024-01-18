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

    void update(EntityManager& em, GameEngine& engine, bool debugphy, bool debugAI);

    void drawLogoGame(ENGI::GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawLogoKaiwa(ENGI::GameEngine& engine);
    void drawOptions(ENGI::GameEngine& engine, EntityManager& em, SoundSystem& ss);
    void drawEnding(ENGI::GameEngine& engine);
    void drawStory(ENGI::GameEngine& engine);
    void unloadModels(EntityManager& em, ENGI::GameEngine& engine);
    void drawEditorInGameIA(ENGI::GameEngine& engine,EntityManager& em);

    // Funciones privadas para organizar el codigo
private:
    void beginFrame(ENGI::GameEngine& engine);
    void endFrame(ENGI::GameEngine& engine, EntityManager& em, bool debugph,bool debugAI);
    void drawHUD(EntityManager& em, ENGI::GameEngine& engine, bool debugphy,bool debugAI);
    void drawEntities(EntityManager& em, ENGI::GameEngine& engine);
    void drawDeath(ENGI::GameEngine& engine);
};

#endif // !RENDER_SYSTEM
