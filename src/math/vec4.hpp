#pragma once

#include "utils/types.hpp"
#include "math/vec2.hpp"
#include "math/vec3.hpp"
#include <fmt/core.h>
#include <string>
#include <cmath>

namespace Aery {
    template<typename vec1>
    struct base_vec4 {
        static_assert(std::is_arithmetic<vec1>(), "vec1 must be a number type.");
    public:
        vec1 x, y, z, a;

        base_vec4(const base_vec4& Other) { x = Other.x; y = Other.y; z = Other.z; a = Other.a; }
        base_vec4(const base_vec3<vec1>& Other) { x = Other.x; y = Other.y; z = Other.z; a = 0; }
        base_vec4(const base_vec2<vec1>& Other) { x = Other.x; y = Other.y; z = 0; a = 0; }
        base_vec4(const vec1 x, const vec1 y, const vec1 z, const vec1 a) { this->x = x; this->y = y; this->z = z; this->a = a; }
        base_vec4(const vec1 x, const vec1 y, const vec1 z) { this->x = x; this->y = y; this->z = z; a = 0; }
        base_vec4(const vec1 x, const vec1 y) { this->x = x; this->y = y; z = 0; a = 0; }
        base_vec4(const vec1 a) { x = a; y = a; z = 0; a = 0; }
        base_vec4() { x = 0; y = 0; z = 0; a = 0; }

        base_vec4 normalize() const { return *this / length(); }

        inline vec1 length() const { return sqrt((x * x) + (y * y) + (z * z)); }
        inline vec1 magnitude() const { return sqrt((x * x) + (y * y) + (z * z)); }
        inline vec1 dot(const base_vec4& Other) const { return (x * Other.x) + (y * Other.y) + (z * Other.z); }
        inline vec1 angle(const base_vec4& Other) const { return dot(Other) / (length() * Other.length()); }

        base_vec4 operator+(const base_vec4& Other) const { return base_vec4(x + Other.x, y + Other.y, z + Other.z); }
        base_vec4 operator-(const base_vec4& Other) const { return base_vec4(x - Other.x, y - Other.y, z - Other.z); }
        base_vec4 operator*(const base_vec4& Other) const { return base_vec4(x * Other.x, y * Other.y, z * Other.z); }
        base_vec4 operator/(const base_vec4& Other) const { return base_vec4(x / Other.x, y / Other.y, z / Other.z); }

        base_vec4 operator+(const base_vec3<vec1>& Other) const { return base_vec4(x + Other.x, y + Other.y, z + Other.z); }
        base_vec4 operator-(const base_vec3<vec1>& Other) const { return base_vec4(x - Other.x, y - Other.y, z - Other.z); }
        base_vec4 operator*(const base_vec3<vec1>& Other) const { return base_vec4(x * Other.x, y * Other.y, z * Other.z); }
        base_vec4 operator/(const base_vec3<vec1>& Other) const { return base_vec4(x / Other.x, y / Other.y, z / Other.z); }

        base_vec4 operator+(const base_vec2<vec1>& Other) const { return base_vec4(x + Other.x, y + Other.y, z); }
        base_vec4 operator-(const base_vec2<vec1>& Other) const { return base_vec4(x - Other.x, y - Other.y, z); }
        base_vec4 operator*(const base_vec2<vec1>& Other) const { return base_vec4(x * Other.x, y * Other.y, z); }
        base_vec4 operator/(const base_vec2<vec1>& Other) const { return base_vec4(x / Other.x, y / Other.y, z); }

        base_vec4 operator+(const vec1 a) const { return base_vec4(x + a, y + a, z + a); }
        base_vec4 operator-(const vec1 a) const { return base_vec4(x - a, y - a, z - a); }
        base_vec4 operator*(const vec1 a) const { return base_vec4(x * a, y * a, z * a); }
        base_vec4 operator/(const vec1 a) const { return base_vec4(x / a, y / a, z / a); }

        base_vec4& operator+=(const vec1 a) { x += a; y += a; z += a; return *this; }
        base_vec4& operator-=(const vec1 a) { x -= a; y -= a; z -= a; return *this; }
        base_vec4& operator*=(const vec1 a) { x *= a; y *= a; z *= a; return *this; }
        base_vec4& operator/=(const vec1 a) { x /= a; y /= a; z /= a; return *this; }

        base_vec4& operator+=(const base_vec4& Other) { x += Other.x; y += Other.y; z += Other.z; return *this; }
        base_vec4& operator-=(const base_vec4& Other) { x -= Other.x; y -= Other.y; z -= Other.z; return *this; }
        base_vec4& operator*=(const base_vec4& Other) { x *= Other.x; y *= Other.y; z *= Other.z; return *this; }
        base_vec4& operator/=(const base_vec4& Other) { x /= Other.x; y /= Other.y; z /= Other.z; return *this; }

        base_vec4& operator+=(const base_vec3<vec1>& Other) { x += Other.x; y += Other.y; z += Other.z; return *this; }
        base_vec4& operator-=(const base_vec3<vec1>& Other) { x -= Other.x; y -= Other.y; z -= Other.z; return *this; }
        base_vec4& operator*=(const base_vec3<vec1>& Other) { x *= Other.x; y *= Other.y; z *= Other.z; return *this; }
        base_vec4& operator/=(const base_vec3<vec1>& Other) { x /= Other.x; y /= Other.y; z /= Other.z; return *this; }

        base_vec4& operator+=(const base_vec2<vec1>& Other) { x += Other.x; y += Other.y; return *this; }
        base_vec4& operator-=(const base_vec2<vec1>& Other) { x -= Other.x; y -= Other.y; return *this; }
        base_vec4& operator*=(const base_vec2<vec1>& Other) { x *= Other.x; y *= Other.y; return *this; }
        base_vec4& operator/=(const base_vec2<vec1>& Other) { x /= Other.x; y /= Other.y; return *this; }

        base_vec4 operator-() const { return base_vec4(-x, -y, -z); }

        std::basic_string<char> toString() const {
            return fmt::format("{{ x: {}, y: {} }}", x, y);
        }
    };

    using vec4 = base_vec4<mut_f32>;
}