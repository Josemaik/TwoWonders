#pragma once
#include <vector>
#include <memory>
#include <set>
#include <unordered_set>
#include "../utils/types.hpp"
#include "../utils/BBox.hpp"
#include "../components/collider_component.hpp"

struct Octree {

    struct pair_hash
    {
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1*, T2*>& p) const
        {
            auto h1 = std::hash<T1*>{}(p.first);
            auto h2 = std::hash<T2*>{}(p.second);

            return h1 ^ h2;
        }
    };

    struct pair_equal
    {
        template <class T1, class T2>
        bool operator () (const std::pair<T1*, T2*>& lhs, const std::pair<T1*, T2*>& rhs) const
        {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
    };

    using OctSet = std::unordered_set<std::pair<Entity*, ColliderComponent*>, pair_hash, pair_equal>;

    Octree(uint8_t depth, const BBox& bounds, const Octree* parent = nullptr, std::size_t max = MAX_ENTITIES)
        : bounds_(bounds), depth_(depth), parent_(const_cast<Octree*>(parent)), max_ent_(max)
    {
        octEntities_.reserve(max_ent_);
    };

    void insert(Entity& entity, ColliderComponent& collider);
    void subdivide();
    std::size_t countEntities() const;
    void remove(std::pair<Entity*, ColliderComponent*> const& entity);
    std::unordered_set<Octree*> getNeighbors(Entity const& entity, ColliderComponent const& collider);
    void getChildrenRecursive(Octree* node, Entity const& entity, ColliderComponent const& collider, std::unordered_set<Octree*>& neighbors);
    void getParentsRecursive(Octree* node, Entity const& entity, ColliderComponent const& collider, std::unordered_set<Octree*>& neighbors);
    bool query(Entity const& entity, ColliderComponent const& collider);
    // Función que nos devuelve las entidades en el octree
    template<typename T = std::pair<Entity*, ColliderComponent*>>
    auto getOctEntities() -> std::conditional_t<std::is_const_v<T>, const OctSet&, OctSet&> {
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

    static const std::size_t MAX_ENTITIES = 15;
    static const std::size_t MAX_DEPTH = 10;
    static constexpr std::array<vec3f, 8> offsets =
    {
        vec3f(-0.5f, -0.5f, -0.5f),
        vec3f(0.5f, -0.5f, -0.5f),
        vec3f(-0.5f, 0.5f, -0.5f),
        vec3f(0.5f, 0.5f, -0.5f),
        vec3f(-0.5f, -0.5f, 0.5f),
        vec3f(0.5f, -0.5f, 0.5f),
        vec3f(-0.5f, 0.5f, 0.5f),
        vec3f(0.5f, 0.5f, 0.5f)
    };

private:
    OctSet octEntities_;
    std::array<std::unique_ptr<Octree>, 8> octants_{};
    BBox bounds_{};
    uint8_t depth_{};
    bool divided_ = false;
    Octree* parent_;
    std::size_t max_ent_{ MAX_ENTITIES };
};