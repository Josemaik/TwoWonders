#include "physics_component.hpp"

BoundingBox PhysicsComponent::getBoundingBox(Vector3 scale)
{
    Vector3 min = { position.x - scale.x / 2, position.y - scale.y / 2, position.z - scale.z / 2 };
    Vector3 max = { position.x + scale.x / 2, position.y + scale.y / 2, position.z + scale.z / 2 };

    return BoundingBox{ min, max };
}