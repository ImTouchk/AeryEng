#pragma once

#include "utils/types.hpp"
#include "math/vec4.hpp"
#include "math/vec3.hpp"
#include <cmath>

namespace Aery {
    constexpr mut_f32 pi = 3.14159265359f;

    template<typename vec1>
    struct base_mat4x4 {
        static_assert(std::is_arithmetic<vec1>(), "vec1 must be a number type");
        vec1 _Elements[16];
#   define iter(s, a) for(mut_u16 i = 0; i < s; i++) { a; }
    public:
        base_mat4x4(const base_vec4<vec1>& a) {
            iter(16, _Elements[i] = 0.0f);
            _Elements[0] = a.x;
            _Elements[5] = a.y;
            _Elements[10] = a.z;
            _Elements[15] = a.w;
        }

        base_mat4x4(const base_vec3<vec1>& a) {
            iter(16, _Elements[i] = 0.0f);
            _Elements[0] = a.x;
            _Elements[5] = a.y;
            _Elements[10] = a.z;
            _Elements[15] = 1.0f;
        }

        base_mat4x4(const base_vec2<vec1>& a) {
            iter(16, _Elements[i] = 0.0f);
            _Elements[0] = a.x;
            _Elements[5] = a.y;
            _Elements[10] = 1.0f;
            _Elements[15] = 1.0f;
        }

        base_mat4x4(const vec1 a) {
            iter(16, _Elements[i] = 0.0f);
            _Elements[0] = a;
            _Elements[5] = a;
            _Elements[10] = a;
            _Elements[15] = a;
        }

        base_mat4x4() {
            iter(16, _Elements[i] = 0.0f);
            _Elements[0] = 1.0f;
            _Elements[5] = 1.0f;
            _Elements[10] = 1.0f;
            _Elements[15] = 1.0f;
        }

        void translate(const base_vec2<vec1>& a) {
            base_mat4x4 translate = {};
            translate(12) = a.x;
            translate(13) = a.y;
            translate(14) = 1.0f;
            *this = *this * translate;
        }

        void translate(const base_vec3<vec1>& a) {
            base_mat4x4 translate = {};
            translate(12) = a.x;
            translate(13) = a.y;
            translate(14) = a.z;
            *this = *this * translate;
        }

        void translate(const base_vec4<vec1>& a) {
            base_mat4x4 translate = {};
            translate(12) = a.x;
            translate(13) = a.y;
            translate(14) = a.z;
            *this = *this * translate;
        }

        void scale(const base_vec2<vec1>& a) {
            base_mat4x4 scale = {};
            scale(0) = a.x;
            scale(5) = a.y;
            scale(10) = 1.0f;
            *this = *this * scale;
        }

        void scale(const base_vec3<vec1>& a) {
            base_mat4x4 scale = {};
            scale(0) = a.x;
            scale(5) = a.y;
            scale(10) = a.z;
            *this = *this * scale;
        }

        void scale(const base_vec4<vec1>& a) {
            base_mat4x4 scale = {};
            scale(0) = a.x;
            scale(5) = a.y;
            scale(10) = a.z;
            *this = *this * scale;
        }

        // Rotate on the x-axis by 'a' degrees
        void rotate_x(const vec1 a) {
            const vec1 rad = a * pi / 180.0f;
            const vec1 cosine = static_cast<vec1>( std::cos(rad) );
            const vec1 sine = static_cast<vec1>( std::sin(rad) );
            base_mat4x4 rotation = {};
            rotation(5)  =  cosine;
            rotation(6)  =  sine;
            rotation(9)  = -sine;
            rotation(10) =  cosine;
            *this = *this * rotation;
        }

        // Rotate on the y-axis by 'a' degrees
        void rotate_y(const vec1 a) {
            const vec1 rad = a * pi / 180.0f;
            const vec1 cosine = static_cast<vec1>( std::cos(rad) );
            const vec1 sine = static_cast<vec1>( std::sin(rad) );
            base_mat4x4 rotation = {};
            rotation(0)  =  cosine;
            rotation(2)  = -sine;
            rotation(8)  =  sine;
            rotation(10) =  cosine;
            *this = *this * rotation;
        }

        // Rotate on the z-axis by 'a' degrees
        void rotate_z(const vec1 a) {
            const vec1 rad = a * pi / 180.0f;
            const vec1 cosine = static_cast<vec1>( std::cos(rad) );
            const vec1 sine = static_cast<vec1>( std::sin(rad) );
            base_mat4x4 rotation = {};
            rotation(0) = cosine;
            rotation(1) = sine;
            rotation(4) = -sine;
            rotation(5) = cosine;
            *this = *this * rotation;
        }

        void rotate(const base_vec2<vec1>& a) {
            rotate_x(a.x);
            rotate_y(a.y);
        }

        void rotate(const base_vec3<vec1>& a) {
            rotate_x(a.x);
            rotate_y(a.y);
            rotate_z(a.z);
        }

        void rotate(const base_vec4<vec1>& a) {
            rotate_x(a.x);
            rotate_y(a.y);
            rotate_z(a.z);
        }

        void lookAt(const base_vec4<vec1>& eye, const base_vec4<vec1>& center, const base_vec4<vec1>& up) {
            const base_vec4 zAxis = (center - eye).normalize();
            const base_vec4 xAxis = up.cross(zAxis).normalize();
            const base_vec4 yAxis = zAxis.cross(xAxis);
            base_mat4x4 lookAt = {};
            lookAt(0) = xAxis.x;
            lookAt(1) = xAxis.y;
            lookAt(2) = xAxis.z;
            lookAt(3) = xAxis.dot(-eye);

            lookAt(4) = yAxis.x;
            lookAt(5) = yAxis.y;
            lookAt(6) = yAxis.z;
            lookAt(7) = yAxis.dot(-eye);

            lookAt(8) = zAxis.x;
            lookAt(9) = zAxis.y;
            lookAt(10) = zAxis.z;
            lookAt(11) = zAxis.dot(-eye);
            *this = *this * lookAt;
        }

        void transpose() {
            for (mut_u16 y = 0; y < 4; y++) {
                for (mut_u16 x = 0; x < 4; x++) {
                    (*this)(x, y) = (*this)(y, x);
                }
            }
        }

        base_mat4x4 operator+(const base_mat4x4& other) const {
            base_mat4x4 result = {};
            iter(16, result._Elements[i] = _Elements[i] + other._Elements[i]);
            return result;
        }

        base_mat4x4 operator-(const base_mat4x4& other) const {
            base_mat4x4 result = {};
            iter(16, result._Elements[i] = _Elements[i] - other._Elements[i]);
            return result;
        }

        base_mat4x4 operator*(const base_mat4x4& other) const {
            base_mat4x4 result = {};
            for (mut_u16 y = 0; y < 4; y++) {
                for (mut_u16 x = 0; x < 4; x++) {
                    result(y, x) = 0.0f;
                    for (int k = 0; k < 4; k++) {
                        result(y, x) += (*this)(y, k) * other(k, x);
                    }
                }
            }
            return result;
        }

        base_mat4x4& operator+=(const base_mat4x4& other) {
            iter(16, _Elements[i] += other._Elements[i]);
            return *this;
        }

        base_mat4x4& operator-=(const base_mat4x4& other) {
            iter(16, _Elements[i] -= other._Elements[i]);
            return *this;
        }

        const vec1& operator()(int x, int y) const {
            return _Elements[x * 4 + y];
        }

        const vec1& operator()(int a) const {
            return _Elements[a];
        }

        vec1& operator()(int x, int y) {
            return _Elements[x * 4 + y];
        }

        vec1& operator()(int a) {
            return _Elements[a];
        }

        std::basic_string<char> toString() const {
            return 
                fmt::format("mat4x4 {{\n    {} {} {} {}\n    {} {} {} {}\n    {} {} {} {}\n    {} {} {} {}\n}}\n",
                    _Elements[0],  _Elements[1],  _Elements[2],  _Elements[3], 
                    _Elements[4],  _Elements[5],  _Elements[6],  _Elements[7], 
                    _Elements[8],  _Elements[9],  _Elements[10], _Elements[11], 
                    _Elements[12], _Elements[13], _Elements[14], _Elements[15]
                );
        }

#undef  iter
    };

    using mat4 = base_mat4x4<mut_f32>;
}