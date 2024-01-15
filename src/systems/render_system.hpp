#pragma once
#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM
#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"

struct RenderSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, GameEngine& engine, bool debug);

    void drawLogoGame(ENGI::GameEngine& engine, EntityManager& em);
    void drawLogoKaiwa(ENGI::GameEngine& engine);
    void drawOptions(ENGI::GameEngine& engine, EntityManager& em);
    void drawEnding(ENGI::GameEngine& engine);
    void drawStory(ENGI::GameEngine& engine);
    void unloadModels(EntityManager& em, ENGI::GameEngine& engine);

    // Funciones privadas para organizar el codigo
private:
    void beginFrame(ENGI::GameEngine& engine);
    void endFrame(ENGI::GameEngine& engine, EntityManager& em, bool debug);
    void drawHUD(EntityManager& em, ENGI::GameEngine& engine, bool debug);
    void drawEntities(EntityManager& em, ENGI::GameEngine& engine);
    void drawDeath(ENGI::GameEngine& engine);
};

#endif // !RENDER_SYSTEM
