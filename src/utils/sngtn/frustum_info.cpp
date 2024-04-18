#include "frustum_info.hpp"

void FrustumInfo::setFrustum(float left, float right, float bottom, float top, float near, float far, vec3f const& pos, vec3f const& target, vec3f const& up)
{
    vec3f Z = (target - pos).normalize();
    vec3f X = up.crossProduct(Z).normalize();
    vec3f Y = Z.crossProduct(X);

    vec3f nc = pos - Z * near;
    vec3f fc = pos - Z * far;

    planes[static_cast<uint8_t>(Plane::NEAR)] = { Z, nc };
    planes[static_cast<uint8_t>(Plane::FAR)] = { Z, fc };

    vec3f aux, normal;

    aux = (nc + Y * top);
    normal = pos - aux;
    planes[static_cast<uint8_t>(Plane::TOP)] = { normal.normalize(), aux };

    aux = (nc - Y * bottom);
    normal = pos - aux;
    planes[static_cast<uint8_t>(Plane::BOTTOM)] = { normal.normalize(), aux };

    aux = (nc - X * left);
    normal = pos - aux;
    planes[static_cast<uint8_t>(Plane::LEFT)] = { normal.normalize(), aux };

    aux = (nc + X * right);
    normal = pos - aux;
    planes[static_cast<uint8_t>(Plane::RIGHT)] = { normal.normalize(), aux };

    fr_ents.clear();
    outside_ents.clear();
}

FrustumInfo::Position FrustumInfo::bboxInFrustum(const BBox& bbox)
{
    // Comprobar si la caja delimitadora ya ha sido comprobada y está fuera del frustum
    if (outside_ents.find(&bbox) != outside_ents.end())
        return Position::OUTSIDE;

    // Comprobar si la caja delimitadora ya ha sido comprobada
    if (fr_ents.find(&bbox) != fr_ents.end())
        return Position::INSIDE;

    // Comprobar si el centro de la caja delimitadora está dentro del frustum
    vec3f center = bbox.center().to_other<float>();
    if (pointInFrustum(center) == Position::INSIDE)
    {
        fr_ents.insert(&bbox);
        return Position::INSIDE;
    }

    // Si el centro no está dentro, comprobar las esquinas
    for (int i = 0; i < 8; ++i)
    {
        vec3f corner = vec3f(
            i & 1 ? static_cast<float>(bbox.min.x()) : static_cast<float>(bbox.max.x()),
            i & 2 ? static_cast<float>(bbox.min.y()) : static_cast<float>(bbox.max.y()),
            i & 4 ? static_cast<float>(bbox.min.z()) : static_cast<float>(bbox.max.z())
        );
        if (pointInFrustum(corner) == Position::INSIDE)
        {
            fr_ents.insert(&bbox);
            return Position::INSIDE;
        }
    }

    outside_ents.insert(&bbox);
    return Position::OUTSIDE;
}

FrustumInfo::Position FrustumInfo::pointInFrustum(vec3f const& point, float radius) const
{
    for (auto const& plane : planes)
    {
        if (plane.distance(point) < -radius)
            return Position::OUTSIDE;
    }
    return Position::INSIDE;
}

FrustumInfo::Position FrustumInfo::pointInFrustum(vec3f const& point) const
{
    for (auto const& plane : planes)
    {
        if (plane.distance(point) < 0)
            return Position::OUTSIDE;
    }
    return Position::INSIDE;
}