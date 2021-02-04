#pragma once

#include "utils/types.hpp"
#include <fmt/core.h>
#include <string>
#include <cmath>

namespace Aery {
    template<typename vec1>
    struct base_vec2 {
    static_assert(std::is_arithmetic<vec1>(), "vec1 must be a number type.");
    public:
        vec1 x, y;

        base_vec2(const base_vec2& Other) { x = Other.x; y = Other.y; }
        base_vec2(const vec1 x, const vec1 y) { this->x = x; this->y = y; }
        base_vec2(const vec1 a) { x = a; y = a; }
        base_vec2() { x = 0; y = 0; }

        base_vec2 normalize() const { return *this / length(); }
        
        inline vec1 length() const { return sqrt((x * x) + (y * y)); }
        inline vec1 magnitude() const { return sqrt((x * x) + (y * y)); }
        inline vec1 dot(const base_vec2& Other) const { return (x * Other.x) + (y * Other.y); }
        inline vec1 angle(const base_vec2& Other) const { return dot(Other) / (length() * Other.length()); }

        base_vec2 operator+(const base_vec2& Other) const { return base_vec2(x + Other.x, y + Other.y); }
        base_vec2 operator-(const base_vec2& Other) const { return base_vec2(x - Other.x, y - Other.y); }
        base_vec2 operator*(const base_vec2& Other) const { return base_vec2(x * Other.x, y * Other.y); }
        base_vec2 operator/(const base_vec2& Other) const { return base_vec2(x / Other.x, y / Other.y); }

        base_vec2 operator+(const vec1 a) const { return base_vec2(x + a, y + a); }
        base_vec2 operator-(const vec1 a) const { return base_vec2(x - a, y - a); }
        base_vec2 operator*(const vec1 a) const { return base_vec2(x * a, y * a); }
        base_vec2 operator/(const vec1 a) const { return base_vec2(x / a, y / a); }

        base_vec2& operator+=(const vec1 a) { x += a; y += a; return *this; }
        base_vec2& operator-=(const vec1 a) { x -= a; y -= a; return *this; }
        base_vec2& operator*=(const vec1 a) { x *= a; y *= a; return *this; }
        base_vec2& operator/=(const vec1 a) { x /= a; y /= a; return *this; }

        base_vec2& operator+=(const base_vec2& Other) { x += Other.x; y += Other.y; return *this; }
        base_vec2& operator-=(const base_vec2& Other) { x -= Other.x; y -= Other.y; return *this; }
        base_vec2& operator*=(const base_vec2& Other) { x *= Other.x; y *= Other.y; return *this; }
        base_vec2& operator/=(const base_vec2& Other) { x /= Other.x; y /= Other.y; return *this; }

        base_vec2 operator-() const { return base_vec2(-x, -y); }

        std::basic_string<char> toString() const {
            return fmt::format("{{ x: {}, y: {} }}", x, y);
        }
    };

    using vec2 = base_vec2<mut_f32>;
}