#pragma once
#include <vector>
#include <memory>
#include <set>
#include "../utils/types.hpp"
#include "../utils/BBox.hpp"
#include "../components/collider_component.hpp"

struct Octree {
    using OctVector = std::vector<std::pair<Entity*, ColliderComponent*>>;

    Octree(uint8_t depth, BBox bounds, Octree* parent = nullptr, uint8_t max = MAX_ENTITIES) : bounds(bounds), depth(depth), parent(parent), max_ent(max) {};
    void insert(Entity& entity, ColliderComponent& collider);
    void subdivide();
    OctVector query(const BBox& box);
    void remove(std::pair<Entity*, ColliderComponent*>& entity);
    std::size_t countEntities() const;
    void recombine();
    std::vector<Octree*> getNeighbors(ColliderComponent const& collider);
    void getChildrenRecursive(Octree* node, ColliderComponent const& collider, std::vector<Octree*>& neighbors, std::set<std::size_t>& removeFromNeighbors, std::size_t position);
    void getParentsRecursive(Octree* node, ColliderComponent const& collider, std::vector<Octree*>& neighbors);

    OctVector entities;
    std::unique_ptr<Octree> octants[8];
    BBox bounds{};
    uint8_t depth{};
    bool divided = false;
    Octree* parent;
    uint8_t max_ent{ MAX_ENTITIES };

    static const uint8_t MAX_ENTITIES = 10;
    static const uint8_t MAX_DEPTH = 10;
};