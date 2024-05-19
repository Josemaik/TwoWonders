#pragma once
#include <iostream>
#include <cmath>
#include <optional>
#include <glm/glm.hpp>
#include <array>
// Si queremos que la serialización sea en json, descomentar las siguientes líneas
// #define CEREAL_RAPIDJSON_NAMESPACE cereal_rapidjson
// #include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/archives/binary.hpp>

template <typename DataT>
struct vec3D
{
    constexpr vec3D() = default;
    constexpr vec3D(DataT x, DataT y, DataT z) : x_{ x }, y_{ y }, z_{ z } {}

    constexpr vec3D operator+(vec3D const& rhs) const
    {
        return { x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_ };
    }

    constexpr vec3D operator+(DataT const s) const
    {
        return { x_ + s, y_ + s, z_ + s };
    }

    constexpr vec3D operator-() const
    {
        return vec3D{ -x_, -y_, -z_ };
    }

    constexpr vec3D operator-(vec3D const& rhs) const
    {
        return *this + (-rhs);
    }

    constexpr vec3D operator-(DataT const s) const
    {
        return { x_ - s, y_ - s, z_ - s };
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

    DataT max() const {
        return std::max(std::max(x_, y_), z_);
    }

    DataT min() const {
        return std::min(std::min(x_, y_), z_);
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

    DataT operator[](std::size_t i) const
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

    constexpr glm::vec3 toGlm() const noexcept
    {
        return glm::vec3{ x_, y_, z_ };
    }

    friend std::ostream& operator<<(std::ostream& os, vec3D const& v)
    {
        os << '(' << v.x_ << ", " << v.y_ << ", " << v.z_ << ')';
        return os;
    }

    // Función para convertir en string
    constexpr std::string toString() const
    {
        return "[" + std::to_string(static_cast<int>(x_)) + ", " + std::to_string(static_cast<int>(y_)) + ", " + std::to_string(static_cast<int>(z_)) + "]";
    }

    // Función para convertir de string a vec3D
    static constexpr vec3D<double> fromString(std::string str)
    {
        // Eliminar los corchetes al principio y al final de la cadena
        if (!str.empty() && str.front() == '[') str.erase(str.begin());
        if (!str.empty() && str.back() == ']') str.erase(str.end() - 1);

        std::string delimiter = ", ";
        size_t pos = 0;
        std::string token;
        int i = 0;
        vec3D<double> v;
        while ((pos = str.find(delimiter)) != std::string::npos) {
            token = str.substr(0, pos);
            if (i == 0) v.setX(std::stod(token));
            if (i == 1) v.setY(std::stod(token));
            str.erase(0, pos + delimiter.length());
            i++;
        }
        v.setZ(std::stod(str));
        return v;
    }

    // Función para convertir un wstring a vec3D
    static constexpr vec3D<double> fromWString(std::wstring str)
    {
        // Lo pasamos a string
        std::string s(str.begin(), str.end());
        return fromString(s);
    }

    // Cuando DataT es float, pasarlo a double
    constexpr vec3D<double> toDouble() const noexcept
    {
        return vec3D<double>{ static_cast<double>(x_), static_cast<double>(y_), static_cast<double>(z_) };
    }

    constexpr vec3D<float> toFloat() const noexcept
    {
        return vec3D<float>{ static_cast<float>(x_), static_cast<float>(y_), static_cast<float>(z_) };
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

    // Devuelve el ángulo entre dos vectores en radianes
    constexpr DataT angle(vec3D const& rhs) const
    {
        return std::acos(dotProduct(rhs) / (length() * rhs.length()));
    }

    // Devuelve el ángulo entre dos vectores en grados
    constexpr DataT angleDeg(vec3D const& rhs) const
    {
        return angle(rhs) * 180 / 3.14159265358979323846;
    }

    constexpr DataT distance(vec3D const& rhs) const
    {
        return (rhs - *this).length();
    }

    template <typename Archive>
    void serialize(Archive& archive)
    {
        archive(x_, y_, z_);
    }

    constexpr vec3D<DataT> clamp(vec3D<DataT> const& min, vec3D<DataT> const& max) const
    {
        return vec3D<DataT>{ std::clamp(x_, min.x_, max.x_), std::clamp(y_, min.y_, max.y_), std::clamp(z_, min.z_, max.z_) };
    }

    //Calculate point distance
    constexpr DataT calculatePointDistance(vec3D const& target) const {
        auto dx{ target.x_ - x_ };
        auto dz{ target.z_ - z_ };
        return std::sqrt(dx * dx + dz * dz);
    }

    // Función para pasar de double a float o de float a double
    template<typename OtherT>
    vec3D<OtherT> to_other() const {
        return vec3D<OtherT>(static_cast<OtherT>(x_), static_cast<OtherT>(y_), static_cast<OtherT>(z_));
    }

    DataT* begin() { return &x_; }
    DataT* end() { return &z_ + 1; }

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

template <typename DataT>
struct vec2D
{
    constexpr vec2D() = default;
    constexpr vec2D(DataT x, DataT y) : x{ x }, y{ y } {}

    constexpr glm::vec2 toGlm() const noexcept
    {
        return glm::vec2{ x, y };
    }

    constexpr bool operator==(vec2D const& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    constexpr vec2D operator*(vec2D const& rhs) const
    {
        return { x * rhs.x, y * rhs.y };
    }

    constexpr vec2D operator-(vec2D const& rhs) const
    {
        return { x - rhs.x, y - rhs.y };
    }

    constexpr vec2D operator+(vec2D const& rhs) const
    {
        return { x + rhs.x, y + rhs.y };
    }

    constexpr vec2D operator/(vec2D const& rhs) const
    {
        return { x / rhs.x, y / rhs.y };
    }

    constexpr vec2D operator*(DataT const s) const
    {
        return { x * s, y * s };
    }

    constexpr vec2D operator/(DataT const s) const
    {
        return { static_cast<DataT>(x / s), static_cast<DataT>(y / s) };
    }

    constexpr vec2D operator*=(vec2D const& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    constexpr vec2D operator/= (vec2D const& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    constexpr vec2D operator*=(DataT const s)
    {
        x *= s;
        y *= s;
        return *this;
    }

    constexpr vec2D operator/=(DataT const s)
    {
        x /= s;
        y /= s;
        return *this;
    }

    constexpr vec2D operator-= (vec2D const& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    constexpr DataT lengthSQ() const
    {
        return x * x + y * y;
    }

    constexpr double length() const
    {
        if (!length_)
            length_ = std::sqrt(lengthSQ());
        return *length_;
    }

    constexpr double distance(vec2D const& rhs) const
    {
        return (rhs - *this).length();
    }

    constexpr double dotProduct(vec2D const& rhs) const
    {
        return x * rhs.x + y * rhs.y;
    }

    // Función para pasar de double a float o de float a double
    template<typename OtherT>
    vec2D<OtherT> to_other() const {
        return vec2D<OtherT>(static_cast<OtherT>(x), static_cast<OtherT>(y));
    }

    friend std::ostream& operator<<(std::ostream& os, vec2D const& v)
    {
        os << '(' << v.x << ", " << v.y << ')';
        return os;
    }

    mutable std::optional<DataT> length_{};
    DataT x{}, y{};
};

using vec2f = vec2D<float>;
using vec2d = vec2D<double>;
using vec2i = vec2D<int>;