#pragma once
#ifndef PHYSICS_SYSTEM
#define PHYSICS_SYSTEM
#include "../managers/entity_manager.hpp"
#include "../entities/entity.hpp"

struct PhysicsSystem
{
    void update(EntityManager<PhysicsComponent, RenderComponent, Entity>& em);
};

#endif // !PHYSICS_SYSTEM