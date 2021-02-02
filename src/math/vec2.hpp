#pragma once

#include "utils/types.hpp"
#include <string>
#include <cmath>

namespace Aery {
    template<typename vec1>
    class base_vec2 {
    static_assert(std::is_arithmetic<vec1>(), "vec1 must be a number type.");
    public:
        vec1 x, y;

        base_vec2(const base_vec2&);
        base_vec2(const vec1, const vec1);
        base_vec2(const vec1);
        base_vec2();

        base_vec2 normalize() const;
        
        inline vec1 length() const;
        inline vec1 magnitude() const;
        inline vec1 dot(const base_vec2&) const;
        inline vec1 angle(const base_vec2&) const;

        base_vec2 operator+(const base_vec2&) const;
        base_vec2 operator-(const base_vec2&) const;
        base_vec2 operator*(const base_vec2&) const;
        base_vec2 operator/(const base_vec2&) const;

        base_vec2 operator+(const vec1) const;
        base_vec2 operator-(const vec1) const;
        base_vec2 operator*(const vec1) const;
        base_vec2 operator/(const vec1) const;

        base_vec2& operator+=(const vec1);
        base_vec2& operator-=(const vec1);
        base_vec2& operator*=(const vec1);
        base_vec2& operator/=(const vec1);

        base_vec2& operator+=(const base_vec2&);
        base_vec2& operator-=(const base_vec2&);
        base_vec2& operator*=(const base_vec2&);
        base_vec2& operator/=(const base_vec2&);

        base_vec2 operator-() const;

        std::basic_string<char> toString() const;
    };

    using vec2 = base_vec2<f32>;
}