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

    // Funciones privadas para organizar el codigo
    private:
        void beginFrame(ENGI::GameEngine&  engine, EntityManager& em);
        void endFrame(ENGI::GameEngine&  engine);
        void drawHUD(EntityManager& em);
};

#endif // !RENDER_SYSTEM