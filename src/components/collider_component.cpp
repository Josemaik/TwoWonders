#include "collider_component.hpp"

void ColliderComponent::updateBox(vec3d pos, vec3d scale, double gravity, double orientation, vec3d& rotationVec)
{
    orientation *= std::abs(rotationVec.y());
    float orientationInDegrees = static_cast<float>(orientation * (180.0f / M_PI));
    int orientationInDegreesInt = static_cast<int>(orientationInDegrees);

    if (std::abs(orientationInDegreesInt) == 270 || std::abs(orientationInDegreesInt) == 90)
    {
        scale = { scale.z(), scale.y(), scale.x() };
    }

    vec3d min = pos - (scale / 2);
    vec3d max = pos + (scale / 2);

    // Para tomar en cuenta la gravedad, se aumenta la altura de la bounding box
    if (gravity > 0.)
        max.setY(max.y() + 0.5);

    // float orientationInDegrees = static_cast<float>(orientation * (180.0f / M_PI));
    // int orientationInDegreesInt = static_cast<int>(orientationInDegrees);

    // if (std::abs(orientationInDegreesInt) == 270 || std::abs(orientationInDegreesInt) == 90)
    // {
    //     vec3d vertices[8] = {
    //      {min.x(), min.y(), min.z()},
    //      {max.x(), min.y(), min.z()},
    //      {max.x(), max.y(), min.z()},
    //      {min.x(), max.y(), min.z()},
    //      {min.x(), min.y(), max.z()},
    //      {max.x(), min.y(), max.z()},
    //      {max.x(), max.y(), max.z()},
    //      {min.x(), max.y(), max.z()}
    //     };

    //     double cosAngle = cos(orientation);
    //     double sinAngle = sin(orientation);

    //     for (int i = 0; i < 8; i++)
    //     {
    //         vec3d temp = vertices[i] - pos;
    //         rotatePointY(temp, cosAngle, sinAngle);
    //         vertices[i] = temp + pos;
    //     }

    //     for (int i = 0; i < 8; i++)
    //     {
    //         min = vec3d::min(boundingBox.min, vertices[i]);
    //         max = vec3d::max(boundingBox.max, vertices[i]);
    //     }
    // }

    boundingBox.min = min;
    boundingBox.max = max;
}

void ColliderComponent::rotatePointY(vec3d& point, double cosAngle, double sinAngle)
{
    double x = point.x() * cosAngle + point.z() * sinAngle;
    double z = -point.x() * sinAngle + point.z() * cosAngle;

    point.setX(x);
    point.setZ(z);
}