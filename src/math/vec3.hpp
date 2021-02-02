#pragma once

#include "utils/types.hpp"
#include "math/vec2.hpp"
#include <fmt/core.h>
#include <string>
#include <cmath>

namespace Aery {
    template<typename vec1>
    class base_vec3 {
        static_assert(std::is_arithmetic<vec1>(), "vec1 must be a number type.");
    public:
        vec1 x, y, z;

        base_vec3(const base_vec3& Other) { x = Other.x; y = Other.y; z = Other.z; }
        base_vec3(const base_vec2& Other) { x = Other.x; y = Other.y; z = 0; }
        base_vec3(const vec1 x, const vec1 y, const vec1 z) { this->x = x; this->y = y; this->z = z; }
        base_vec3(const vec1 x, const vec1 y) { this->x = x; this->y = y; z = 0; }
        base_vec3(const vec1 a) { x = a; y = a; }
        base_vec3() { x = 0; y = 0; }

        base_vec3 normalize() const { return *this / length(); }

        inline vec1 length() const { return sqrt((x * x) + (y * y)); }
        inline vec1 magnitude() const { return sqrt((x * x) + (y * y)); }
        inline vec1 dot(const base_vec3& Other) const { return (x * Other.x) + (y * Other.y); }
        inline vec1 angle(const base_vec3& Other) const { return dot(Other) / (length() * Other.length()); }

        base_vec3 operator+(const base_vec3& Other) const { return base_vec3(x + Other.x, y + Other.y, z + Other.z); }
        base_vec3 operator-(const base_vec3& Other) const { return base_vec3(x - Other.x, y - Other.y, z - Other.z); }
        base_vec3 operator*(const base_vec3& Other) const { return base_vec3(x * Other.x, y * Other.y, z * Other.z); }
        base_vec3 operator/(const base_vec3& Other) const { return base_vec3(x / Other.x, y / Other.y, z / Other.z); }

        base_vec3 operator+(const base_vec2& Other) const { return base_vec3(x + Other.x, y + Other.y, z); }
        base_vec3 operator-(const base_vec2& Other) const { return base_vec3(x - Other.x, y - Other.y, z); }
        base_vec3 operator*(const base_vec2& Other) const { return base_vec3(x * Other.x, y * Other.y, z); }
        base_vec3 operator/(const base_vec2& Other) const { return base_vec3(x / Other.x, y / Other.y, z); }

        base_vec3 operator+(const vec1 a) const { return base_vec3(x + a, y + a, z + a); }
        base_vec3 operator-(const vec1 a) const { return base_vec3(x - a, y - a, z - a); }
        base_vec3 operator*(const vec1 a) const { return base_vec3(x * a, y * a, z * a); }
        base_vec3 operator/(const vec1 a) const { return base_vec3(x / a, y / a, z / a); }

        base_vec3& operator+=(const vec1 a) { x += a; y += a; z += a; return *this; }
        base_vec3& operator-=(const vec1 a) { x -= a; y -= a; z -= a; return *this; }
        base_vec3& operator*=(const vec1 a) { x *= a; y *= a; z *= a; return *this; }
        base_vec3& operator/=(const vec1 a) { x /= a; y /= a; z /= a; return *this; }

        base_vec3& operator+=(const base_vec3& Other) { x += Other.x; y += Other.y; z += Other.z; return *this; }
        base_vec3& operator-=(const base_vec3& Other) { x -= Other.x; y -= Other.y; z -= Other.z; return *this; }
        base_vec3& operator*=(const base_vec3& Other) { x *= Other.x; y *= Other.y; z *= Other.z; return *this; }
        base_vec3& operator/=(const base_vec3& Other) { x /= Other.x; y /= Other.y; z /= Other.z; return *this; }

        base_vec3& operator+=(const base_vec2& Other) { x += Other.x; y += Other.y; return *this; }
        base_vec3& operator-=(const base_vec2& Other) { x -= Other.x; y -= Other.y; return *this; }
        base_vec3& operator*=(const base_vec2& Other) { x *= Other.x; y *= Other.y; return *this; }
        base_vec3& operator/=(const base_vec2& Other) { x /= Other.x; y /= Other.y; return *this; }

        base_vec3 operator-() const { return base_vec3(-x, -y, -z); }

        std::basic_string<char> toString() const {
            return fmt::format("{{ x: {}, y: {} }}", x, y);
        }
    };

    using vec3 = base_vec3<mut_f32>;
}