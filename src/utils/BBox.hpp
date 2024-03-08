#pragma once
#include "vec3D.hpp"

struct BBox
{
    vec3d min{};
    vec3d max{};

    BBox() = default;

    // Constructor de bounding box a partir de un centro un tamaño
    BBox(const vec3d& center, const vec3d& size)
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
    bool intersects(const BBox& other) const { return !(max < other.min || min > other.max); }

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
};