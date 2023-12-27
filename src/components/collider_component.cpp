#include "collider_component.hpp"

void ColliderComponent::updateBox(vec3d pos, vec3d scale, double gravity)
{
    boundingBox.min = pos - (scale / 2);
    boundingBox.max = pos + (scale / 2);

    // Para tomar en cuenta la gravedad solo se le sume a la y cuando no esté en el suelo
    if (gravity == 0 && boundingBox.min.y() > 0.)
        boundingBox.min.setY(boundingBox.min.y() - 0.6);
    // boundingBox.min.setY(boundingBox.min.y() - 0.6f);
}