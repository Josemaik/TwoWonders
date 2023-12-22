#include "collider_component.hpp"

void ColliderComponent::updateBox(vec3f pos, vec3f scale, float gravity)
{
    boundingBox.min = pos - (scale / 2);
    boundingBox.max = pos + (scale / 2);

    // Para tomar en cuenta la gravedad solo se le sume a la y cuando no esté en el suelo
    if (gravity == 0 && boundingBox.min.y() > 0.f)
        boundingBox.min.setY(boundingBox.min.y() - 0.6f);
    // boundingBox.min.setY(boundingBox.min.y() - 0.6f);
}