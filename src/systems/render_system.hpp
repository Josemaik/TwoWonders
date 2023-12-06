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

    bool update(EntityManager& em, GameEngine& engine);

    // Funciones privadas para organizar el codigo
private:
    void beginFrame(ENGI::GameEngine& engine);
    void endFrame(ENGI::GameEngine& engine, EntityManager& em);
    void drawHUD(EntityManager& em, ENGI::GameEngine& engine);
    bool drawEntities(EntityManager& em, ENGI::GameEngine& engine);
    void drawDeath(ENGI::GameEngine& engine);
};

#endif // !RENDER_SYSTEM