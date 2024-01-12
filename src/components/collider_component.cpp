#include "collider_component.hpp"

void ColliderComponent::updateBox(vec3d pos, vec3d scale, double gravity, double)
{
    boundingBox.min = pos - (scale / 2);
    boundingBox.max = pos + (scale / 2);

    // Para tomar en cuenta la gravedad solo se le sume a la y cuando no esté en el suelo
    if (gravity == 0 && boundingBox.min.y() > 0.)
        boundingBox.min.setY(boundingBox.min.y() - 0.6);
    // boundingBox.min.setY(boundingBox.min.y() - 0.6f);
}

vec3d rotateY(vec3d point, vec3d center, double angle) {
    double s = sin(angle);
    double c = cos(angle);

    // translate point back to origin:
    point -= center;

    // rotate point
    double xnew = point.x() * c - point.z() * s;
    double znew = point.x() * s + point.z() * c;

    // translate point back:
    point.setX(xnew + center.x());
    point.setZ(znew + center.z());
    return point;
}