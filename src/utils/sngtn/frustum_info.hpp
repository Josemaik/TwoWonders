#pragma once
#include <cstdint>
#include <array>
#include <set>
#include "../BBox.hpp"

struct FrustumInfo
{
    enum struct Plane : uint8_t
    {
        NEAR = 0,
        FAR,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    };

    enum struct Position : uint8_t
    {
        INSIDE = 0,
        OUTSIDE,
        INTERSECT
    };

    struct Plane_t
    {
        float distance(vec3f const& point) const
        {
            return (point - p).dotProduct(normal);
        }
        vec3f normal;
        vec3f p;
    };

    FrustumInfo() = default;
    ~FrustumInfo() = default;

    void setFrustum(float left, float right, float bottom, float top, float near, float far, vec3f const& pos, vec3f const& dir, vec3f const& up);
    void calculateCorners(vec3f pos, vec3f target, vec3f up, float fovy, float near, float far);

    Position pointInFrustum(vec3f const& point) const;
    Position pointInFrustum(vec3f const& point, float radius) const;
    Position bboxInFrustum(const BBox& bbox);

private:
    std::array<Plane_t, 6> planes{};
    std::set<const BBox*> fr_ents{};
};