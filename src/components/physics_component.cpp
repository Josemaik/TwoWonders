#include "physics_component.hpp"

BBox PhysicsComponent::getBoundingBox(vec3f scale)
{
    vec3f min = position - (scale / 2);
    vec3f max = position + (scale / 2);

    return BBox{ min, max };
}