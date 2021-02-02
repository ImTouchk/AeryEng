#include "math/vec2.hpp"
#include <fmt/core.h>

namespace Aery {
    template<typename vec1>
    base_vec2<vec1>::base_vec2(const base_vec2& Other) {
        x = Other.x;
        y = Other.y;
    }

    template<typename vec1>
    base_vec2<vec1>::base_vec2(const vec1 x, const vec1 y) {
        this->x = x;
        this->y = y;
    }

    template<typename vec1>
    base_vec2<vec1>::base_vec2(const vec1 a) {
        x = a;
        y = a;
    }

    template<typename vec1>
    base_vec2<vec1>::base_vec2() {
        x = 0;
        y = 0;
    }

    template<typename vec1>
    base_vec2<vec1> base_vec2<vec1>::normalize() const {
        const vec1 Length = length();
        return *this / Length;
    }

    template<typename vec1>
    inline vec1 base_vec2<vec1>::length() const {
        return sqrt((x * x) + (y * y));
    }

    template<typename vec1>
    inline vec1 base_vec2<vec1>::magnitude() const {
        return sqrt((x * x) + (y * y));
    }

    template<typename vec1>
    inline vec1 base_vec2<vec1>::dot(const base_vec2& Other) const {
        return (x * Other.x) + (y * Other.y);
    }

    template<typename vec1>
    inline vec1 base_vec2<vec1>::angle(const base_vec2& Other) const {
        return dot(Other) / (length() * Other.length());
    }

    template<typename vec1>
    base_vec2<vec1> base_vec2<vec1>::operator+(const base_vec2& Other) const {
        return base_vec2<vec1>(x + Other.x, y + Other.y);
    }

    template<typename vec1>
    base_vec2<vec1> base_vec2<vec1>::operator-(const base_vec2& Other) const {
        return base_vec2<vec1>(x - Other.x, y - Other.y);
    }

    template<typename vec1>
    base_vec2<vec1> base_vec2<vec1>::operator*(const base_vec2& Other) const {
        return base_vec2<vec1>(x * Other.x, y * Other.y);
    }

    template<typename vec1>
    base_vec2<vec1> base_vec2<vec1>::operator/(const base_vec2& Other) const {
        return base_vec2<vec1>(x / Other.x, y / Other.y);
    }

    template<typename vec1>
    base_vec2<vec1> base_vec2<vec1>::operator+(const vec1 a) const {
        return base_vec2<vec1>(x + a, y + a);
    }

    template<typename vec1>
    base_vec2<vec1> base_vec2<vec1>::operator-(const vec1 a) const {
        return base_vec2<vec1>(x - a, y - a);
    }

    template<typename vec1>
    base_vec2<vec1> base_vec2<vec1>::operator*(const vec1 a) const {
        return base_vec2<vec1>(x * a, y * a);
    }

    template<typename vec1>
    base_vec2<vec1> base_vec2<vec1>::operator/(const vec1 a) const {
        return base_vec2<vec1>(x / a, y / a);
    }

    template<typename vec1>
    base_vec2<vec1>& base_vec2<vec1>::operator+=(const vec1 a) {
        x += a;
        y += a;
        return *this;
    }

    template<typename vec1>
    base_vec2<vec1>& base_vec2<vec1>::operator-=(const vec1 a) {
        x -= a;
        y -= a;
        return *this;
    }

    template<typename vec1>
    base_vec2<vec1>& base_vec2<vec1>::operator*=(const vec1 a) {
        x *= a;
        y *= a;
        return *this;
    }

    template<typename vec1>
    base_vec2<vec1>& base_vec2<vec1>::operator/=(const vec1 a) {
        x /= a;
        y /= a;
        return *this;
    }

    template<typename vec1>
    base_vec2<vec1>& base_vec2<vec1>::operator+=(const base_vec2& Other) {
        x += Other.x;
        y += Other.y;
        return *this;
    }

    template<typename vec1>
    base_vec2<vec1>& base_vec2<vec1>::operator-=(const base_vec2& Other) {
        x -= Other.x;
        y -= Other.y;
        return *this;
    }

    template<typename vec1>
    base_vec2<vec1>& base_vec2<vec1>::operator*=(const base_vec2& Other) {
        x *= Other.x;
        y *= Other.y;
        return *this;
    }

    template<typename vec1>
    base_vec2<vec1>& base_vec2<vec1>::operator/=(const base_vec2& Other) {
        x /= Other.x;
        y /= Other.y;
        return *this;
    }

    template<typename vec1>
    base_vec2<vec1> Aery::base_vec2<vec1>::operator-() const {
        return base_vec2<vec1>(-x, -y);
    }

    template<typename vec1>
    std::basic_string<char> base_vec2<vec1>::toString() const {
        return fmt::format("\{ x: {}, y: {} \}", x, y);
    }
}