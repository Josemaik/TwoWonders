#pragma once
#include "vec3D.hpp"

struct BBox
{
    vec3d min{};
    vec3d max{};

    constexpr BBox() = default;

    // Constructor de bounding box a partir de un centro un tamaño
    constexpr BBox(const vec3d& center, const vec3d& size)
    {
        min = center - size / 2.0;
        max = center + size / 2.0;
    }

    // Sacamos el solape entre dos bounding boxes
    [[nodiscard]] inline static constexpr vec3d minOverlap(const BBox& bbox1, const BBox& bbox2) noexcept
    {
        // vec3d overlapMin = vec3d::max(bbox1.min, bbox2.min);
        // vec3d overlapMax = vec3d::min(bbox1.max, bbox2.max);
        // vec3d overlapSize = overlapMax - overlapMin;
        // return overlapSize;

        vec3d overlap1 = (bbox1.max - bbox2.min);
        vec3d overlap2 = (bbox2.max - bbox1.min);

        return vec3d::min(overlap1, overlap2);
    }

    constexpr vec3d size() const { return max - min; }
    constexpr vec3d center() const { return (min + max) / 2.0; }
    bool intersects(const BBox& other) const {
        return !(max.x() < other.min.x() || min.x() > other.max.x() ||
            max.y() < other.min.y() || min.y() > other.max.y() ||
            max.z() < other.min.z() || min.z() > other.max.z());
    }

    bool intersects(const vec3d& point) const {
        return !(point.x() < min.x() || point.x() > max.x() ||
            point.y() < min.y() || point.y() > max.y() ||
            point.z() < min.z() || point.z() > max.z());
    }
    // Función para comprobar si un rayo intersecta con la caja delimitadora
    bool intersectsRay(vec3d& rayOrigin, vec3d& rayDirection) const {
        vec3d t1 = (min - rayOrigin) / rayDirection;
        vec3d t2 = (max - rayOrigin) / rayDirection;

        vec3d tmin = vec3d::min(t1, t2);
        vec3d tmax = vec3d::max(t1, t2);

        double tNear = tmin.max();
        double tFar = tmax.min();

        return tNear <= tFar && tFar >= 0;
    }

    // Función para comprobar si un rayo intersecta y además devuelve el punto de intersección
    bool intersectsRay(vec3d& rayOrigin, vec3d& rayDirection, vec3d& intersectionPoint) const {
        vec3d t1 = (min - rayOrigin) / rayDirection;
        vec3d t2 = (max - rayOrigin) / rayDirection;

        vec3d tmin = vec3d::min(t1, t2);
        vec3d tmax = vec3d::max(t1, t2);

        double tNear = tmin.max();
        double tFar = tmax.min();

        if (tNear <= tFar && tFar >= 0) {
            intersectionPoint = rayOrigin + rayDirection * tNear;
            return true;
        }
        return false;
    }

    // Función para comprobar si el rayo intersecta y además devuelve el punto de intersección más alejado
    bool intersectsRay2(vec3d& rayOrigin, vec3d& rayDirection, vec3d& intersectionPoint) const {
        vec3d t1 = (min - rayOrigin) / rayDirection;
        vec3d t2 = (max - rayOrigin) / rayDirection;

        vec3d tmin = vec3d::min(t1, t2);
        vec3d tmax = vec3d::max(t1, t2);

        double tNear = tmin.max();
        double tFar = tmax.min();

        if (tNear <= tFar && tFar >= 0) {
            intersectionPoint = rayOrigin + rayDirection * tFar;
            return true;
        }
        return false;
    }

    vec3d getPositiveVertex(const vec3d& normal) const {
        vec3d res = min;
        if (normal.x() >= 0) res.setX(max.x());
        if (normal.y() >= 0) res.setY(max.y());
        if (normal.z() >= 0) res.setZ(max.z());
        return res;
    }

    vec3d getNegativeVertex(const vec3d& normal) const {
        vec3d res = max;
        if (normal.x() >= 0) res.setX(min.x());
        if (normal.y() >= 0) res.setY(min.y());
        if (normal.z() >= 0) res.setZ(min.z());
        return res;
    }

    const std::array<vec3f, 8> getCorners() const {
        return {
            vec3d(min.x(), min.y(), min.z()).to_other<float>(),
            vec3d(max.x(), min.y(), min.z()).to_other<float>(),
            vec3d(max.x(), max.y(), min.z()).to_other<float>(),
            vec3d(min.x(), max.y(), min.z()).to_other<float>(),
            vec3d(min.x(), min.y(), max.z()).to_other<float>(),
            vec3d(max.x(), min.y(), max.z()).to_other<float>(),
            vec3d(max.x(), max.y(), max.z()).to_other<float>(),
            vec3d(min.x(), max.y(), max.z()).to_other<float>()
        };
    }

    void expand(const BBox& other) {
        min = vec3d::min(min, other.min);
        max = vec3d::max(max, other.max);
    }

    int longestAxis() const {
        vec3d size = this->size();
        if (size.x() > size.y() && size.x() > size.z()) return 0;
        if (size.y() > size.z()) return 1;
        return 2;
    }
};

struct Sphere
{
    vec3d center{};
    double radius{};

    Sphere() = default;
    Sphere(const vec3d& center, double radius) : center(center), radius(radius) {}

    static Sphere boundingSphere(const std::vector<vec3d>& points) {
        vec3d min = points[0];
        vec3d max = points[0];
        for (const auto& point : points) {
            min = vec3d::min(min, point);
            max = vec3d::max(max, point);
        }
        vec3d center = (min + max) / 2.0;
        double radius = 0.0;
        for (const auto& point : points) {
            double distance = (point - center).length();
            radius = std::max(radius, distance);
        }
        return Sphere(center, radius);
    }

    bool intersects(const BBox& bbox) const {
        vec3d closestPoint = bbox.center().clamp(bbox.min, bbox.max);
        vec3d distance = closestPoint - center;
        return distance.length() <= radius * radius;
    }

    bool intersects(const Sphere& other) const {
        vec3d distance = other.center - center;
        return distance.length() <= (radius + other.radius) * (radius + other.radius);
    }

    bool intersects(const vec3d& point) const {
        vec3d distance = point - center;
        return distance.length() <= radius * radius;
    }

    double size() const { return radius * 2.0; }
};