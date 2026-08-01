#pragma once
#include <array>
#include <cmath>
#include <xmmintrin.h>
#include <immintrin.h>

namespace hlm {
    struct Vec3 {
        union 
        {
            struct { float x, y, z; };
            float data[3];
        }; 
        Vec3(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) { }
    };
    struct alignas(16) Vec4 {
        union {
            struct { float x, y, z, w; };
            float data[4];
        };
        Vec4(float _x = 0, float _y = 0, float _z = 0, float _w = 0) : x(_x), y(_y), z(_z), w(_w) { }
        
    };
    struct Mat3 {
        std::array<float, 9> mat3{};
    };
    struct alignas(64) Mat4 {
        std::array<float, 16> mat4{};
    };
    inline Vec4 operator*(const Vec4& v1, const Vec4& v2) {
        Vec4 result;
        _mm_store_ps(result.data, _mm_mul_ps(_mm_load_ps(v1.data), _mm_load_ps(v2.data)));
        return result; 
    }
    inline float dot(const Vec4& v1, const Vec4& v2) {
        Vec4 v = v1 * v2;
        return v.x + v.y + v.z + v.w;
        
    }

    inline Vec4 operator*(const Mat4& m, const Vec4& v) {
        return Vec4(
            m.mat4[0]*v.x + m.mat4[4]*v.y + m.mat4[8]*v.z + m.mat4[12]*v.w,
            m.mat4[1]*v.x + m.mat4[5]*v.y + m.mat4[9]*v.z + m.mat4[13]*v.w,
            m.mat4[2]*v.x + m.mat4[6]*v.y + m.mat4[10]*v.z + m.mat4[14]*v.w,
            m.mat4[3]*v.x + m.mat4[7]*v.y + m.mat4[11]*v.z + m.mat4[15]*v.w
        );
    }
    inline Mat4 transpose(Mat4 b) {
        __m128 row0 = _mm_load_ps(&b.mat4[0]);
        __m128 row1 = _mm_load_ps(&b.mat4[4]);
        __m128 row2 = _mm_load_ps(&b.mat4[8]);
        __m128 row3 = _mm_load_ps(&b.mat4[12]);

        _MM_TRANSPOSE4_PS(row0, row1, row2, row3);
        Mat4 result;
        _mm_store_ps(&result.mat4[0], row0);
        _mm_store_ps(&result.mat4[4], row1);
        _mm_store_ps(&result.mat4[8], row2);
        _mm_store_ps(&result.mat4[12], row3);
        return result; 
    }
    
    /*inline Mat4 operator*(const Mat4& a, const Mat4& b) {
        __m128 col0_b = _mm_load_ps(&b.mat4[0]);
        __m128 col1_b = _mm_load_ps(&b.mat4[4]);
        __m128 col2_b = _mm_load_ps(&b.mat4[8]);
        __m128 col3_b = _mm_load_ps(&b.mat4[12]);
        
        __m128 col0_a = _mm_load_ps(&a.mat4[0]);
        __m128 col1_a = _mm_load_ps(&a.mat4[4]);
        __m128 col2_a = _mm_load_ps(&a.mat4[8]);
        __m128 col3_a = _mm_load_ps(&a.mat4[12]);

        __m128 row1 = 
        _mm_fmadd_ps(col0_a, col0_b, 
        _mm_fmadd_ps(col0_a, col1_b, 
        _mm_fmadd_ps(col0_a, col2_b, 
        _mm_mul_ps(col0_a, col3_b))));
         
        __m128 row2 = 
        _mm_fmadd_ps(col1_a, col0_b, 
        _mm_fmadd_ps(col1_a, col1_b, 
        _mm_fmadd_ps(col1_a, col2_b, 
        _mm_mul_ps(col1_a, col3_b))));

        __m128 row3 = 
        _mm_fmadd_ps(col2_a, col0_b, 
        _mm_fmadd_ps(col2_a, col1_b, 
        _mm_fmadd_ps(col2_a, col2_b, 
        _mm_mul_ps(col2_a, col3_b))));

        __m128 row4 = 
        _mm_fmadd_ps(col3_a, col0_b, 
        _mm_fmadd_ps(col3_a, col1_b, 
        _mm_fmadd_ps(col3_a, col2_b, 
        _mm_mul_ps(col3_a, col3_b))));

        Mat4 result;
        _mm_store_ps(&result.mat4[0], row1);
        _mm_store_ps(&result.mat4[4], row2);
        _mm_store_ps(&result.mat4[8], row3);
        _mm_store_ps(&result.mat4[12], row4);

        result.mat4[4 * i + j] = 
                a.mat4[0 + i * 4] * b.mat4[0 + j * 4] + 
                a.mat4[1 + i * 4] * b.mat4[1 + j * 4] +
                a.mat4[2 + i * 4] * b.mat4[2 + j * 4] +
                a.mat4[3 + i * 4] * b.mat4[3 + j * 4];
        return result;
    }*/
    


    namespace mvp {
        /*inline hlm::Mat4 CreateView(float x, float y, float z) {
            return hlm::Mat4{{
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            }};
        }*/
        inline hlm::Mat4 CreateProjection(float aspect, float fov, float near_pos, float far_pos) {
            float tanHalfFov = std::tan(fov / 2.0f);
            return hlm::Mat4{{
                static_cast<float>(1.0f / (aspect * tanHalfFov)),  0.0f, 0.0f, 0.0f,
                0.0f, static_cast<float>(1.0f / tanHalfFov), 0.0f, 0.0f,
                0.0f, 0.0f, -(far_pos + near_pos) / (far_pos - near_pos), -1.0f,
                0.0f, 0.0f, -(2 * far_pos * near_pos) / (far_pos - near_pos), 0.0f
            }};
        }
        namespace model {
            inline hlm::Mat4 CreateTransiton(float tx = 0, float ty = 0, float tz = -1) {
                return hlm::Mat4{{
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    tx, ty, tz, 1.0f,
                }};
            }
            inline hlm::Mat4 CreateScale(float sx = 1.0f, float sy = 1.0f, float sz = 1.0f) {
                return hlm::Mat4{{
                    sx, 0.0f, 0.0f, 0.0f,
                    0.0f, sy, 0.0f, 0.0f,
                    0.0f, 0.0f, sz, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f,
                }};
            }
            inline hlm::Mat4 CreateRotation(float A = 0.0f) {
                return hlm::Mat4{{
                    std::cos(A), 0.0f, std::sin(A), 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    -std::sin(A), 0.0f, std::cos(A), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                }};
            }
            inline hlm::Mat4 CreateModel() {
                hlm::Mat4 S = CreateScale();
                hlm::Mat4 T = CreateTransiton();
                hlm::Mat4 R = CreateRotation();

                return T * R * S;
            }
        }
    }
}
