#pragma once
#include <vector>
#include <memory>
#include <set>
#include "../utils/types.hpp"
#include "../utils/BBox.hpp"
#include "../components/collider_component.hpp"

struct Octree {
    using OctVector = std::vector<std::pair<Entity*, ColliderComponent*>>;

    Octree(uint8_t depth, const BBox& bounds, const Octree* parent = nullptr, uint8_t max = MAX_ENTITIES)
        : bounds_(bounds), depth_(depth), parent_(const_cast<Octree*>(parent)), max_ent_(max) {};

    void insert(Entity& entity, ColliderComponent& collider);
    void subdivide();
    OctVector query(const BBox& box);
    void remove(std::pair<Entity*, ColliderComponent*>& entity);
    std::size_t countEntities() const;
    void recombine();
    std::vector<Octree*> getNeighbors(ColliderComponent const& collider);
    void getChildrenRecursive(Octree* node, ColliderComponent const& collider, std::vector<Octree*>& neighbors, std::set<std::size_t>& removeFromNeighbors, std::size_t position);
    void getParentsRecursive(Octree* node, ColliderComponent const& collider, std::vector<Octree*>& neighbors);

    // Función que nos devuelve las entidades en el octree
    template<typename T = std::pair<Entity*, ColliderComponent*>>
    auto getOctEntities() -> std::span<std::conditional_t<std::is_const_v<T>, const std::pair<Entity*, ColliderComponent*>, std::pair<Entity*, ColliderComponent*>>>
    {
        return std::span{ octEntities_.begin(), octEntities_.end() };
    }

    // Función que nos devuelve el número de entidades en nodo
    [[nodiscard]] std::size_t getNumEntities() const noexcept { return octEntities_.size(); }

    // Función que nos dice si el nodo está dividido
    [[nodiscard]] bool isDivided() const noexcept { return divided_; }

    // Función que nos devuelve la bounding box del nodo
    [[nodiscard]] BBox const& getBounds() const noexcept { return bounds_; }

    // Función que nos devuelve los hijos del nodo
    [[nodiscard]] std::array<std::unique_ptr<Octree>, 8> const& getOctants() const noexcept { return octants_; }

    // Función que nos devuelve la profundidad del nodo
    [[nodiscard]] uint8_t getDepth() const noexcept { return depth_; }

    // Función que nos devuelve el padre del nodo
    [[nodiscard]] Octree const* getParent() const noexcept { return parent_; }

    // Función que nos devuelve el número máximo de entidades en el nodo
    [[nodiscard]] uint8_t getMaxEntities() const noexcept { return max_ent_; }

    static const uint8_t MAX_ENTITIES = 10;
    static const uint8_t MAX_DEPTH = 10;

private:
    OctVector octEntities_;
    std::array<std::unique_ptr<Octree>, 8> octants_{};
    BBox bounds_{};
    uint8_t depth_{};
    bool divided_ = false;
    Octree* parent_;
    uint8_t max_ent_{ MAX_ENTITIES };
};