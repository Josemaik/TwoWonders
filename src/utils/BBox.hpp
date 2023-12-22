#pragma once
#include "vec3D.hpp"

struct BBox
{
    vec3f min{};
    vec3f max{};

    BBox() = default;

    // Constructor de bounding box a partir de un centro un tamaño
    BBox(const vec3f& center, const vec3f& size)
    {
        min = center - size / 2.f;
        max = center + size / 2.f;
    }

    // Sacamos el solape entre dos bounding boxes
    [[nodiscard]] inline static constexpr vec3f minOverlap(const BBox& bbox1, const BBox& bbox2) noexcept
    {
        // vec3f overlapMin = vec3f::max(bbox1.min, bbox2.min);
        // vec3f overlapMax = vec3f::min(bbox1.max, bbox2.max);
        // vec3f overlapSize = overlapMax - overlapMin;
        // return overlapSize;

        vec3f overlap1 = (bbox1.max - bbox2.min);
        vec3f overlap2 = (bbox2.max - bbox1.min);

        return vec3f::min(overlap1, overlap2);
    }

    constexpr vec3f size() const { return max - min; }
    constexpr vec3f center() const { return (min + max) / 2.f; }
    bool intersects(const BBox& other) const { return !(max < other.min || min > other.max); }

    // Función para comprobar si un rayo intersecta con la caja delimitadora
    bool intersectsRay(vec3f& rayOrigin, vec3f& rayDirection) const {
        vec3f t1 = (min - rayOrigin) / rayDirection;
        vec3f t2 = (max - rayOrigin) / rayDirection;

        vec3f tmin = vec3f::min(t1, t2);
        vec3f tmax = vec3f::max(t1, t2);

        float tNear = tmin.max();
        float tFar = tmax.min();

        return tNear <= tFar && tFar >= 0;
    }
};