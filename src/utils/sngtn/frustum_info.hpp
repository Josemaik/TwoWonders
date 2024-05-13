#pragma once
#include <cstdint>
#include <array>
#include <unordered_set>
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
    void addToFrustum(std::size_t id);
    bool inFrustum(std::size_t id);

    bool pointInFrustum(vec3f const& point) const;
    bool pointInFrustum(vec3f const& point, float radius) const;
    void bboxIn(std::size_t id, const BBox& bbox);

private:
    std::array<Plane_t, 6> planes{};
    std::unordered_set<std::size_t> frustSet{};
};