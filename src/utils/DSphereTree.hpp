#pragma once

#include <vector>
#include <memory>
#include <map>
#include "types.hpp"
#include "BBox.hpp" // Asegúrate de tener una clase Sphere que represente una esfera delimitadora
#include "../components/collider_component.hpp"

struct DSphereTree
{
    using SphereMap = std::vector<std::pair<Entity*, ColliderComponent*>>;
    using SphereType = std::array<std::unique_ptr<DSphereTree>, 2>; // Un DSphereTree generalmente se divide en 2

    DSphereTree(uint8_t depth, const Sphere& bounds, const DSphereTree* parent = nullptr, std::size_t max = MAX_ENTITIES)
        : bounds_(bounds), parent_(const_cast<DSphereTree*>(parent)), depth_(depth), max_ent_(max)
    {
        sphereEntities_.reserve(max_ent_);
    }

    void insert(Entity& entity, ColliderComponent& collider);
    void subdivide(Entity& entity, ColliderComponent& collider);
    void clear();
    std::size_t countEntities() const;

    template<typename T = std::pair<Entity*, ColliderComponent*>>
    auto getSphereEntities() -> std::conditional_t<std::is_const_v<T>, const SphereMap&, SphereMap&> {
        return sphereEntities_;
    }

    [[nodiscard]] std::size_t getNumEntities() const noexcept {
        return sphereEntities_.size();
    }

    [[nodiscard]] bool isDivided() const noexcept {
        return divided_;
    }

    [[nodiscard]] Sphere const& getBounds() const noexcept {
        return bounds_;
    }

    [[nodiscard]] SphereType const& getSpheres() const noexcept {
        return spheres_;
    }

    [[nodiscard]] uint8_t getDepth() const noexcept {
        return depth_;
    }

    [[nodiscard]] DSphereTree const* getParent() const noexcept {
        return parent_;
    }

    [[nodiscard]] std::size_t getMaxEntities() const noexcept {
        return max_ent_;
    }

    static const std::size_t MAX_ENTITIES = 100;
    static const uint8_t MAX_DEPTH = 20;
    static constexpr double offsets[2] = { -1.0, 1.0 };

private:
    Sphere bounds_;
    SphereMap sphereEntities_;
    SphereType spheres_{};
    DSphereTree* parent_{ nullptr };
    bool divided_{ false };
    uint8_t depth_{ 0 };
    std::size_t max_ent_{ MAX_ENTITIES };
};