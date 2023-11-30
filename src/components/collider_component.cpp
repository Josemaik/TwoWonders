#include "collider_component.hpp"

ColliderComponent::ColliderComponent(vec3f pos, vec3f scale)
{
    boundingBox = BBox{ pos, scale };
}

void ColliderComponent::updateBox(vec3f pos, vec3f scale)
{
    boundingBox.min = pos - (scale / 2);
    boundingBox.max = pos + (scale / 2);
}