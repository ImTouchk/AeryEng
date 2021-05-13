#ifndef _LUNAR_MATH_VEC3_H
#define _LUNAR_MATH_VEC3_H

#include <cmath>
#include <type_traits>
#include <immintrin.h>
#include "core/common.h"

namespace Lunar
{
    template<typename T>
    struct base_vec3
    {
        static_assert(
            std::is_arithmetic<T>::value,
            "vec<T> -> T must be a numeric type."
        );

        using Value    = base_vec3<T>;
        using Ref      = base_vec3<T>&;
        using ConstRef = const base_vec3<T>&;

        base_vec3(ConstRef other)
            : x(other.x),
              y(other.y),
              z(other.z)
        {
        }

        base_vec3(T x, T y, T z)
            : x(x),
              y(y),
              z(z)
        {
        }

        base_vec3()
            : x(0),
              y(0),
              z(0)
        {
        }

        T length() const
        {
            /* sqrt( (x * x) + (y * y) + (z * z) ) */
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
            return Value(-x, -y, -z);
        }

        Value operator+(T a) const
        {
            return Value(x + a, y + a, z + a);
        }

        Value operator-(T a) const
        {
            return Value(x - a, y - a, z - a);
        }

        Value operator*(T a) const
        {
            return Value(x * a, y * a, z * a);
        }

        Value operator/(T a) const
        {
            return Value(x / a, y / a, z /a);
        }

        Value operator+(ConstRef other) const
        {
            return Value(x + other.x, y + other.y, z + other.z);
        }

        Value operator-(ConstRef other) const
        {
            return Value(x - other.x, y - other.y, z - other.z);
        }

        Value operator*(ConstRef other) const
        {
            return Value(x * other.x, y * other.y, z * other.z);
        }

        Value operator/(ConstRef other) const
        {
            return Value(x / other.x, y / other.y, z / other.z);
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

    private:
        T _unused; 
        /* alignment for avx*/
    };

    using vec3 = base_vec3<fsize>;
}

#endif // _LUNAR_MATH_VEC3_H
