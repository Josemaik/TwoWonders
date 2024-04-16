#pragma once
#include <vector>
#include <memory>
#include <map>
#include "../utils/types.hpp"
#include "../utils/BBox.hpp"
#include "../components/collider_component.hpp"

struct Octree
{
    using OctMap = std::map<Entity*, ColliderComponent*>;

    Octree(uint8_t depth, const BBox& bounds, const Octree* parent = nullptr, std::size_t max = MAX_ENTITIES)
        : bounds_(bounds), depth_(depth), parent_(const_cast<Octree*>(parent)), max_ent_(max)
    {
        // octEntities_.reserve(max_ent_);
    };

    void insert(Entity& entity, ColliderComponent& collider);
    void subdivide(Entity& entity, ColliderComponent& collider);
    void clear();
    std::size_t countEntities() const;

    // Funciones en desuso
    //
    // void remove(std::pair<Entity*, ColliderComponent*> const& entity);
    // std::unordered_set<Octree*>& getNeighbors(Entity const& entity, ColliderComponent const& collider);
    // void getChildrenRecursive(Octree* node, Entity const& entity, ColliderComponent const& collider, std::unordered_set<Octree*>& neighbors);
    // void getParentsRecursive(Octree* node, Entity const& entity, ColliderComponent const& collider, std::unordered_set<Octree*>& neighbors);
    // bool query(Entity const& entity, ColliderComponent const& collider);
    // void updateNeighbors(Entity& entity, ColliderComponent& collider);

    // Función que nos devuelve las entidades en el octree
    template<typename T = std::pair<Entity*, ColliderComponent*>>
    auto getOctEntities() -> std::conditional_t<std::is_const_v<T>, const OctMap&, OctMap&> {
        return octEntities_;
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
    [[nodiscard]] std::size_t getMaxEntities() const noexcept { return max_ent_; }

    static const std::size_t MAX_ENTITIES = 25;
    static const std::size_t MAX_DEPTH = 10;
    static constexpr std::array<vec3d, 8> offsets =
    {
        vec3d(-0.5, -0.5, -0.5),
        vec3d(0.5, -0.5, -0.5),
        vec3d(-0.5, 0.5, -0.5),
        vec3d(0.5, 0.5, -0.5),
        vec3d(-0.5, -0.5, 0.5),
        vec3d(0.5, -0.5, 0.5),
        vec3d(-0.5, 0.5, 0.5),
        vec3d(0.5, 0.5, 0.5)
    };

private:
    OctMap octEntities_;
    std::array<std::unique_ptr<Octree>, 8> octants_{};
    BBox bounds_{};
    uint8_t depth_{};
    bool divided_ = false;
    Octree* parent_;
    std::size_t max_ent_{ MAX_ENTITIES };
    std::unordered_set<Octree*> neighbors;
};