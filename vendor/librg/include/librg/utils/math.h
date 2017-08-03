#ifndef HANDMADE_NO_SSE
#include <xmmintrin.h>
#endif

#ifndef librg_math_h
#define librg_math_h

#ifdef _MSC_VER
#pragma warning(disable:4201)
#endif

#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef HANDMADE_MATH_STATIC
#define HMMDEF static
#else
#define HMMDEF extern
#endif

// #ifdef HANDMADE_MATH_NO_INLINE
// #define HINLINE
// #elif _MSC_VER && !__INTEL_COMPILER
// #define HINLINE __inline
// #else
// #define HINLINE inline
// #endif

#define HINLINE

#if !defined(HMM_SINF) || !defined(HMM_COSF) || !defined(HMM_TANF) || \
    !defined(HMM_EXPF) || !defined(HMM_LOGF) || !defined(HMM_ACOSF) || \
    !defined(HMM_ATANF)|| !defined(HMM_ATAN2F)
#include <math.h>
#endif

#ifndef HMM_SINF
#define HMM_SINF sinf
#endif

#ifndef HMM_COSF
#define HMM_COSF cosf
#endif

#ifndef HMM_TANF
#define HMM_TANF tanf
#endif

#ifndef HMM_EXPF
#define HMM_EXPF expf
#endif

#ifndef HMM_LOGF
#define HMM_LOGF logf
#endif

#ifndef HMM_ACOSF
#define HMM_ACOSF acosf
#endif

#ifndef HMM_ASINF
#define HMM_ASINF asinf
#endif

#ifndef HMM_ATANF
#define HMM_ATANF atanf
#endif

#ifndef HMM_ATAN2F
#define HMM_ATAN2F atan2f
#endif

#define HMM_PI32 3.14159265359f
#define HMM_PI 3.14159265358979323846

#define HMM_MIN(a, b) (a) > (b) ? (b) : (a)
#define HMM_MAX(a, b) (a) < (b) ? (b) : (a)
#define HMM_ABS(a) ((a) > 0 ? (a) : -(a))
#define HMM_MOD(a, m) ((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m))
#define HMM_SQUARE(x) ((x) * (x))

    typedef union hmm_vec2
    {
        struct
        {
            float x, y;
        };

        struct
        {
            float U, V;
        };

        struct
        {
            float Left, Right;
        };

        float Elements[2];
    } hmm_vec2;

    typedef union hmm_vec3
    {
        struct
        {
            float x, y, z;
        };

        struct
        {
            float U, V, w;
        };

        struct
        {
            float R, G, B;
        };

        struct
        {
            hmm_vec2 XY;
            float Ignored0_;
        };

        struct
        {
            float Ignored1_;
            hmm_vec2 YZ;
        };

        struct
        {
            hmm_vec2 UV;
            float Ignored2_;
        };

        struct
        {
            float Ignored3_;
            hmm_vec2 VW;
        };

        float Elements[3];
    } hmm_vec3;

    typedef union hmm_vec4
    {
        struct
        {
            union
            {
                hmm_vec3 XYZ;
                struct
                {
                    float x, y, z;
                };
            };

            float w;
        };
        struct
        {
            union
            {
                hmm_vec3 RGB;
                struct
                {
                    float R, G, B;
                };
            };

            float A;
        };

        struct
        {
            hmm_vec2 XY;
            float Ignored0_;
            float Ignored1_;
        };

        struct
        {
            float Ignored2_;
            hmm_vec2 YZ;
            float Ignored3_;
        };

        struct
        {
            float Ignored4_;
            float Ignored5_;
            hmm_vec2 ZW;
        };

        float Elements[4];
    } hmm_vec4;

    typedef union hmm_mat4
    {
        float Elements[4][4];
    } hmm_mat4;

    typedef union hmm_quaternion
    {
        struct
        {
            union
            {
                hmm_vec3 XYZ;
                struct
                {
                    float x, y, z;
                };
            };

            float w;
        };

        float Elements[4];
    } hmm_quaternion;

    typedef hmm_vec2 hmm_v2;
    typedef hmm_vec3 hmm_v3;
    typedef hmm_vec4 hmm_v4;
    typedef hmm_mat4 hmm_m4;

    HMMDEF float HMM_SinF(float Angle);
    HMMDEF float HMM_TanF(float Angle);
    HMMDEF float HMM_ATanF(float Theta);
    HMMDEF float HMM_ATan2F(float Theta, float Theta2);
    HMMDEF float HMM_CosF(float Angle);
    HMMDEF float HMM_ACosF(float Theta);
    HMMDEF float HMM_ExpF(float Float);
    HMMDEF float HMM_LogF(float Float);

    HMMDEF float HMM_ToRadians(float Degrees);
    HMMDEF float HMM_SquareRootF(float Float);
    HMMDEF float HMM_RSquareRootF(float Float);

    HMMDEF float HMM_LengthSquaredVec2(hmm_vec2 A);
    HMMDEF float HMM_LengthSquaredVec3(hmm_vec3 A);
    HMMDEF float HMM_LengthSquaredVec4(hmm_vec4 A);

    HMMDEF float HMM_LengthVec2(hmm_vec2 A);
    HMMDEF float HMM_LengthVec3(hmm_vec3 A);
    HMMDEF float HMM_LengthVec4(hmm_vec4 A);

    HMMDEF float HMM_Power(float Base, int Exponent);
    HMMDEF float HMM_PowerF(float Base, float Exponent);
    HMMDEF float HMM_Lerp(float A, float Time, float B);
    HMMDEF float HMM_Clamp(float Min, float Value, float Max);

    HMMDEF hmm_vec2 HMM_NormalizeVec2(hmm_vec2 A);
    HMMDEF hmm_vec3 HMM_NormalizeVec3(hmm_vec3 A);
    HMMDEF hmm_vec4 HMM_NormalizeVec4(hmm_vec4 A);

    HMMDEF float HMM_DotVec2(hmm_vec2 VecOne, hmm_vec2 VecTwo);
    HMMDEF float HMM_DotVec3(hmm_vec3 VecOne, hmm_vec3 VecTwo);
    HMMDEF float HMM_DotVec4(hmm_vec4 VecOne, hmm_vec4 VecTwo);

    HMMDEF hmm_vec3 HMM_Cross(hmm_vec3 VecOne, hmm_vec3 VecTwo);

    HMMDEF hmm_vec2 HMM_Vec2(float x, float y);
    HMMDEF hmm_vec2 HMM_Vec2i(int x, int y);
    HMMDEF hmm_vec3 HMM_Vec3(float x, float y, float z);
    HMMDEF hmm_vec3 HMM_Vec3i(int x, int y, int z);
    HMMDEF hmm_vec4 HMM_Vec4(float x, float y, float z, float w);
    HMMDEF hmm_vec4 HMM_Vec4i(int x, int y, int z, int w);
    HMMDEF hmm_vec4 HMM_Vec4v(hmm_vec3 Vector, float w);

    HMMDEF hmm_vec2 HMM_AddVec2(hmm_vec2 Left, hmm_vec2 Right);
    HMMDEF hmm_vec3 HMM_AddVec3(hmm_vec3 Left, hmm_vec3 Right);
    HMMDEF hmm_vec4 HMM_AddVec4(hmm_vec4 Left, hmm_vec4 Right);

    HMMDEF hmm_vec2 HMM_SubtractVec2(hmm_vec2 Left, hmm_vec2 Right);
    HMMDEF hmm_vec3 HMM_SubtractVec3(hmm_vec3 Left, hmm_vec3 Right);
    HMMDEF hmm_vec4 HMM_SubtractVec4(hmm_vec4 Left, hmm_vec4 Right);

    HMMDEF hmm_vec2 HMM_MultiplyVec2(hmm_vec2 Left, hmm_vec2 Right);
    HMMDEF hmm_vec2 HMM_MultiplyVec2f(hmm_vec2 Left, float Right);
    HMMDEF hmm_vec3 HMM_MultiplyVec3(hmm_vec3 Left, hmm_vec3 Right);
    HMMDEF hmm_vec3 HMM_MultiplyVec3f(hmm_vec3 Left, float Right);
    HMMDEF hmm_vec4 HMM_MultiplyVec4(hmm_vec4 Left, hmm_vec4 Right);
    HMMDEF hmm_vec4 HMM_MultiplyVec4f(hmm_vec4 Left, float Right);

    HMMDEF hmm_vec2 HMM_DivideVec2(hmm_vec2 Left, hmm_vec2 Right);
    HMMDEF hmm_vec2 HMM_DivideVec2f(hmm_vec2 Left, float Right);
    HMMDEF hmm_vec3 HMM_DivideVec3(hmm_vec3 Left, hmm_vec3 Right);
    HMMDEF hmm_vec3 HMM_DivideVec3f(hmm_vec3 Left, float Right);
    HMMDEF hmm_vec4 HMM_DivideVec4(hmm_vec4 Left, hmm_vec4 Right);
    HMMDEF hmm_vec4 HMM_DivideVec4f(hmm_vec4 Left, float Right);

    HMMDEF hmm_mat4 HMM_Mat4(void);
    HMMDEF hmm_mat4 HMM_Mat4d(float Diagonal);
    HMMDEF hmm_mat4 HMM_AddMat4(hmm_mat4 Left, hmm_mat4 Right);
    HMMDEF hmm_mat4 HMM_SubtractMat4(hmm_mat4 Left, hmm_mat4 Right);
    HMMDEF hmm_mat4 HMM_MultiplyMat4(hmm_mat4 Left, hmm_mat4 Right);
    HMMDEF hmm_mat4 HMM_MultiplyMat4f(hmm_mat4 Matrix, float Scalar);
    HMMDEF hmm_vec4 HMM_MultiplyMat4ByVec4(hmm_mat4 Matrix, hmm_vec4 Vector);
    HMMDEF hmm_mat4 HMM_DivideMat4f(hmm_mat4 Matrix, float Scalar);

    HMMDEF hmm_mat4 HMM_Transpose(hmm_mat4 Matrix);

    HMMDEF hmm_mat4 HMM_Orthographic(float Left, float Right, float Bottom, float Top, float Near, float Far);
    HMMDEF hmm_mat4 HMM_Perspective(float FOV, float AspectRatio, float Near, float Far);

    HMMDEF hmm_mat4 HMM_Translate(hmm_vec3 Translation);
    HMMDEF hmm_mat4 HMM_Rotate(float Angle, hmm_vec3 Axis);
    HMMDEF hmm_mat4 HMM_Scale(hmm_vec3 Scale);

    HMMDEF hmm_mat4 HMM_LookAt(hmm_vec3 Eye, hmm_vec3 Center, hmm_vec3 Up);

    HMMDEF hmm_quaternion HMM_Quaternion(float x, float y, float z, float w);
    HMMDEF hmm_quaternion HMM_QuaternionV4(hmm_vec4 Vector);
    HMMDEF hmm_quaternion HMM_QuaternionFromVec3(hmm_vec3 Vector);
    HMMDEF hmm_quaternion HMM_AddQuaternion(hmm_quaternion Left, hmm_quaternion Right);
    HMMDEF hmm_quaternion HMM_SubtractQuaternion(hmm_quaternion Left, hmm_quaternion Right);
    HMMDEF hmm_quaternion HMM_MultiplyQuaternion(hmm_quaternion Left, hmm_quaternion Right);
    HMMDEF hmm_quaternion HMM_MultiplyQuaternionF(hmm_quaternion Left, float Multiplicative);
    HMMDEF hmm_quaternion HMM_DivideQuaternionF(hmm_quaternion Left, float Dividend);
    HMMDEF hmm_quaternion HMM_InverseQuaternion(hmm_quaternion Left);
    HMMDEF float HMM_DotQuaternion(hmm_quaternion Left, hmm_quaternion Right);
    HMMDEF bool HMM_CompareQuaternion(hmm_quaternion Left, hmm_quaternion Right);
    HMMDEF hmm_quaternion HMM_NormalizeQuaternion(hmm_quaternion Left);
    HMMDEF hmm_quaternion HMM_NLerp(hmm_quaternion Left, float Time, hmm_quaternion Right);
    HMMDEF hmm_quaternion HMM_Slerp(hmm_quaternion Left, float Time, hmm_quaternion Right);
    HMMDEF hmm_mat4 HMM_QuaternionToMat4(hmm_quaternion Left);
    HMMDEF hmm_vec4 HMM_QuaternionToVec4(hmm_quaternion Quaternion);
    HMMDEF hmm_quaternion HMM_QuaternionFromAxisAngle(hmm_vec3 Axis, float AngleOfRotation);
    HMMDEF hmm_vec3 HMM_QuaternionToEuler(hmm_quaternion Quaternion);

#ifdef __cplusplus
}
#endif

#if 1 // TODO: #ifdef HANDMADE_MATH_CPP_MODE

HMMDEF float HMM_Length(hmm_vec2 A);
HMMDEF float HMM_Length(hmm_vec3 A);
HMMDEF float HMM_Length(hmm_vec4 A);

HMMDEF float HMM_LengthSquared(hmm_vec2 A);
HMMDEF float HMM_LengthSquared(hmm_vec3 A);
HMMDEF float HMM_LengthSquared(hmm_vec4 A);

HMMDEF hmm_vec2 HMM_Normalize(hmm_vec2 A);
HMMDEF hmm_vec3 HMM_Normalize(hmm_vec3 A);
HMMDEF hmm_vec4 HMM_Normalize(hmm_vec4 A);
HMMDEF hmm_quaternion HMM_Normalize(hmm_quaternion A);

HMMDEF float HMM_Dot(hmm_vec2 VecOne, hmm_vec2 VecTwo);
HMMDEF float HMM_Dot(hmm_vec3 VecOne, hmm_vec3 VecTwo);
HMMDEF float HMM_Dot(hmm_vec4 VecOne, hmm_vec4 VecTwo);
HMMDEF float HMM_Dot(hmm_quaternion QuatOne, hmm_quaternion QuatTwo);

HMMDEF hmm_vec2 HMM_Add(hmm_vec2 Left, hmm_vec2 Right);
HMMDEF hmm_vec3 HMM_Add(hmm_vec3 Left, hmm_vec3 Right);
HMMDEF hmm_vec4 HMM_Add(hmm_vec4 Left, hmm_vec4 Right);
HMMDEF hmm_mat4 HMM_Add(hmm_mat4 Left, hmm_mat4 Right);
HMMDEF hmm_quaternion HMM_Add(hmm_quaternion Left, hmm_quaternion Right);

HMMDEF hmm_vec2 HMM_Subtract(hmm_vec2 Left, hmm_vec2 Right);
HMMDEF hmm_vec3 HMM_Subtract(hmm_vec3 Left, hmm_vec3 Right);
HMMDEF hmm_vec4 HMM_Subtract(hmm_vec4 Left, hmm_vec4 Right);
HMMDEF hmm_mat4 HMM_Subtract(hmm_mat4 Left, hmm_mat4 Right);
HMMDEF hmm_quaternion HMM_Subtract(hmm_quaternion Left, hmm_quaternion Right);

HMMDEF hmm_vec2 HMM_Multiply(hmm_vec2 Left, hmm_vec2 Right);
HMMDEF hmm_vec2 HMM_Multiply(hmm_vec2 Left, float Right);
HMMDEF hmm_vec3 HMM_Multiply(hmm_vec3 Left, hmm_vec3 Right);
HMMDEF hmm_vec3 HMM_Multiply(hmm_vec3 Left, float Right);
HMMDEF hmm_vec4 HMM_Multiply(hmm_vec4 Left, hmm_vec4 Right);
HMMDEF hmm_vec4 HMM_Multiply(hmm_vec4 Left, float Right);
HMMDEF hmm_mat4 HMM_Multiply(hmm_mat4 Left, hmm_mat4 Right);
HMMDEF hmm_mat4 HMM_Multiply(hmm_mat4 Left, float Right);
HMMDEF hmm_vec4 HMM_Multiply(hmm_mat4 Matrix, hmm_vec4 Vector);
HMMDEF hmm_quaternion HMM_Multiply(hmm_quaternion Left, hmm_quaternion Right);
HMMDEF hmm_quaternion HMM_Multiply(hmm_quaternion Left, float Right);

HMMDEF hmm_vec2 HMM_Divide(hmm_vec2 Left, hmm_vec2 Right);
HMMDEF hmm_vec2 HMM_Divide(hmm_vec2 Left, float Right);
HMMDEF hmm_vec3 HMM_Divide(hmm_vec3 Left, hmm_vec3 Right);
HMMDEF hmm_vec3 HMM_Divide(hmm_vec3 Left, float Right);
HMMDEF hmm_vec4 HMM_Divide(hmm_vec4 Left, hmm_vec4 Right);
HMMDEF hmm_vec4 HMM_Divide(hmm_vec4 Left, float Right);
HMMDEF hmm_mat4 HMM_Divide(hmm_mat4 Left, float Right);
HMMDEF hmm_quaternion HMM_Divide(hmm_quaternion Left, hmm_quaternion Right);
HMMDEF hmm_quaternion HMM_Divide(hmm_quaternion Left, float Right);

HMMDEF hmm_vec2 operator+(hmm_vec2 Left, hmm_vec2 Right);
HMMDEF hmm_vec3 operator+(hmm_vec3 Left, hmm_vec3 Right);
HMMDEF hmm_vec4 operator+(hmm_vec4 Left, hmm_vec4 Right);
HMMDEF hmm_mat4 operator+(hmm_mat4 Left, hmm_mat4 Right);
HMMDEF hmm_quaternion operator+(hmm_quaternion Left, hmm_quaternion Right);

HMMDEF hmm_vec2 operator-(hmm_vec2 Left, hmm_vec2 Right);
HMMDEF hmm_vec3 operator-(hmm_vec3 Left, hmm_vec3 Right);
HMMDEF hmm_vec4 operator-(hmm_vec4 Left, hmm_vec4 Right);
HMMDEF hmm_mat4 operator-(hmm_mat4 Left, hmm_mat4 Right);
HMMDEF hmm_quaternion operator-(hmm_quaternion Left, hmm_quaternion Right);

HMMDEF hmm_vec2 operator*(hmm_vec2 Left, hmm_vec2 Right);
HMMDEF hmm_vec3 operator*(hmm_vec3 Left, hmm_vec3 Right);
HMMDEF hmm_vec4 operator*(hmm_vec4 Left, hmm_vec4 Right);
HMMDEF hmm_mat4 operator*(hmm_mat4 Left, hmm_mat4 Right);
HMMDEF hmm_quaternion operator*(hmm_quaternion Left, hmm_quaternion Right);

HMMDEF hmm_vec2 operator*(hmm_vec2 Left, float Right);
HMMDEF hmm_vec3 operator*(hmm_vec3 Left, float Right);
HMMDEF hmm_vec4 operator*(hmm_vec4 Left, float Right);
HMMDEF hmm_mat4 operator*(hmm_mat4 Left, float Right);
HMMDEF hmm_quaternion operator*(hmm_quaternion Left, float Right);

HMMDEF hmm_vec2 operator*(float Left, hmm_vec2 Right);
HMMDEF hmm_vec3 operator*(float Left, hmm_vec3 Right);
HMMDEF hmm_vec4 operator*(float Left, hmm_vec4 Right);
HMMDEF hmm_mat4 operator*(float Left, hmm_mat4 Right);
HMMDEF hmm_quaternion operator*(float Left, hmm_quaternion Right);

HMMDEF hmm_vec4 operator*(hmm_mat4 Matrix, hmm_vec4 Vector);

HMMDEF hmm_vec2 operator/(hmm_vec2 Left, hmm_vec2 Right);
HMMDEF hmm_vec3 operator/(hmm_vec3 Left, hmm_vec3 Right);
HMMDEF hmm_vec4 operator/(hmm_vec4 Left, hmm_vec4 Right);

HMMDEF hmm_vec2 operator/(hmm_vec2 Left, float Right);
HMMDEF hmm_vec3 operator/(hmm_vec3 Left, float Right);
HMMDEF hmm_vec4 operator/(hmm_vec4 Left, float Right);
HMMDEF hmm_mat4 operator/(hmm_mat4 Left, float Right);
HMMDEF hmm_quaternion operator/(hmm_quaternion Left, float Right);

HMMDEF hmm_vec2 &operator+=(hmm_vec2 &Left, hmm_vec2 Right);
HMMDEF hmm_vec3 &operator+=(hmm_vec3 &Left, hmm_vec3 Right);
HMMDEF hmm_vec4 &operator+=(hmm_vec4 &Left, hmm_vec4 Right);
HMMDEF hmm_mat4 &operator+=(hmm_mat4 &Left, hmm_mat4 Right);
HMMDEF hmm_quaternion &operator+=(hmm_quaternion &Left, hmm_quaternion Right);

HMMDEF hmm_vec2 &operator-=(hmm_vec2 &Left, hmm_vec2 Right);
HMMDEF hmm_vec3 &operator-=(hmm_vec3 &Left, hmm_vec3 Right);
HMMDEF hmm_vec4 &operator-=(hmm_vec4 &Left, hmm_vec4 Right);
HMMDEF hmm_mat4 &operator-=(hmm_mat4 &Left, hmm_mat4 Right);
HMMDEF hmm_quaternion &operator-=(hmm_quaternion &Left, hmm_quaternion Right);

HMMDEF hmm_vec2 &operator*=(hmm_vec2 &Left, hmm_vec2 Right);
HMMDEF hmm_vec3 &operator*=(hmm_vec3 &Left, hmm_vec3 Right);
HMMDEF hmm_vec4 &operator*=(hmm_vec4 &Left, hmm_vec4 Right);

HMMDEF hmm_vec2 &operator*=(hmm_vec2 &Left, float Right);
HMMDEF hmm_vec3 &operator*=(hmm_vec3 &Left, float Right);
HMMDEF hmm_vec4 &operator*=(hmm_vec4 &Left, float Right);
HMMDEF hmm_mat4 &operator*=(hmm_mat4 &Left, float Right);
HMMDEF hmm_quaternion &operator*=(hmm_quaternion &Left, float Right);

HMMDEF hmm_vec2 &operator/=(hmm_vec2 &Left, hmm_vec2 Right);
HMMDEF hmm_vec3 &operator/=(hmm_vec3 &Left, hmm_vec3 Right);
HMMDEF hmm_vec4 &operator/=(hmm_vec4 &Left, hmm_vec4 Right);

HMMDEF hmm_vec2 &operator/=(hmm_vec2 &Left, float Right);
HMMDEF hmm_vec3 &operator/=(hmm_vec3 &Left, float Right);
HMMDEF hmm_vec4 &operator/=(hmm_vec4 &Left, float Right);
HMMDEF hmm_mat4 &operator/=(hmm_mat4 &Left, float Right);
HMMDEF hmm_quaternion &operator/=(hmm_quaternion &Left, float Right);

#endif /* HANDMADE_MATH_CPP */

#ifdef __clang__
#pragma GCC diagnostic pop
#endif

#endif /* librg_math_h */


/*
HandmadeMath.h v1.1.2

This is a single header file with a bunch of useful functions for
basic game math operations.

==========================================================================

You MUST

#define HANDMADE_MATH_IMPLEMENTATION

in EXACTLY one C or C++ file that includes this header, BEFORE the
include, like this:

#define HANDMADE_MATH_IMPLEMENTATION
#include "HandmadeMath.h"

All other files should just #include "HandmadeMath.h" without the #define.

==========================================================================

For overloaded and operator overloaded versions of the base C functions,
you MUST

#define HANDMADE_MATH_CPP_MODE

in EXACTLY one C or C++ file that includes this header, BEFORE the
include, like this:

#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_CPP_MODE
#include "HandmadeMath.h"

All other files should just #include "HandmadeMath.h" without the #define.

==========================================================================

To disable SSE intrinsics, you MUST

#define HANDMADE_MATH_NO_SSE

in EXACTLY one C or C++ file that includes this header, BEFORE the
include, like this:

#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_CPP_MODE
#define HANDMADE_MATH_NO_SSE
#include "HandmadeMath.h"

or

#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_NO_SSE
#include "HandmadeMath.h"

==========================================================================

To disable inlining functions, you MUST

#define HANDMADE_MATH_NO_INLINE

in EXACTLY one C or C++ file that includes this header, BEFORE the
include, like this:

#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_CPP_MODE
#define HANDMADE_MATH_NO_INLINE
#include "HandmadeMath.h"

All other files should just #include "HandmadeMath.h" without the #define.

==========================================================================

To Disable the CRT, you MUST

#define HMM_SINF MySinF
#define HMM_COSF MyCosF
#define HMM_TANF MyTanF
#define HMM_EXPF MyExpF
#define HMM_LOGF MyLogF
#define HMM_ACOSF MyACosF
#define HMM_ATANF MyATanF
#define HMM_ATAN2F MYATan2F

Provide your own implementations of SinF, CosF, TanF, ACosF, ATanF, ATan2F, ExpF and LogF
in EXACTLY one C or C++ file that includes this header, BEFORE the
include, like this:

#define HMM_SINF MySinF
#define HMM_COSF MyCosF
#define HMM_TANF MyTanF
#define HMM_EXPF MyExpF
#define HMM_LOGF MyLogF
#define HMM_ACOSF MyACosF
#define HMM_ATANF MyATanF
#define HMM_ATAN2F MyATan2F
#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_CPP_MODE
#include "HandmadeMath.h"

If you do not define all five of these, HandmadeMath.h will use the
versions of these functions that are provided by the CRT.

==========================================================================

Version History:
0.2 (*) Updated documentation
(*) Better C compliance
(*) Prefix all handmade math functions
(*) Better operator overloading
0.2a
(*) Prefixed Macros
0.2b
(*) Disabled warning 4201 on MSVC as it is legal is C11
(*) Removed the f at the end of HMM_PI to get 64bit precision
0.3
(*) Added +=, -=, *=, /= for hmm_vec2, hmm_vec3, hmm_vec4
0.4
(*) SSE Optimized HMM_SqrtF
(*) SSE Optimized HMM_RSqrtF
(*) Removed CRT
0.5
(*) Added scalar multiplication and division for vectors
and matrices
(*) Added matrix subtraction and += for hmm_mat4
(*) Reconciled all headers and implementations
(*) Tidied up, and filled in a few missing operators
0.5.1
(*) Ensured column-major order for matrices throughout
(*) Fixed HMM_Translate producing row-major matrices
0.5.2
(*) Fixed SSE code in HMM_SqrtF
(*) Fixed SSE code in HMM_RSqrtF
0.6
(*) Added Unit testing
(*) Made HMM_Power faster
(*) Fixed possible efficiency problem with HMM_Normalize
(*) RENAMED HMM_LengthSquareRoot to HMM_LengthSquared
(*) RENAMED HMM_RSqrtF to HMM_RSquareRootF
(*) RENAMED HMM_SqrtF to HMM_SquareRootF
(*) REMOVED Inner function (user should use Dot now)
(*) REMOVED HMM_FastInverseSquareRoot function declaration
0.7
(*) REMOVED HMM_LengthSquared in HANDMADE_MATH_IMPLEMENTATION  (should use HMM_LengthSquaredVec3, or HANDMADE_MATH_CPP_MODE for function overloaded version)
(*) REMOVED HMM_Length in HANDMADE_MATH_IMPLEMENTATION (should use HMM_LengthVec3, HANDMADE_MATH_CPP_MODE for function overloaded version)
(*) REMOVED HMM_Normalize in HANDMADE_MATH_IMPLEMENTATION (should use HMM_NormalizeVec3, or HANDMADE_MATH_CPP_MODE for function overloaded version)
(*) Added HMM_LengthSquaredVec2
(*) Added HMM_LengthSquaredVec4
(*) Addd HMM_LengthVec2
(*) Added HMM_LengthVec4
(*) Added HMM_NormalizeVec2
(*) Added HMM_NormalizeVec4
1.0
(*) Lots of testing!
1.1
(*) Quaternion support
(*) Added type hmm_quaternion
(*) Added HMM_Quaternion
(*) Added HMM_QuaternionV4
(*) Added HMM_AddQuaternion
(*) Added HMM_SubtractQuaternion
(*) Added HMM_MultiplyQuaternion
(*) Added HMM_MultiplyQuaternionF
(*) Added HMM_DivideQuaternionF
(*) Added HMM_InverseQuaternion
(*) Added HMM_DotQuaternion
(*) Added HMM_NormalizeQuaternion
(*) Added HMM_Slerp
(*) Added HMM_QuaternionToMat4
(*) Added HMM_QuaternionFromAxisAngle
1.1.1
(*) Resolved compiler warnings on gcc and g++
1.1.2
(*) Fixed invalid HMMDEF's in the function definitions
1.1.3
(*) Fixed compile error in C mode

LICENSE

This software is in the public domain. Where that dedication is not
recognized, you are granted a perpetual, irrevocable license to copy,
distribute, and modify this file as you see fit.

CREDITS

Written by Zakary Strange (zak@handmade.network && @strangezak)

Functionality:
Matt Mascarenhas (@miblo_)
Aleph
FieryDrake (@fierydrake)
Gingerbill (@TheGingerBill)
Ben Visness (@bvisness)
Trinton Bullard (@Peliex_Dev)

Fixes:
Jeroen van Rijn (@J_vanRijn)
Kiljacken (@Kiljacken)
Insofaras (@insofaras)
*/
