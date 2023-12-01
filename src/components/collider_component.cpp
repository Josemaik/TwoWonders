#include "collider_component.hpp"

void ColliderComponent::updateBox(vec3f pos, vec3f scale)
{
    boundingBox.min = pos - (scale / 2);
    boundingBox.max = pos + (scale / 2);
}