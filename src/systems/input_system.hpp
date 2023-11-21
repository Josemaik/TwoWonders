#pragma once
#ifndef INPUT_MANAGER
#define INPUT_MANAGER
#include <iostream>
#include <raylib.h>
#include "../managers/entity_manager.hpp"
#include "../managers/game_engine.hpp"

struct InputSystem
{
    void update(EntityManager<PhysicsComponent, RenderComponent, Entity>& em);
};

#endif // !INPUT_MANAGER