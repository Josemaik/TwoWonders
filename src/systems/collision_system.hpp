#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER
#include "../utils/types.hpp"

constexpr float BORDER = 10.f;

struct CollisionSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent>;
    using SYSTAGs = MP::TypeList<>;
    void update(EntityManager& em);
    void checkCollision(EntityManager& em, std::vector<std::pair<Entity&, BBox>>& boxes);
    bool CheckCollisionBoxes(BBox box1, BBox box2);
};

#endif // !INPUT_MANAGER