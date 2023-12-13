#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER
#include <unordered_set>
#include "../utils/Octree.hpp"


constexpr float BORDER = 20.f;

struct CollisionSystem
{
    struct pair_hash
    {
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1, T2>& p) const
        {
            // Order the elements in the pair
            auto ordered_p = std::minmax(p.first, p.second);

            auto h1 = std::hash<T1>{}(ordered_p.first);
            auto h2 = std::hash<T2>{}(ordered_p.second);

            return h1 ^ h2;
        }
    };

    struct pair_equal
    {
        template <class T1, class T2>
        bool operator () (const std::pair<T1, T2>& p1, const std::pair<T1, T2>& p2) const
        {
            // Order the elements in the pairs
            auto ordered_p1 = std::minmax(p1.first, p1.second);
            auto ordered_p2 = std::minmax(p2.first, p2.second);

            return ordered_p1 == ordered_p2;
        }
    };

    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent, ColliderComponent>;
    using SYSTAGs = MP::TypeList<>;
    using pairsType = std::unordered_set<std::pair<std::size_t, std::size_t>, pair_hash, pair_equal>;

    void update(EntityManager& em);
private:
    void checkCollision(EntityManager& em, Octree& boxes, pairsType& checkedPairs);
    void enemyCollision(EntityManager& em, Entity& damagedEntity);
    void staticCollision(PhysicsComponent& playerPhysics, PhysicsComponent& staticPhysics, vec3f& minOverlap);
    void nonStaticCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap);
    void groundCollision(PhysicsComponent& playerPhysics, vec3f& playerEsc, vec3f& minOverlap);
    void floorCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap);
    void handleCollision(EntityManager& em, Entity& entity1, Entity& entity2, vec3f& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2);
    void handleStaticCollision(EntityManager& em, Entity& entity1, Entity& entity2, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, vec3f& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2);
    void handleZoneCollision(EntityManager& em, Entity& entity1, Entity& entity2, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, BehaviorType behaviorType1, BehaviorType behaviorType2);
    void handlePlayerCollision(EntityManager& em, Entity& entity1, Entity& entity2, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, vec3f& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2);
    void handleAtkCollision(EntityManager& em, bool& atkPl1, bool& atkPl2, bool& atkEn1, bool& atkEn2, Entity& entity1, Entity& entity2);
    void classicCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3f& minOverlap);
    void resolveCollisionX(PhysicsComponent& phy1, PhysicsComponent& phy2, float overlap);
    void resolveCollisionY(PhysicsComponent& phy1, PhysicsComponent& phy2, float overlap);
    void resolveCollisionZ(PhysicsComponent& phy1, PhysicsComponent& phy2, float overlap);
    // void checkBorderCollision(EntityManager& em, Octree& boxes);
};

#endif // !INPUT_MANAGER