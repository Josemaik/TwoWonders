#pragma once
#include <iostream>
#include <cmath>
#include <optional>
#include <raylib.h>

template <typename DataT>
struct vec3D
{
    constexpr vec3D() = default;
    constexpr vec3D(DataT x, DataT y, DataT z) : x_{ x }, y_{ y }, z_{ z } {}

    constexpr vec3D operator+(vec3D const& rhs) const
    {
        return { x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_ };
    }

    constexpr vec3D operator-() const
    {
        return vec3D{ -x_, -y_, -z_ };
    }

    constexpr vec3D operator-(vec3D const& rhs) const
    {
        return *this + (-rhs);
    }

    constexpr DataT dotProduct(vec3D const& rhs) const
    {
        return { x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_ };
    }

    constexpr vec3D operator*(DataT const s) const
    {
        return { x_ * s, y_ * s, z_ * s };
    }

    friend constexpr vec3D operator*(DataT const s, vec3D const& rhs)
    {
        return rhs * s;
    }

    constexpr vec3D operator*(const vec3D& rhs) const
    {
        return { x_ * rhs.x_, y_ * rhs.y_, z_ * rhs.z_ };
    }

    constexpr vec3D operator/(DataT const s) const
    {
        return *this * (1 / s);
    }

    constexpr vec3D& operator-=(vec3D const& rhs)
    {
        return *this += -rhs;
    }

    constexpr vec3D& operator*=(DataT const s)
    {
        x_ *= s;
        y_ *= s;
        z_ *= s;
        length_.reset();
        return *this;
    }

    constexpr vec3D& operator*=(vec3D const& rhs)
    {
        return *this = *this * rhs;
    }

    constexpr vec3D& operator/=(DataT const s)
    {
        return *this *= (1 / s);
    }

    constexpr DataT lengthSQ() const
    {
        return dotProduct(*this);
    }

    DataT length() const
    {
        if (!length_)
            length_ = sqrt(lengthSQ());
        return *length_;
    }

    constexpr vec3D& normalize() noexcept
    {
        vec3D v{ std::abs(x_), std::abs(y_), std::abs(z_) };
        *this /= length();

        // Aplicar la velocidad constante
        *this *= v;
        return *this;
    }
    constexpr vec3D normalized() const noexcept
    {
        auto v { *this };
        return v.normalize();
    }
    static constexpr vec3D min(const vec3D& a, const vec3D& b)
    {
        return { std::min(a.x_, b.x_), std::min(a.y_, b.y_), std::min(a.z_, b.z_) };
    }

    constexpr DataT x() const noexcept { return x_; }
    constexpr DataT y() const noexcept { return y_; }
    constexpr DataT z() const noexcept { return z_; }
    constexpr void setX(DataT const x) { x_ = x; }
    constexpr void setY(DataT const y) { y_ = y; }
    constexpr void setZ(DataT const z) { z_ = z; }

    constexpr auto operator<=>(vec3D const&) const = default;

    bool operator<(const vec3D& rhs) const
    {
        if (x_ < rhs.x_ || y_ < rhs.y_ || z_ < rhs.z_)
            return true;
        return false;
    }

    bool operator>(const vec3D& rhs) const
    {
        if (x_ > rhs.x_ || y_ > rhs.y_ || z_ > rhs.z_)
            return true;
        return false;
    }

    constexpr Vector3 toRaylib() const noexcept
    {
        return Vector3{ x_, y_, z_ };
    }

private:
    DataT x_{}, y_{}, z_{};
    mutable std::optional<DataT> length_{};
};

using vec3f = vec3D<float>;
using vec3d = vec3D<double>;

struct BBox
{
    vec3f min{};
    vec3f max{};
};