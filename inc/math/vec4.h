#ifndef _LUNAR_MATH_VEC4_H
#define _LUNAR_MATH_VEC4_H

#include <cmath>
#include <type_traits>
#include <immintrin.h>
#include "core/common.h"

namespace Lunar
{
    template<typename T>
    struct base_vec4
    {
        static_assert(
            std::is_arithmetic<T>::value,
            "vec<T> -> T must be a numeric type."
        );

        using Value    = base_vec4<T>;
        using Ref      = base_vec4<T>&;
        using ConstRef = const base_vec4<T>&;

        base_vec4(ConstRef other)
            : x(other.x),
              y(other.y),
              z(other.z),
              w(other.w)
        {
        }

        base_vec4(T x, T y, T z, T w)
            : x(x),
              y(y),
              z(z),
              w(w)
        {
        }

        base_vec4(T x, T y, T z)
            : x(x),
              y(y),
              z(z),
              w(0)
        {
        }

        base_vec4()
            : x(0),
              y(0),
              z(0),
              w(0)
        {
        }

        T length() const
        {
            return (w == 1) ?
                1 :
                std::sqrt(
                    (x * x) +
                    (y * y) +
                    (z * z)
                );
        }

        T dot(ConstRef other)
        {
            return (x * other.x) +
                   (y * other.y) +
                   (z * other.z);
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
            return Value(-x, -y, -z, w);
        }

        Value operator+(T a) const
        {
            return Value(x + a, y + a, z + a, w);
        }

        Value operator-(T a) const
        {
            return Value(x - a, y - a, z - a, w);
        }

        Value operator*(T a) const
        {
            return Value(x * a, y * a, z * a, w);
        }

        Value operator/(T a) const
        {
            return Value(x / a, y / a, z / a, w);
        }

        Value operator+(ConstRef other) const
        {
            return Value(x + other.x, y + other.y, z + other.z, w);
        }

        Value operator-(ConstRef other) const
        {
            return Value(x - other.x, y - other.y, z - other.z, w);
        }

        Value operator*(ConstRef other) const
        {
            return Value(x * other.x, y * other.y, z * other.z, w);
        }

        Value operator/(ConstRef other) const
        {
            return Value(x / other.x, y / other.y, z / other.z, w);
        }

        Ref operator+=(T a)
        {
            x += a;
            y += a;
            z += a;
            return *this;
        }

        Ref operator-=(T a)
        {
            x -= a;
            y -= a;
            z -= a;
            return *this;
        }

        Ref operator*=(T a)
        {
            x *= a;
            y *= a;
            z *= a;
            return *this;
        }

        Ref operator/=(T a)
        {
            x /= a;
            y /= a;
            z /= a;
            return *this;
        }

        Ref operator+=(ConstRef other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Ref operator-=(ConstRef other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Ref operator*=(ConstRef other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        Ref operator/=(ConstRef other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            return *this;
        }
    public:
        union { T x; T r; };
        union { T y; T g; };
        union { T z; T b; };
        union { T w; T a; };
    };

    using vec4 = base_vec4<fsize>;
}

#endif // _LUNAR_MATH_VEC4_H
