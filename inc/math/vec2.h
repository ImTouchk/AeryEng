#ifndef _LUNAR_MATH_VEC2_H
#define _LUNAR_MATH_VEC2_H

#include <cmath>
#include <type_traits>
#include "core/common.h"

namespace Lunar
{
    template<typename T>
    struct base_vec2
    {
        static_assert(
            std::is_arithmetic<T>::value, 
            "vec<T> -> T must be a numeric type."
        );

        using Value    = base_vec2<T>;
        using Ref      = base_vec2<T>&;
        using ConstRef = const base_vec2<T>&;

        base_vec2(ConstRef other)
            : x(other.x),
              y(other.y)
        {
        }

        base_vec2(T x, T y)
            : x(x),
              y(y)
        {
        }

        base_vec2()
            : x(0),
              y(0)
        {
        }

        T length() const
        {
            return std::sqrt((x * x) + (y * y));
        }

        T dot(ConstRef other)
        {
            return (x * other.x) + (y * other.y);
        }

        T angle(ConstRef other)
        {
            return dot(other) / (length() * other.length());
        }

        Value normalize() const
        {
            const T len = length();
            return *this / len;
        }

        Value operator-() const
        {
            return Value(-x, -y);
        }

        Value operator+(T a) const
        {
            return Value(x + a, y + a);
        }

        Value operator-(T a) const
        {
            return Value(x - a, y - a);
        }

        Value operator*(T a) const
        {
            return Value(x * a, y * a);
        }

        Value operator/(T a) const
        {
            return Value(x / a, y / a);
        }

        Value operator+(ConstRef other) const
        {
            return Value(x + other.x, y + other.y);
        }

        Value operator-(ConstRef other) const
        {
            return Value(x - other.x, y - other.y);
        }

        Value operator*(ConstRef other) const
        {
            return Value(x * other.x, y * other.y);
        }

        Value operator/(ConstRef other) const
        {
            return Value(x / other.x, y / other.y);
        }

        Ref operator+=(T a)
        {
            x += a;
            y += a;
            return *this;
        }

        Ref operator-=(T a)
        {
            x -= a;
            y -= a;
            return *this;
        }

        Ref operator*=(T a)
        {
            x *= a;
            y *= a;
            return *this;
        }

        Ref operator/=(T a)
        {
            x /= a;
            y /= a;
            return *this;
        }

        Ref operator+=(ConstRef other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Ref operator-=(ConstRef other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Ref operator*=(ConstRef other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        }

        Ref operator/=(ConstRef other)
        {
            x /= other.x;
            y /= other.y;
            return *this;
        }
    public:
        union { T x; T r; };
        union { T y; T g; };
    };

    using vec2 = base_vec2<fsize>;
}

#endif // _LUNAR_MATH_VEC2_H
