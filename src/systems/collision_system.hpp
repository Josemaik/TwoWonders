#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER
#include <iostream>
#include <raylib.h>
#include "../managers/entity_manager.hpp"
#include "../managers/game_engine.hpp"

struct CollisionSystem
{
    void update(EntityManager<PhysicsComponent, RenderComponent, Entity>& em);
};

#endif // !INPUT_MANAGER