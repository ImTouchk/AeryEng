#ifndef _LUNAR_MATH_VEC4_H
#define _LUNAR_MATH_VEC4_H

#include <cmath>
#include <type_traits>
#include <immintrin.h>
#include "core/common.h"

namespace Lunar
{
    template<typename T, int _ = 4>
    struct vec
    {
        static_assert(
            std::is_arithmetic<T>::value,
            "vec<T> -> T must be a numeric type."
        );

        using Value    = vec<T, 4>;
        using Ref      = vec<T, 4>&;
        using ConstRef = const vec<T, 4>&;

        vec(ConstRef other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
        }

        vec(T x, T y, T z, T w)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        vec(T x, T y, T z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            w = 0;
        }

        vec()
        {
            x = 0;
            y = 0;
            z = 0;
            w = 0;
        }

        T length() const
        {
            (w == 1) ?
                return 1 :
                return std::sqrt(
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

    using vec4 = vec<fsize, 4>;
}

#endif // _LUNAR_MATH_VEC4_H
