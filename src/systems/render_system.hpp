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

    void update(EntityManager& em, GameEngine& engine);

    void drawLogoGame(ENGI::GameEngine& engine);
    void drawLogoKaiwa(ENGI::GameEngine& engine);
    void drawEnding(ENGI::GameEngine& engine);

    // Funciones privadas para organizar el codigo
private:
    void beginFrame(ENGI::GameEngine& engine);
    void endFrame(ENGI::GameEngine& engine, EntityManager& em);
    void drawHUD(EntityManager& em, ENGI::GameEngine& engine);
    void drawEntities(EntityManager& em, ENGI::GameEngine& engine);
    void drawDeath(ENGI::GameEngine& engine);
};

#endif // !RENDER_SYSTEM