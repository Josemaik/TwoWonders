#pragma once 
#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM
#include "../managers/entity_manager.hpp"
#include "../managers/game_engine.hpp"

struct RenderSystem
{
    void update(EntityManager<PhysicsComponent, RenderComponent, Entity>& em, GameEngine& engine);
};

#endif // !RENDER_SYSTEM