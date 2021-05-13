#ifndef _LUNAR_MATH_MAT4_H
#define _LUNAR_MATH_MAT4_H

#include <cmath>
#include <cassert>
#include "core/common.h"
#include "math/vec3.h"
#include "math/vec4.h"

namespace Lunar
{
    constexpr fsize PI = 3.14;

    template<typename T>
    struct base_mat4
    {
        static_assert(
            std::is_arithmetic<T>::value,
            "mat4<T> -> T must be a numeric type."
        );

        using Value    = base_mat4<T>;
        using Ref      = base_mat4<T>&;
        using ConstRef = const base_mat4<T>&;

        base_mat4(ConstRef other)
            : m_Elements(other.m_Elements)
        {
        }

        base_mat4(const base_vec4<T>& vec)
            : m_Elements(0)
        {
            m_Elements[0]  = vec.x;
            m_Elements[5]  = vec.y;
            m_Elements[10] = vec.z;
            m_Elements[15] = vec.w;
        }

        base_mat4(T other)
            : m_Elements(0)
        {
            m_Elements[0]  = other;
            m_Elements[5]  = other;
            m_Elements[10] = other;
            m_Elements[15] = other;
        }

        base_mat4()
            : m_Elements(0)
        {
            m_Elements[0]  = 1;
            m_Elements[5]  = 1;
            m_Elements[10] = 1;
            m_Elements[15] = 1;
        }

        T& at(size_t index)
        {
            assert(index > 0 && index < 16);
            return m_Elements[index];
        }

        T& at(size_t x, size_t y)
        {
            assert(x > 0 && x < 4);
            assert(y > 0 && y < 4);
            return m_Elements[x * 4 + y];
        }

        T& begin()
        {
            return m_Elements[0];
        }

        T& end()
        {
            return m_Elements[15];
        }

        Value operator+(ConstRef other) const
        {
            Value result;
            for(size_t i = 0; i < 16; i++) {
                result.at(i) = this->at(i) + other.at(i);
            }
            return result;
        }

        Value operator-(ConstRef other) const
        {
            Value result;
            for(size_t i = 0; i < 16; i++) {
                result.at(i) = this->at(i) - other.at(i);
            }
            return result;
        }

        Value operator*(ConstRef other) const
        {
            Value result;
            for(size_t y = 0; y < 4; y++) {
                for(size_t x = 0; x < 4; x++) {
                    result.at(y, x) = 0;
                    for(size_t k = 0; k < 4; k++)
                        result.at(y, x) += this->at(y, k) * other.at(k, x);
                }
            }
            return result;
        }

        Value operator/(ConstRef other) const
        {
            Value result;
            for(size_t y = 0; y < 4; y++) {
                for(size_t x = 0; x < 4; x++) {
                    result.at(y, x) = 0;
                    for(size_t k = 0; k < 4; k++)
                        result.at(y, x) += this->at(y, k) / other.at(k, x);
                }
            }
            return result;
        }

    private:
        T m_Elements[4 * 4];
    };

    using mat4 = base_mat4<fsize>;

    template<typename T>
    void translate(base_mat4<T>& mat, const base_vec4<T>& pos)
    {
        mat.at(0, 3) = pos.x;
        mat.at(1, 3) = pos.y;
        mat.at(2, 3) = pos.z;
    }

    template<typename T>
    void scale(base_mat4<T>& mat, const base_vec4<T>& scale)
    {
        mat.at(0, 0) = scale.x;
        mat.at(1, 1) = scale.y;
        mat.at(2, 2) = scale.z;
    }

    template<typename T>
    void rotate(base_mat4<T>& mat, const base_vec4<T>& scale)
    {
        auto rotateX = [&]() {
            const float rad = static_cast<float>(scale.x * PI);
            const float cos = cosf(rad);
            const float sin = sinf(rad);

            base_mat4<T> xRotation;
            xRotation.at(1, 1) = cos;
            xRotation.at(1, 2) = sin;
            xRotation.at(2, 1) = -sin;
            xRotation.at(2, 2) = cos;

            mat = mat * xRotation;
        };

        auto rotateY = [&]() {
            const float rad = static_cast<float>(scale.y * PI);
            const float cos = cosf(rad);
            const float sin = sinf(rad);

            base_mat4<T> yRotation;
            yRotation.at(0, 0) = cos;
            yRotation.at(0, 2) = -sin;
            yRotation.at(2, 0) = sin;
            yRotation.at(2, 2) = cos;
            
            mat = mat * yRotation;
        };

        auto rotateZ = [&]() {
            const float rad = static_cast<float>(scale.z * PI);
            const float cos = cosf(rad);
            const float sin = sinf(rad);

            base_mat4<T> zRotation;
            zRotation.at(0, 0) = cos;
            zRotation.at(0, 1) = sin;
            zRotation.at(1, 0) = -sin;
            zRotation.at(1, 1) = cos;

            mat = mat * zRotation;
        };

        rotateX();
        rotateY();
        rotateZ();
    }

    template<typename T>
    void transpose(base_mat4<T>& mat)
    {
        base_mat4<T> copy = mat;
        for(size_t y = 0; y < 4; y++) {
            for(size_t x = 0; x < 4; x++) {
                mat.at(x, y) = copy.at(y, x);
            }
        }
    }
}

#endif // _LUNAR_MATH_MAT4_H
