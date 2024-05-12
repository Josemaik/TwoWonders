#pragma once

#include <vector>
#include "types.hpp"
#include "BBox.hpp"

struct BVHNode
{
    using BVHEnt = std::size_t;
    using BVHPair = std::pair<BVHEnt, BBox*>;
    using BVHVector = std::vector<BVHPair>;

    BBox bbox;
    // ColliderComponent* collider{ nullptr };
    BVHEnt entity{ max };

    BVHNode() = default;
    BVHNode(BVHVector& entities) { insert(entities); }
    void insert(BVHVector& entities);
    std::vector<BVHEnt> getEntities(const BBox& box);
    void clear();

private:
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;
    static BVHEnt max;
};