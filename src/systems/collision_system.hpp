#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER
#include "../utils/Octree.hpp"
#include "../managers/event_manager.hpp"

constexpr float BORDER = 20.f;

struct CollisionSystem
{
    struct pair_hash
    {
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1, T2>& p) const
        {
            // Ordenar los elementos de los pares
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
            // Ordenar los elementos de los pares
            auto ordered_p1 = std::minmax(p1.first, p1.second);
            auto ordered_p2 = std::minmax(p2.first, p2.second);

            return ordered_p1 == ordered_p2;
        }
    };

    struct Ramp
    {
        double xMin, xMax, zMin, zMax, slope, xOffset, zOffset;
    };

    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<PhysicsComponent, ColliderComponent>;
    using SYSTAGs = MP::TypeList<>;
    using matrixType = std::vector<std::vector<bool>>;
    using pairsType = std::unordered_set<std::pair<std::size_t, std::size_t>, pair_hash, pair_equal>;
    using pairsType2 = std::set<std::pair<std::size_t, std::size_t>>;
    // using octreeMap = std::unordered_map<std::size_t, std::unordered_set<Octree*>>;

    CollisionSystem() :
        octree(0, octreeSizes[0]),
        checkedPairs(EntityManager::MAX_ENTITIES, std::vector<bool>(EntityManager::MAX_ENTITIES, false)) {}
    void update(EntityManager& em);
    void updateOctreeSize(uint8_t size);
    bool checkWallCollision(EntityManager& em, vec3d& pos, vec3d& normalDir);
    void setEventManager(EventManager& evm) { this->evm = &evm; }
private:
    void checkCollision(EntityManager& em, Octree& boxes);
    void handleRampCollision(EntityManager& em);
    void enemyCollision(EntityManager& em, Entity& damagedEntity);
    void staticCollision(PhysicsComponent& playerPhysics, PhysicsComponent& staticPhysics, vec3d& minOverlap);
    void nonStaticCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3d& minOverlap);
    void groundCollision(PhysicsComponent& playerPhysics, PhysicsComponent& groundPhy, vec3d& minOverlap);
    void floorCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3d& minOverlap);
    void handleCollision(EntityManager& em, Entity& entity1, Entity& entity2, vec3d& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2);
    void handleStaticCollision(EntityManager& em, Entity& entity1, Entity& entity2, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, vec3d& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2);
    void handleZoneCollision(EntityManager& em, Entity& entity1, Entity& entity2, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, BehaviorType behaviorType1, BehaviorType behaviorType2);
    void handlePlayerCollision(EntityManager& em, Entity& entity1, Entity& entity2, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, vec3d& minOverlap, BehaviorType behaviorType1, BehaviorType behaviorType2);
    void enemiesWallCollision(EntityManager& em, Entity& entity2, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, vec3d& minOverlap);
    void classicCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, vec3d& minOverlap);
    void resolveEnemyDirection(EntityManager& em, Entity& entity2, PhysicsComponent& staticPhy, PhysicsComponent& otherPhy, double overlap, bool isZAxis);
    void resolvePlayerDirection(PhysicsComponent& playerPhysics, PhysicsComponent& enemyPhy, bool isEnemy);
    template <auto getPos, auto setPos>
    bool resolveCollision(PhysicsComponent& phy1, PhysicsComponent& phy2, double overlap);
    void resetCollisionsMatrix();

    Octree octree;
    matrixType checkedPairs{};
    pairsType2 checkedPairsRamp{};
    std::vector<PhysicsComponent*> previousEntsOnRamp{};
    std::vector<std::pair<std::size_t, double>> rampHeights{};
    EventManager* evm{ nullptr };

    inline static constexpr std::array<BBox, 4> octreeSizes =
    {
        BBox({ -2.0, 5.0, 55.0 }, { 200.0, 25.0, 200.0 }),
        BBox({ -50.0, 21.0, 103.5 }, { 200.0, 43.5, 300.0 }),
        BBox({ -49.5, 26.0, -49.5 }, { 200.0, 56.0, 200.0 }),
        BBox({ -48.0, 35.5, 148.5 }, { 200.0, 71.0, 394.0 }),
    };

};

#endif // !INPUT_MANAGER