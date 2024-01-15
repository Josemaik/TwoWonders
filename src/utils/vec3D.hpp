#pragma once
#include <iostream>
#include <cmath>
#include <optional>
#include <raylib.h>
#include <array>

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

    static constexpr DataT dotProduct(vec3D const& lhs, vec3D const& rhs)
    {
        return lhs.dotProduct(rhs);
    }

    constexpr vec3D crossProduct(vec3D const& rhs) const
    {
        return { y_ * rhs.z_ - z_ * rhs.y_, z_ * rhs.x_ - x_ * rhs.z_, x_ * rhs.y_ - y_ * rhs.x_ };
    }

    static constexpr vec3D crossProduct(vec3D const& lhs, vec3D const& rhs)
    {
        return lhs.crossProduct(rhs);
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

    constexpr vec3D operator/(vec3D const& rhs) const
    {
        return { x_ / rhs.x_, y_ / rhs.y_, z_ / rhs.z_ };
    }

    constexpr vec3D& operator+=(vec3D const& nhs) {
        x_ += nhs.x_;
        y_ += nhs.y_;
        z_ += nhs.z_;
        return *this;
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

    constexpr DataT length() const
    {
        if (!length_)
            length_ = sqrt(lengthSQ());
        return *length_;
    }

    constexpr vec3D& normalize()
    {
        auto const& len = length();

        if (len >= 1.f)
            *this /= len;
        else
            *this /= (1.f / len);

        return *this;
    }

    constexpr vec3D normalized() const noexcept
    {
        auto v{ *this };
        return v.normalize();
    }

    static constexpr vec3D min(const vec3D& a, const vec3D& b)
    {
        return { std::min(a.x_, b.x_), std::min(a.y_, b.y_), std::min(a.z_, b.z_) };
    }

    template<typename T>
    static constexpr vec3D min(std::array<vec3D<T>, 8> array)
    {
        vec3D<T> min = array[0];
        for (int i = 1; i < 8; i++) {
            if (array[i].x() < min.x()) min.setX(array[i].x());
            if (array[i].y() < min.y()) min.setY(array[i].y());
            if (array[i].z() < min.z()) min.setZ(array[i].z());
        }
        return min;
    }

    static constexpr vec3D max(const vec3D& a, const vec3D& b)
    {
        return { std::max(a.x_, b.x_), std::max(a.y_, b.y_), std::max(a.z_, b.z_) };
    }

    template<typename T>
    static vec3D max(std::array<vec3D<T>, 8> array)
    {
        vec3D<T> max = array[0];
        for (int i = 1; i < 8; i++) {
            if (array[i].x() > max.x()) max.setX(array[i].x());
            if (array[i].y() > max.y()) max.setY(array[i].y());
            if (array[i].z() > max.z()) max.setZ(array[i].z());
        }
        return max;
    }

    double max() const {
        return std::max({ x_, y_, z_ });
    }

    double min() const {
        return std::min({ x_, y_, z_ });
    }

    static constexpr vec3D abs(const vec3D& a)
    {
        return { std::abs(a.x_), std::abs(a.y_), std::abs(a.z_) };
    }

    constexpr vec3D abs()
    {
        return { std::abs(x_), std::abs(y_), std::abs(z_) };
    }

    static constexpr vec3D zero() { return { 0, 0, 0 }; }

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

    DataT operator[](int i) const
    {
        if (i == 0)
            return x_;
        else if (i == 1)
            return y_;
        else if (i == 2)
            return z_;
        else
            return 0;
    }

    constexpr Vector3 toRaylib() const noexcept
    {
        return Vector3{ static_cast<float>(x_),  static_cast<float>(y_),  static_cast<float>(z_) };
    }

    friend std::ostream& operator<<(std::ostream& os, vec3D const& v)
    {
        os << '(' << v.x_ << ", " << v.y_ << ", " << v.z_ << ')';
        return os;
    }

    void updateLowest(vec3D const& v) noexcept
    {
        if (x_ <= y_ && x_ <= z_) {
            x_ += v.x_ < 0 ? -v.x_ : v.x_;
        }
        else if (y_ <= x_ && y_ <= z_) {
            y_ += v.y_ < 0 ? -v.y_ : v.y_;
        }
        else {
            z_ += v.z_ < 0 ? -v.z_ : v.z_;
        }
    }

private:
    DataT x_{}, y_{}, z_{};
    mutable std::optional<DataT> length_{};
};

using vec3f = vec3D<float>;
using vec3d = vec3D<double>;

struct RayCast
{
    vec3d origin;
    vec3d direction;
};