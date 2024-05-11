#include "BVH.hpp"

BVHNode::BVHEnt BVHNode::max = std::numeric_limits<BVHNode::BVHEnt>::max();

void BVHNode::insert(BVHVector& entities)
{
    // Compute bounding box that contains all entities
    for (const auto& [entity, entBox] : entities) {
        bbox.expand(*entBox);
    }

    // If there's more than one entity, split the list and create children
    if (entities.size() > 1) {
        int axis = bbox.longestAxis();
        std::sort(entities.begin(), entities.end(),
            [axis](const BVHPair& a, const BVHPair& b) {
            if (axis == 0) // x-axis
                return a.second->center().x() < b.second->center().x();
            else if (axis == 1) // y-axis
                return a.second->center().y() < b.second->center().y();
            else // z-axis
                return a.second->center().z() < b.second->center().z();
        });

        // Split the list of entities in two and create children
        std::size_t mid = entities.size() / 2;
        BVHVector leftEntities(entities.begin(), entities.begin() + mid);
        BVHVector rightEntities(entities.begin() + mid, entities.end());
        left = std::make_unique<BVHNode>(leftEntities);
        right = std::make_unique<BVHNode>(rightEntities);
    }
    else if (entities.size() == 1) {
        // If there's only one entity, this is a leaf node
        entity = entities[0].first;
        // collider = entities[0].second;
        left = nullptr;
        right = nullptr;
    }
}

std::vector<BVHNode::BVHEnt> BVHNode::getEntities(const BBox& box)
{
    std::vector<BVHNode::BVHEnt> result{};
    if (bbox.intersects(box)) {
        if (entity != max) {
            result.push_back(entity);
            entity = max;
        }
        else {
            if (left != nullptr) {
                auto leftResult = left->getEntities(box);
                result.insert(result.end(), leftResult.begin(), leftResult.end());
            }
            if (right != nullptr) {
                auto rightResult = right->getEntities(box);
                result.insert(result.end(), rightResult.begin(), rightResult.end());
            }
        }
    }
    return result;
}

void BVHNode::clear()
{
    if (left != nullptr) {
        left->clear();
        left = nullptr;
    }
    if (right != nullptr) {
        right->clear();
        right = nullptr;
    }
}