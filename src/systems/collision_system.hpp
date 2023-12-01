#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER
#include <unordered_map>
#include "../utils/types.hpp"

constexpr float BORDER = 10.f;

struct CollisionSystem
{
    using EntColPair = std::unordered_map<Entity*, ColliderComponent*>;

    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent, ColliderComponent>;
    using SYSTAGs = MP::TypeList<>;
    void update(EntityManager& em);
    void checkCollision(EntityManager& em, EntColPair& boxes);
    bool CheckCollisionBoxes(BBox box1, BBox box2);
    void checkBorderCollision(EntityManager& em, EntColPair& boxes);
    void enemyCollision(PhysicsComponent& playerPhysics, PhysicsComponent& enemyPhysics, vec3f& minOverlap);
    void staticCollision(PhysicsComponent& playerPhysics, PhysicsComponent& staticPhysics, vec3f& minOverlap);
    void nonPlayerCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap);
    void classicCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap);
};

#endif // !INPUT_MANAGER