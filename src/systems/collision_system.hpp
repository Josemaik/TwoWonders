#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER
#include <unordered_map>
#include "../utils/types.hpp"

constexpr float BORDER = 20.f;

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
    void enemyCollision(EntityManager& em, Entity* damagedEntity);
    void staticCollision(PhysicsComponent& playerPhysics, PhysicsComponent& staticPhysics, vec3f& minOverlap);
    void nonStaticCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap);
    void bulletCollision(bool& bulletPl1, bool& bulletPl2, bool& bulletEn1, bool& bulletEn2, EntityManager& em, Entity* entity1, Entity* entity2);
    void groundCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap);
    void handleCollision(EntityManager& em, Entity* entity1, Entity* entity2, vec3f& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2);
    void classicCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap);

private:
    float posY{};
    float gravity{};
};

#endif // !INPUT_MANAGER