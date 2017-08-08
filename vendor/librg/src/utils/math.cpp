#include <librg/utils/math.h>

HINLINE float
HMM_SinF(float Angle)
{
    float Result = 0.0f;

    Result = HMM_SINF(Angle);
    return (Result);
}

HINLINE float
HMM_CosF(float Angle)
{
    float Result = 0.0f;

    Result = HMM_COSF(Angle);
    return (Result);
}

HINLINE float
HMM_TanF(float Radians)
{
    float Result = 0.0f;

    Result = HMM_TANF(Radians);
    return (Result);
}

HINLINE float
HMM_ACosF(float Radians)
{
    float Result = 0.0f;

    Result = HMM_ACOSF(Radians);
    return (Result);
}

HINLINE float
HMM_ATanF(float Radians)
{
    float Result = 0.0f;

    Result = HMM_ATANF(Radians);
    return (Result);
}

HINLINE float
HMM_Atan2F(float Left, float Right)
{
    float Result = 0.0f;

    Result = HMM_ATAN2F(Left, Right);
    return (Result);
}

HINLINE float
HMM_ExpF(float Float)
{
    float Result = 0.0f;

    Result = HMM_EXPF(Float);
    return (Result);
}

HINLINE float
HMM_LogF(float Float)
{
    float Result = 0.0f;

    Result = HMM_LOGF(Float);
    return (Result);
}

HINLINE float
HMM_ToRadians(float Degrees)
{
    float Result = 0.0f;

    Result = Degrees * (HMM_PI32 / 180.0f);
    return (Result);
}

HINLINE float
HMM_SquareRootF(float Value)
{
    float Result = 0.0f;

#ifdef HANDMADE_MATH_NO_SSE
    Result = sqrtf(Value);
#else
    __m128 In = _mm_set_ss(Value);
    __m128 Out = _mm_sqrt_ss(In);
    Result = _mm_cvtss_f32(Out);
#endif

    return(Result);
}

HINLINE float
HMM_RSquareRootF(float Value)
{
    float Result = 0.0f;

#ifdef HANDMADE_MATH_NO_SSE
    Result = 1.0f / HMM_SqrtF(Value);
#else
    __m128 In = _mm_set_ss(Value);
    __m128 Out = _mm_rsqrt_ss(In);
    Result = _mm_cvtss_f32(Out);
#endif

    return(Result);
}

HINLINE float
HMM_LengthSquaredVec2(hmm_vec2 A)
{
    float Result = 0.0f;

    Result = HMM_DotVec2(A, A);

    return(Result);
}

HINLINE float
HMM_LengthSquaredVec3(hmm_vec3 A)
{
    float Result = 0.0f;

    Result = HMM_DotVec3(A, A);

    return (Result);
}

HINLINE float
HMM_LengthSquaredVec4(hmm_vec4 A)
{
    float Result = 0.0f;

    Result = HMM_DotVec4(A, A);

    return(Result);
}

HINLINE float
HMM_LengthVec2(hmm_vec2 A)
{
    float Result = 0.0f;

    Result = HMM_SquareRootF(HMM_LengthSquaredVec2(A));

    return(Result);
}

HINLINE float
HMM_LengthVec3(hmm_vec3 A)
{
    float Result = 0.0f;

    Result = HMM_SquareRootF(HMM_LengthSquaredVec3(A));

    return (Result);
}

HINLINE float
HMM_LengthVec4(hmm_vec4 A)
{
    float Result = 0.0f;

    Result = HMM_SquareRootF(HMM_LengthSquaredVec4(A));

    return(Result);
}

HINLINE float
HMM_Power(float Base, int Exponent)
{
    float Result = 1.0f;
    float Mul = Exponent < 0 ? 1.f / Base : Base;
    unsigned int x = Exponent < 0 ? -Exponent : Exponent;
    while (x)
    {
        if (x & 1)
        {
            Result *= Mul;
        }

        Mul *= Mul;
        x >>= 1;
    }

    return (Result);
}

HINLINE float
HMM_PowerF(float Base, float Exponent)
{
    return HMM_EXPF(Exponent * HMM_LOGF(Base));
}

HINLINE float
HMM_Lerp(float A, float Time, float B)
{
    float Result = 0;

    Result = (1.0f - Time) * A + Time * B;
    return (Result);
}

HINLINE float
HMM_Clamp(float Min, float Value, float Max)
{
    float Result = Value;

    if (Result < Min)
    {
        Result = Min;
    }
    else if (Result > Max)
    {
        Result = Max;
    }

    return (Result);
}

HINLINE hmm_vec2
HMM_NormalizeVec2(hmm_vec2 A)
{
    hmm_vec2 Result = { 0 };

    float VectorLength = HMM_LengthVec2(A);

    Result.x = A.x * (1.0f / VectorLength);
    Result.y = A.y * (1.0f / VectorLength);

    return (Result);
}

HINLINE hmm_vec3
HMM_NormalizeVec3(hmm_vec3 A)
{
    hmm_vec3 Result = { 0 };

    float VectorLength = HMM_LengthVec3(A);

    Result.x = A.x * (1.0f / VectorLength);
    Result.y = A.y * (1.0f / VectorLength);
    Result.z = A.z * (1.0f / VectorLength);

    return (Result);
}

HINLINE hmm_vec4
HMM_NormalizeVec4(hmm_vec4 A)
{
    hmm_vec4 Result = { 0 };

    float VectorLength = HMM_LengthVec4(A);

    Result.x = A.x * (1.0f / VectorLength);
    Result.y = A.y * (1.0f / VectorLength);
    Result.z = A.z * (1.0f / VectorLength);
    Result.w = A.w * (1.0f / VectorLength);

    return (Result);
}

HINLINE float
HMM_DotVec2(hmm_vec2 VecOne, hmm_vec2 VecTwo)
{
    float Result = 0.0f;

    Result = (VecOne.x * VecTwo.x) + (VecOne.y * VecTwo.y);

    return (Result);
}

HINLINE float
HMM_DotVec3(hmm_vec3 VecOne, hmm_vec3 VecTwo)
{
    float Result = 0.0f;

    Result = (VecOne.x * VecTwo.x) + (VecOne.y * VecTwo.y) + (VecOne.z * VecTwo.z);

    return (Result);
}

HINLINE float
HMM_DotVec4(hmm_vec4 VecOne, hmm_vec4 VecTwo)
{
    float Result = 0.0f;

    Result = (VecOne.x * VecTwo.x) + (VecOne.y * VecTwo.y) + (VecOne.z * VecTwo.z) + (VecOne.w * VecTwo.w);

    return (Result);
}

HINLINE hmm_vec3
HMM_Cross(hmm_vec3 VecOne, hmm_vec3 VecTwo)
{
    hmm_vec3 Result = { 0 };

    Result.x = (VecOne.y * VecTwo.z) - (VecOne.z * VecTwo.y);
    Result.y = (VecOne.z * VecTwo.x) - (VecOne.x * VecTwo.z);
    Result.z = (VecOne.x * VecTwo.y) - (VecOne.y * VecTwo.x);

    return (Result);
}

HINLINE hmm_vec2
HMM_Vec2(float x, float y)
{
    hmm_vec2 Result = { 0 };

    Result.x = x;
    Result.y = y;

    return (Result);
}

HINLINE hmm_vec2
HMM_Vec2i(int x, int y)
{
    hmm_vec2 Result = { 0 };

    Result.x = (float)x;
    Result.y = (float)y;

    return (Result);
}

HINLINE hmm_vec3
HMM_Vec3(float x, float y, float z)
{
    hmm_vec3 Result = { 0 };

    Result.x = x;
    Result.y = y;
    Result.z = z;

    return (Result);
}

HINLINE hmm_vec3
HMM_Vec3i(int x, int y, int z)
{
    hmm_vec3 Result = { 0 };

    Result.x = (float)x;
    Result.y = (float)y;
    Result.z = (float)z;

    return (Result);
}

HINLINE hmm_vec4
HMM_Vec4(float x, float y, float z, float w)
{
    hmm_vec4 Result = { 0 };

    Result.x = x;
    Result.y = y;
    Result.z = z;
    Result.w = w;

    return (Result);
}

HINLINE hmm_vec4
HMM_Vec4i(int x, int y, int z, int w)
{
    hmm_vec4 Result = { 0 };

    Result.x = (float)x;
    Result.y = (float)y;
    Result.z = (float)z;
    Result.w = (float)w;

    return (Result);
}

HINLINE hmm_vec4
HMM_Vec4v(hmm_vec3 Vector, float w)
{
    hmm_vec4 Result = { 0 };

    Result.XYZ = Vector;
    Result.w = w;

    return (Result);
}

HINLINE hmm_vec2
HMM_AddVec2(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result.x = Left.x + Right.x;
    Result.y = Left.y + Right.y;

    return (Result);
}

HINLINE hmm_vec3
HMM_AddVec3(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result.x = Left.x + Right.x;
    Result.y = Left.y + Right.y;
    Result.z = Left.z + Right.z;

    return (Result);
}

HINLINE hmm_vec4
HMM_AddVec4(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = { 0 };

    Result.x = Left.x + Right.x;
    Result.y = Left.y + Right.y;
    Result.z = Left.z + Right.z;
    Result.w = Left.w + Right.w;

    return (Result);
}

HINLINE hmm_vec2
HMM_SubtractVec2(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result.x = Left.x - Right.x;
    Result.y = Left.y - Right.y;

    return (Result);
}

HINLINE hmm_vec3
HMM_SubtractVec3(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result.x = Left.x - Right.x;
    Result.y = Left.y - Right.y;
    Result.z = Left.z - Right.z;

    return (Result);
}

HINLINE hmm_vec4
HMM_SubtractVec4(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = { 0 };

    Result.x = Left.x - Right.x;
    Result.y = Left.y - Right.y;
    Result.z = Left.z - Right.z;
    Result.w = Left.w - Right.w;

    return (Result);
}

HINLINE hmm_vec2
HMM_MultiplyVec2(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result.x = Left.x * Right.x;
    Result.y = Left.y * Right.y;

    return (Result);
}

HINLINE hmm_vec2
HMM_MultiplyVec2f(hmm_vec2 Left, float Right)
{
    hmm_vec2 Result = { 0 };

    Result.x = Left.x * Right;
    Result.y = Left.y * Right;

    return (Result);
}

HINLINE hmm_vec3
HMM_MultiplyVec3(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result.x = Left.x * Right.x;
    Result.y = Left.y * Right.y;
    Result.z = Left.z * Right.z;

    return (Result);
}

HINLINE hmm_vec3
HMM_MultiplyVec3f(hmm_vec3 Left, float Right)
{
    hmm_vec3 Result = { 0 };

    Result.x = Left.x * Right;
    Result.y = Left.y * Right;
    Result.z = Left.z * Right;

    return (Result);
}

HINLINE hmm_vec4
HMM_MultiplyVec4(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = { 0 };

    Result.x = Left.x * Right.x;
    Result.y = Left.y * Right.y;
    Result.z = Left.z * Right.z;
    Result.w = Left.w * Right.w;

    return (Result);
}

HINLINE hmm_vec4
HMM_MultiplyVec4f(hmm_vec4 Left, float Right)
{
    hmm_vec4 Result = { 0 };

    Result.x = Left.x * Right;
    Result.y = Left.y * Right;
    Result.z = Left.z * Right;
    Result.w = Left.w * Right;

    return (Result);
}

HINLINE hmm_vec2
HMM_DivideVec2(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result.x = Left.x / Right.x;
    Result.y = Left.y / Right.y;

    return (Result);
}

HINLINE hmm_vec2
HMM_DivideVec2f(hmm_vec2 Left, float Right)
{
    hmm_vec2 Result = { 0 };

    Result.x = Left.x / Right;
    Result.y = Left.y / Right;

    return (Result);
}

HINLINE hmm_vec3
HMM_DivideVec3(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result.x = Left.x / Right.x;
    Result.y = Left.y / Right.y;
    Result.z = Left.z / Right.z;

    return (Result);
}

HINLINE hmm_vec3
HMM_DivideVec3f(hmm_vec3 Left, float Right)
{
    hmm_vec3 Result = { 0 };

    Result.x = Left.x / Right;
    Result.y = Left.y / Right;
    Result.z = Left.z / Right;

    return (Result);
}

HINLINE hmm_vec4
HMM_DivideVec4(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = { 0 };

    Result.x = Left.x / Right.x;
    Result.y = Left.y / Right.y;
    Result.z = Left.z / Right.z;
    Result.w = Left.w / Right.w;

    return (Result);
}

HINLINE hmm_vec4
HMM_DivideVec4f(hmm_vec4 Left, float Right)
{
    hmm_vec4 Result = { 0 };

    Result.x = Left.x / Right;
    Result.y = Left.y / Right;
    Result.z = Left.z / Right;
    Result.w = Left.w / Right;

    return (Result);
}

HINLINE hmm_mat4
HMM_Mat4(void)
{
    hmm_mat4 Result = { 0 };

    return (Result);
}

HINLINE hmm_mat4
HMM_Mat4d(float Diagonal)
{
    hmm_mat4 Result = HMM_Mat4();

    Result.Elements[0][0] = Diagonal;
    Result.Elements[1][1] = Diagonal;
    Result.Elements[2][2] = Diagonal;
    Result.Elements[3][3] = Diagonal;

    return (Result);
}

HINLINE hmm_mat4
HMM_AddMat4(hmm_mat4 Left, hmm_mat4 Right)
{
    hmm_mat4 Result = HMM_Mat4();

    int Columns;
    for (Columns = 0; Columns < 4; ++Columns)
    {
        int Rows;
        for (Rows = 0; Rows < 4; ++Rows)
        {
            Result.Elements[Columns][Rows] = Left.Elements[Columns][Rows] + Right.Elements[Columns][Rows];
        }
    }

    return (Result);
}

HINLINE hmm_mat4
HMM_SubtractMat4(hmm_mat4 Left, hmm_mat4 Right)
{
    hmm_mat4 Result = HMM_Mat4();

    int Columns;
    for (Columns = 0; Columns < 4; ++Columns)
    {
        int Rows;
        for (Rows = 0; Rows < 4; ++Rows)
        {
            Result.Elements[Columns][Rows] = Left.Elements[Columns][Rows] - Right.Elements[Columns][Rows];
        }
    }

    return (Result);
}

HINLINE hmm_mat4
HMM_MultiplyMat4(hmm_mat4 Left, hmm_mat4 Right)
{
    hmm_mat4 Result = HMM_Mat4();

    int Columns;
    for (Columns = 0; Columns < 4; ++Columns)
    {
        int Rows;
        for (Rows = 0; Rows < 4; ++Rows)
        {
            float Sum = 0;
            int CurrentMatrice;
            for (CurrentMatrice = 0; CurrentMatrice < 4; ++CurrentMatrice)
            {
                Sum += Left.Elements[CurrentMatrice][Rows] * Right.Elements[Columns][CurrentMatrice];
            }

            Result.Elements[Columns][Rows] = Sum;
        }
    }

    return (Result);
}

HINLINE hmm_mat4
HMM_MultiplyMat4f(hmm_mat4 Matrix, float Scalar)
{
    hmm_mat4 Result = HMM_Mat4();

    int Columns;
    for (Columns = 0; Columns < 4; ++Columns)
    {
        int Rows;
        for (Rows = 0; Rows < 4; ++Rows)
        {
            Result.Elements[Columns][Rows] = Matrix.Elements[Columns][Rows] * Scalar;
        }
    }

    return (Result);
}

HINLINE hmm_vec4
HMM_MultiplyMat4ByVec4(hmm_mat4 Matrix, hmm_vec4 Vector)
{
    hmm_vec4 Result = { 0 };

    int Columns, Rows;
    for (Rows = 0; Rows < 4; ++Rows)
    {
        float Sum = 0;
        for (Columns = 0; Columns < 4; ++Columns)
        {
            Sum += Matrix.Elements[Columns][Rows] * Vector.Elements[Columns];
        }

        Result.Elements[Rows] = Sum;
    }

    return (Result);
}

HINLINE hmm_mat4
HMM_DivideMat4f(hmm_mat4 Matrix, float Scalar)
{
    hmm_mat4 Result = HMM_Mat4();

    int Columns;
    for (Columns = 0; Columns < 4; ++Columns)
    {
        int Rows;
        for (Rows = 0; Rows < 4; ++Rows)
        {
            Result.Elements[Columns][Rows] = Matrix.Elements[Columns][Rows] / Scalar;
        }
    }

    return (Result);
}

HINLINE hmm_mat4
HMM_Transpose(hmm_mat4 Matrix)
{
    hmm_mat4 Result = HMM_Mat4();

    int Columns;
    for (Columns = 0; Columns < 4; ++Columns)
    {
        int Rows;
        for (Rows = 0; Rows < 4; ++Rows)
        {
            Result.Elements[Rows][Columns] = Matrix.Elements[Columns][Rows];
        }
    }

    return (Result);
}

HINLINE hmm_mat4
HMM_Orthographic(float Left, float Right, float Bottom, float Top, float Near, float Far)
{
    hmm_mat4 Result = HMM_Mat4d(1.0f);

    Result.Elements[0][0] = 2.0f / (Right - Left);
    Result.Elements[1][1] = 2.0f / (Top - Bottom);
    Result.Elements[2][2] = 2.0f / (Near - Far);

    Result.Elements[3][0] = (Left + Right) / (Left - Right);
    Result.Elements[3][1] = (Bottom + Top) / (Bottom - Top);
    Result.Elements[3][2] = (Far + Near) / (Near - Far);

    return (Result);
}

HINLINE hmm_mat4
HMM_Perspective(float FOV, float AspectRatio, float Near, float Far)
{
    hmm_mat4 Result = HMM_Mat4d(1.0f);

    float TanThetaOver2 = HMM_TanF(FOV * (HMM_PI32 / 360.0f));

    Result.Elements[0][0] = 1.0f / TanThetaOver2;
    Result.Elements[1][1] = AspectRatio / TanThetaOver2;
    Result.Elements[2][3] = -1.0f;
    Result.Elements[2][2] = (Near + Far) / (Near - Far);
    Result.Elements[3][2] = (2.0f * Near * Far) / (Near - Far);
    Result.Elements[3][3] = 0.0f;

    return (Result);
}

HINLINE hmm_mat4
HMM_Translate(hmm_vec3 Translation)
{
    hmm_mat4 Result = HMM_Mat4d(1.0f);

    Result.Elements[3][0] = Translation.x;
    Result.Elements[3][1] = Translation.y;
    Result.Elements[3][2] = Translation.z;

    return (Result);
}

HINLINE hmm_mat4
HMM_Rotate(float Angle, hmm_vec3 Axis)
{
    hmm_mat4 Result = HMM_Mat4d(1.0f);

    Axis = HMM_NormalizeVec3(Axis);

    float SinTheta = HMM_SinF(HMM_ToRadians(Angle));
    float CosTheta = HMM_CosF(HMM_ToRadians(Angle));
    float CosValue = 1.0f - CosTheta;

    Result.Elements[0][0] = (Axis.x * Axis.x * CosValue) + CosTheta;
    Result.Elements[0][1] = (Axis.x * Axis.y * CosValue) + (Axis.z * SinTheta);
    Result.Elements[0][2] = (Axis.x * Axis.z * CosValue) - (Axis.y * SinTheta);

    Result.Elements[1][0] = (Axis.y * Axis.x * CosValue) - (Axis.z * SinTheta);
    Result.Elements[1][1] = (Axis.y * Axis.y * CosValue) + CosTheta;
    Result.Elements[1][2] = (Axis.y * Axis.z * CosValue) + (Axis.x * SinTheta);

    Result.Elements[2][0] = (Axis.z * Axis.x * CosValue) + (Axis.y * SinTheta);
    Result.Elements[2][1] = (Axis.z * Axis.y * CosValue) - (Axis.x * SinTheta);
    Result.Elements[2][2] = (Axis.z * Axis.z * CosValue) + CosTheta;

    return (Result);
}

HINLINE hmm_mat4
HMM_Scale(hmm_vec3 Scale)
{
    hmm_mat4 Result = HMM_Mat4d(1.0f);

    Result.Elements[0][0] = Scale.x;
    Result.Elements[1][1] = Scale.y;
    Result.Elements[2][2] = Scale.z;

    return (Result);
}

HINLINE hmm_mat4
HMM_LookAt(hmm_vec3 Eye, hmm_vec3 Center, hmm_vec3 Up)
{
    hmm_mat4 Result = { 0 };

    hmm_vec3 F = HMM_NormalizeVec3(HMM_SubtractVec3(Center, Eye));
    hmm_vec3 S = HMM_NormalizeVec3(HMM_Cross(F, Up));
    hmm_vec3 U = HMM_Cross(S, F);

    Result.Elements[0][0] = S.x;
    Result.Elements[0][1] = U.x;
    Result.Elements[0][2] = -F.x;

    Result.Elements[1][0] = S.y;
    Result.Elements[1][1] = U.y;
    Result.Elements[1][2] = -F.y;

    Result.Elements[2][0] = S.z;
    Result.Elements[2][1] = U.z;
    Result.Elements[2][2] = -F.z;

    Result.Elements[3][0] = -HMM_DotVec3(S, Eye);
    Result.Elements[3][1] = -HMM_DotVec3(U, Eye);
    Result.Elements[3][2] = HMM_DotVec3(F, Eye);
    Result.Elements[3][3] = 1.0f;

    return (Result);
}


HINLINE hmm_quaternion
HMM_Quaternion(float x, float y, float z, float w)
{
    hmm_quaternion Result = { 0 };

    Result.x = x;
    Result.y = y;
    Result.z = z;
    Result.w = w;

    return(Result);
}

HINLINE hmm_quaternion
HMM_QuaternionV4(hmm_vec4 Vector)
{
    hmm_quaternion Result = { 0 };

    Result.x = Vector.x;
    Result.y = Vector.y;
    Result.z = Vector.z;
    Result.w = Vector.w;

    return(Result);
}

HINLINE hmm_quaternion
HMM_AddQuaternion(hmm_quaternion Left, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };

    Result.x = Left.x + Right.x;
    Result.y = Left.y + Right.y;
    Result.z = Left.z + Right.z;
    Result.w = Left.w + Right.w;

    return(Result);
}

HINLINE hmm_quaternion
HMM_SubtractQuaternion(hmm_quaternion Left, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };

    Result.x = Left.x - Right.x;
    Result.y = Left.y - Right.y;
    Result.z = Left.z - Right.z;
    Result.w = Left.w - Right.w;

    return(Result);
}

HINLINE hmm_quaternion
HMM_MultiplyQuaternion(hmm_quaternion Left, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };

    Result.x = (Left.x * Right.w) + (Left.y * Right.z) - (Left.z * Right.y) + (Left.w * Right.x);
    Result.y = (-Left.x * Right.z) + (Left.y * Right.w) + (Left.z * Right.x) + (Left.w * Right.y);
    Result.z = (Left.x * Right.y) - (Left.y * Right.x) + (Left.z * Right.w) + (Left.w * Right.z);
    Result.w = (-Left.x * Right.x) - (Left.y * Right.y) - (Left.z * Right.z) + (Left.w * Right.w);

    return(Result);
}

HINLINE hmm_quaternion
HMM_MultiplyQuaternionF(hmm_quaternion Left, float Multiplicative)
{
    hmm_quaternion Result = { 0 };

    Result.x = Left.x * Multiplicative;
    Result.y = Left.y * Multiplicative;
    Result.z = Left.z * Multiplicative;
    Result.w = Left.w * Multiplicative;

    return(Result);
}

HINLINE hmm_quaternion
HMM_DivideQuaternionF(hmm_quaternion Left, float Dividend)
{
    hmm_quaternion Result = { 0 };

    Result.x = Left.x / Dividend;
    Result.y = Left.y / Dividend;
    Result.z = Left.z / Dividend;
    Result.w = Left.w / Dividend;

    return(Result);
}

HINLINE hmm_quaternion
HMM_InverseQuaternion(hmm_quaternion Left)
{
    hmm_quaternion Conjugate = { 0 };
    hmm_quaternion Result = { 0 };
    float Norm = 0;
    float NormSquared = 0;

    Conjugate.x = -Left.x;
    Conjugate.y = -Left.y;
    Conjugate.z = -Left.z;
    Conjugate.w = Left.w;

    Norm = HMM_SquareRootF(HMM_DotQuaternion(Left, Left));
    NormSquared = Norm * Norm;

    Result.x = Conjugate.x / NormSquared;
    Result.y = Conjugate.y / NormSquared;
    Result.z = Conjugate.z / NormSquared;
    Result.w = Conjugate.w / NormSquared;

    return(Result);
}

HINLINE float
HMM_DotQuaternion(hmm_quaternion Left, hmm_quaternion Right)
{
    float Result = 0.0f;

    Result = (Left.x * Right.x) + (Left.y * Right.y) + (Left.z * Right.z) + (Left.w * Right.w);

    return(Result);
}

HINLINE hmm_quaternion
HMM_NormalizeQuaternion(hmm_quaternion Left)
{
    hmm_quaternion Result = { 0 };

    float Length = HMM_SquareRootF(HMM_DotQuaternion(Left, Left));
    Result = HMM_DivideQuaternionF(Left, Length);

    return(Result);
}

HINLINE hmm_quaternion
HMM_NLerp(hmm_quaternion Left, float Time, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };

    Result.x = HMM_Lerp(Left.x, Time, Right.x);
    Result.y = HMM_Lerp(Left.y, Time, Right.y);
    Result.z = HMM_Lerp(Left.z, Time, Right.z);
    Result.w = HMM_Lerp(Left.w, Time, Right.w);

    Result = HMM_NormalizeQuaternion(Result);

    return(Result);
}

/*
HINLINE hmm_quaternion
HMM_Slerp(hmm_quaternion Left, float Time, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };
    hmm_quaternion QuaternionLeft = { 0 };
    hmm_quaternion QuaternionRight = { 0 };

    float Cos_Theta = HMM_DotQuaternion(Left, Right);
    float Angle = HMM_ACosF(Cos_Theta);

    float S1 = HMM_SinF((1.0f - Time) * Angle);
    float S2 = HMM_SinF(Time * Angle);
    float Is = 1.0f / HMM_SinF(Angle);

    QuaternionLeft = HMM_MultiplyQuaternionF(Left, S1);
    QuaternionRight = HMM_MultiplyQuaternionF(Right, S2);

    Result = HMM_AddQuaternion(QuaternionLeft, QuaternionRight);
    Result = HMM_MultiplyQuaternionF(Result, Is);

    return(Result);
}
*/
HINLINE hmm_quaternion
HMM_QuaternionFromVec3(hmm_vec3 i)
{
    hmm_quaternion q;

    float pitch = i.x;
    float roll  = i.y;
    float yaw   = i.z;

    float t0 = HMM_CosF(yaw * 0.5f);
    float t1 = HMM_SinF(yaw * 0.5f);
    float t2 = HMM_CosF(roll * 0.5f);
    float t3 = HMM_SinF(roll * 0.5f);
    float t4 = HMM_CosF(pitch * 0.5f);
    float t5 = HMM_SinF(pitch * 0.5f);

    q.w = t0 * t2 * t4 + t1 * t3 * t5;
    q.x = t0 * t3 * t4 - t1 * t2 * t5;
    q.y = t0 * t2 * t5 + t1 * t3 * t4;
    q.z = t1 * t2 * t4 - t0 * t3 * t5;

    return q;
}

HINLINE hmm_quaternion
HMM_Slerp(hmm_quaternion v0, float t, hmm_quaternion v1) {
    // Only unit quaternions are valid rotations.
    // Normalize to avoid undefined behavior.
    v0 = HMM_NormalizeQuaternion(v0);
    v1 = HMM_NormalizeQuaternion(v1);

    // Compute the cosine of the angle between the two vectors.
    float dot = HMM_DotQuaternion(v0, v1);

    const double DOT_THRESHOLD = 0.9995;
    if (HMM_ABS(dot) > DOT_THRESHOLD) {
        // If the inputs are too close for comfort, linearly interpolate
        // and normalize the result.

        hmm_quaternion result = HMM_AddQuaternion(v0, HMM_MultiplyQuaternionF(HMM_SubtractQuaternion(v1, v0), t));
        return HMM_NormalizeQuaternion(result);
    }

    // If the dot product is negative, the quaternions
    // have opposite handed-ness and slerp won't take
    // the shorter path. Fix by reversing one quaternion.
    if (dot < 0.0f) {
        v1 = HMM_Quaternion(-v1.x, -v1.y, -v1.z, -v1.w);
        dot = -dot;
    }

    dot = HMM_Clamp(-1, dot, 1);           // Robustness: Stay within domain of acos()
    float theta_0 = HMM_ACosF(dot);  // theta_0 = angle between input vectors
    float theta = theta_0*t;    // theta = angle between v0 and result

    hmm_quaternion v2 = HMM_SubtractQuaternion(v1, HMM_MultiplyQuaternionF(v0, dot));
    v2 = HMM_NormalizeQuaternion(v2);              // { v0, v2 } is now an orthonormal basis

    return HMM_AddQuaternion(HMM_MultiplyQuaternionF(v0, HMM_CosF(theta)), HMM_MultiplyQuaternionF(v2, HMM_SinF(theta)));
}

HINLINE hmm_mat4
HMM_QuaternionToMat4(hmm_quaternion Left)
{
    hmm_mat4 Result = { 0 };
    Result = HMM_Mat4d(1);

    hmm_quaternion NormalizedQuaternion = HMM_NormalizeQuaternion(Left);

    float XX, YY, ZZ,
        XY, XZ, YZ,
        WX, WY, WZ;

    XX = NormalizedQuaternion.x * NormalizedQuaternion.x;
    YY = NormalizedQuaternion.y * NormalizedQuaternion.y;
    ZZ = NormalizedQuaternion.z * NormalizedQuaternion.z;
    XY = NormalizedQuaternion.x * NormalizedQuaternion.y;
    XZ = NormalizedQuaternion.x * NormalizedQuaternion.z;
    YZ = NormalizedQuaternion.y * NormalizedQuaternion.z;
    WX = NormalizedQuaternion.w * NormalizedQuaternion.x;
    WY = NormalizedQuaternion.w * NormalizedQuaternion.y;
    WZ = NormalizedQuaternion.w * NormalizedQuaternion.z;

    Result.Elements[0][0] = 1.0f - 2.0f * (YY + ZZ);
    Result.Elements[0][1] = 2.0f * (XY + WZ);
    Result.Elements[0][2] = 2.0f * (XZ - WY);

    Result.Elements[1][0] = 2.0f * (XY - WZ);
    Result.Elements[1][1] = 1.0f - 2.0f * (XX + ZZ);
    Result.Elements[1][2] = 2.0f * (YZ + WX);

    Result.Elements[2][0] = 2.0f * (XZ + WY);
    Result.Elements[2][1] = 2.0f * (YZ - WX);
    Result.Elements[2][2] = 1.0f - 2.0f * (XX + YY);

    return(Result);
}

HINLINE hmm_vec4
HMM_QuaternionToVec4(hmm_quaternion q)
{
    hmm_vec4 result;

    result.x = q.x;
    result.y = q.y;
    result.z = q.y;
    result.w = q.w;

    return result;
}

HINLINE hmm_quaternion
HMM_QuaternionFromAxisAngle(hmm_vec3 Axis, float AngleOfRotation)
{
    hmm_quaternion Result = { 0 };
    float AxisNorm = 0;
    float SineOfRotation = 0;
    hmm_vec3 RotatedVector = { 0 };

    AxisNorm = HMM_SquareRootF(HMM_DotVec3(Axis, Axis));
    SineOfRotation = HMM_SinF(AngleOfRotation / 2.0f);
    RotatedVector = HMM_MultiplyVec3f(Axis, SineOfRotation);

    Result.w = HMM_CosF(AngleOfRotation / 2.0f);
    Result.XYZ = HMM_DivideVec3f(RotatedVector, AxisNorm);

    return(Result);
}

HINLINE hmm_vec3
HMM_QuaternionToEuler(hmm_quaternion q)
{
    hmm_vec3 result;
    double ysqr = q.y * q.y;

    // roll (x-axis rotation)
    double t0 = +2.0 * (q.w * q.x + q.y * q.z);
    double t1 = +1.0 - 2.0 * (q.x * q.x + ysqr);
    result.x = HMM_ATAN2F(t0, t1);

    // pitch (y-axis rotation)
    double t2 = +2.0 * (q.w * q.y - q.z * q.x);
    t2 = ((t2 > 1.0) ? 1.0 : t2);
    t2 = ((t2 < -1.0) ? -1.0 : t2);
    result.y = HMM_ASINF(t2);

    // yaw (z-axis rotation)
    double t3 = +2.0 * (q.w * q.z + q.x * q.y);
    double t4 = +1.0 - 2.0 * (ysqr + q.z * q.z);
    result.z = HMM_ATAN2F(t3, t4);

    return result;
}

HINLINE bool
HMM_CompareQuaternion(hmm_quaternion q1, hmm_quaternion q2)
{
    return (q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w);
}

#ifdef HANDMADE_MATH_CPP_MODE

HINLINE float
HMM_Length(hmm_vec2 A)
{
    float Result = 0.0f;

    Result = HMM_LengthVec2(A);

    return(Result);
}

HINLINE float
HMM_Length(hmm_vec3 A)
{
    float Result = 0.0f;

    Result = HMM_LengthVec3(A);

    return(Result);
}

HINLINE float
HMM_Length(hmm_vec4 A)
{
    float Result = 0.0f;

    Result = HMM_LengthVec4(A);

    return(Result);
}

HINLINE float
HMM_LengthSquared(hmm_vec2 A)
{
    float Result = 0.0f;

    Result = HMM_LengthSquaredVec2(A);

    return(Result);
}

HINLINE float
HMM_LengthSquared(hmm_vec3 A)
{
    float Result = 0.0f;

    Result = HMM_LengthSquaredVec3(A);

    return(Result);
}

HINLINE float
HMM_LengthSquared(hmm_vec4 A)
{
    float Result = 0.0f;

    Result = HMM_LengthSquaredVec4(A);

    return(Result);
}

HINLINE hmm_vec2
HMM_Normalize(hmm_vec2 A)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_NormalizeVec2(A);

    return(Result);
}

HINLINE hmm_vec3
HMM_Normalize(hmm_vec3 A)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_NormalizeVec3(A);

    return(Result);
}

HINLINE hmm_vec4
HMM_Normalize(hmm_vec4 A)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_NormalizeVec4(A);

    return(Result);
}

HINLINE hmm_quaternion
HMM_Normalize(hmm_quaternion A)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_NormalizeQuaternion(A);

    return(Result);
}

HINLINE float
HMM_Dot(hmm_vec2 VecOne, hmm_vec2 VecTwo)
{
    float Result = 0;

    Result = HMM_DotVec2(VecOne, VecTwo);

    return(Result);
}

HINLINE float
HMM_Dot(hmm_vec3 VecOne, hmm_vec3 VecTwo)
{
    float Result = 0;

    Result = HMM_DotVec3(VecOne, VecTwo);

    return(Result);
}

HINLINE float
HMM_Dot(hmm_vec4 VecOne, hmm_vec4 VecTwo)
{
    float Result = 0;

    Result = HMM_DotVec4(VecOne, VecTwo);

    return(Result);
}

HINLINE float
HMM_Dot(hmm_quaternion QuatOne, hmm_quaternion QuatTwo)
{
    float Result = 0;

    Result = HMM_DotQuaternion(QuatOne, QuatTwo);

    return(Result);
}

HINLINE hmm_vec2
HMM_Add(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_AddVec2(Left, Right);
    return (Result);
}

HINLINE hmm_vec3
HMM_Add(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_AddVec3(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
HMM_Add(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_AddVec4(Left, Right);
    return (Result);
}

HINLINE hmm_mat4
HMM_Add(hmm_mat4 Left, hmm_mat4 Right)
{
    hmm_mat4 Result = { 0 };

    Result = HMM_AddMat4(Left, Right);
    return (Result);
}

HINLINE hmm_quaternion
HMM_Add(hmm_quaternion Left, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_AddQuaternion(Left, Right);
    return(Result);
}

HINLINE hmm_vec2
HMM_Subtract(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_SubtractVec2(Left, Right);
    return (Result);
}

HINLINE hmm_vec3
HMM_Subtract(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_SubtractVec3(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
HMM_Subtract(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_SubtractVec4(Left, Right);
    return (Result);
}

HINLINE hmm_mat4
HMM_Subtract(hmm_mat4 Left, hmm_mat4 Right)
{
    hmm_mat4 Result = { 0 };

    Result = HMM_SubtractMat4(Left, Right);
    return (Result);
}

HINLINE hmm_quaternion
HMM_Subtract(hmm_quaternion Left, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_SubtractQuaternion(Left, Right);
    return (Result);
}

HINLINE hmm_vec2
HMM_Multiply(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_MultiplyVec2(Left, Right);
    return (Result);
}

HINLINE hmm_vec2
HMM_Multiply(hmm_vec2 Left, float Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_MultiplyVec2f(Left, Right);
    return (Result);
}

HINLINE hmm_vec3
HMM_Multiply(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_MultiplyVec3(Left, Right);
    return (Result);
}

HINLINE hmm_vec3
HMM_Multiply(hmm_vec3 Left, float Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_MultiplyVec3f(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
HMM_Multiply(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_MultiplyVec4(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
HMM_Multiply(hmm_vec4 Left, float Right)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_MultiplyVec4f(Left, Right);
    return (Result);
}

HINLINE hmm_mat4
HMM_Multiply(hmm_mat4 Left, hmm_mat4 Right)
{
    hmm_mat4 Result = { 0 };

    Result = HMM_MultiplyMat4(Left, Right);
    return (Result);
}

HINLINE hmm_mat4
HMM_Multiply(hmm_mat4 Left, float Right)
{
    hmm_mat4 Result = { 0 };

    Result = HMM_MultiplyMat4f(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
HMM_Multiply(hmm_mat4 Matrix, hmm_vec4 Vector)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_MultiplyMat4ByVec4(Matrix, Vector);
    return (Result);
}

HINLINE hmm_quaternion
HMM_Multiply(hmm_quaternion Left, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_MultiplyQuaternion(Left, Right);
    return (Result);
}

HINLINE hmm_quaternion
HMM_Multiply(hmm_quaternion Left, float Right)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_MultiplyQuaternionF(Left, Right);
    return (Result);
}

HINLINE hmm_quaternion
HMM_Multiply(float Left, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_MultiplyQuaternionF(Right, Left);
    return (Result);
}

HINLINE hmm_vec2
HMM_Divide(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_DivideVec2(Left, Right);
    return (Result);
}

HINLINE hmm_vec2
HMM_Divide(hmm_vec2 Left, float Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_DivideVec2f(Left, Right);
    return (Result);
}

HINLINE hmm_vec3
HMM_Divide(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_DivideVec3(Left, Right);
    return (Result);
}

HINLINE hmm_vec3
HMM_Divide(hmm_vec3 Left, float Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_DivideVec3f(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
HMM_Divide(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_DivideVec4(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
HMM_Divide(hmm_vec4 Left, float Right)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_DivideVec4f(Left, Right);
    return (Result);
}

HINLINE hmm_mat4
HMM_Divide(hmm_mat4 Left, float Right)
{
    hmm_mat4 Result = { 0 };

    Result = HMM_DivideMat4f(Left, Right);
    return (Result);
}

HINLINE hmm_quaternion
HMM_Divide(hmm_quaternion Left, float Right)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_DivideQuaternionF(Left, Right);
    return (Result);
}

HINLINE hmm_vec2
operator+(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_Add(Left, Right);
    return (Result);
}

HINLINE hmm_vec3
operator+(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_Add(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
operator+(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_Add(Left, Right);
    return (Result);
}

HINLINE hmm_mat4
operator+(hmm_mat4 Left, hmm_mat4 Right)
{
    hmm_mat4 Result = { 0 };

    Result = HMM_Add(Left, Right);
    return (Result);
}

HINLINE hmm_quaternion
operator+(hmm_quaternion Left, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_Add(Left, Right);
    return (Result);
}

HINLINE hmm_vec2
operator-(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_Subtract(Left, Right);
    return (Result);
}

HINLINE hmm_vec3
operator-(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_Subtract(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
operator-(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_Subtract(Left, Right);
    return (Result);
}

HINLINE hmm_mat4
operator-(hmm_mat4 Left, hmm_mat4 Right)
{
    hmm_mat4 Result = { 0 };

    Result = HMM_Subtract(Left, Right);
    return (Result);
}

HINLINE hmm_quaternion
operator-(hmm_quaternion Left, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_Subtract(Left, Right);
    return (Result);
}

HINLINE hmm_vec2
operator*(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_Multiply(Left, Right);
    return (Result);
}

HINLINE hmm_vec3
operator*(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_Multiply(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
operator*(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = HMM_Multiply(Left, Right);

    return (Result);
}

HINLINE hmm_vec2
operator*(hmm_vec2 Left, float Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_Multiply(Left, Right);
    return (Result);
}

HINLINE hmm_vec3
operator*(hmm_vec3 Left, float Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_Multiply(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
operator*(hmm_vec4 Left, float Right)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_Multiply(Left, Right);
    return (Result);
}

HINLINE hmm_mat4
operator*(hmm_mat4 Left, float Right)
{
    hmm_mat4 Result = { 0 };

    Result = HMM_Multiply(Left, Right);
    return (Result);
}

HINLINE hmm_vec2
operator*(float Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_Multiply(Right, Left);
    return (Result);
}

HINLINE hmm_vec3
operator*(float Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_Multiply(Right, Left);
    return (Result);
}

HINLINE hmm_vec4
operator*(float Left, hmm_vec4 Right)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_Multiply(Right, Left);
    return (Result);
}

HINLINE hmm_mat4
operator*(float Left, hmm_mat4 Right)
{
    hmm_mat4 Result = { 0 };

    Result = HMM_Multiply(Right, Left);
    return (Result);
}

HINLINE hmm_mat4
operator*(hmm_mat4 Left, hmm_mat4 Right)
{
    hmm_mat4 Result = { 0 };

    Result = HMM_Multiply(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
operator*(hmm_mat4 Matrix, hmm_vec4 Vector)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_Multiply(Matrix, Vector);
    return (Result);
}

HINLINE hmm_quaternion
operator*(hmm_quaternion Left, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_Multiply(Left, Right);
    return (Result);
}

HINLINE hmm_quaternion
operator*(hmm_quaternion Left, float Right)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_Multiply(Left, Right);
    return (Result);
}

HINLINE hmm_quaternion
operator*(float Left, hmm_quaternion Right)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_Multiply(Right, Left);
    return (Result);
}

HINLINE hmm_vec2
operator/(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_Divide(Left, Right);
    return (Result);
}

HINLINE hmm_vec3
operator/(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_Divide(Left, Right);

    return (Result);
}

HINLINE hmm_vec4
operator/(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_Divide(Left, Right);
    return (Result);
}

HINLINE hmm_vec2
operator/(hmm_vec2 Left, float Right)
{
    hmm_vec2 Result = { 0 };

    Result = HMM_Divide(Left, Right);
    return (Result);
}

HINLINE hmm_vec3
operator/(hmm_vec3 Left, float Right)
{
    hmm_vec3 Result = { 0 };

    Result = HMM_Divide(Left, Right);
    return (Result);
}

HINLINE hmm_vec4
operator/(hmm_vec4 Left, float Right)
{
    hmm_vec4 Result = { 0 };

    Result = HMM_Divide(Left, Right);
    return (Result);
}

HINLINE hmm_mat4
operator/(hmm_mat4 Left, float Right)
{
    hmm_mat4 Result = { 0 };

    Result = HMM_Divide(Left, Right);
    return (Result);
}

HINLINE hmm_quaternion
operator/(hmm_quaternion Left, float Right)
{
    hmm_quaternion Result = { 0 };

    Result = HMM_Divide(Left, Right);
    return (Result);
}

HINLINE hmm_vec2 &
operator+=(hmm_vec2 &Left, hmm_vec2 Right)
{
    return (Left = Left + Right);
}

HINLINE hmm_vec3 &
operator+=(hmm_vec3 &Left, hmm_vec3 Right)
{
    return (Left = Left + Right);
}

HINLINE hmm_vec4 &
operator+=(hmm_vec4 &Left, hmm_vec4 Right)
{
    return (Left = Left + Right);
}

HINLINE hmm_mat4 &
operator+=(hmm_mat4 &Left, hmm_mat4 Right)
{
    return (Left = Left + Right);
}

HINLINE hmm_quaternion &
operator+=(hmm_quaternion &Left, hmm_quaternion Right)
{
    return (Left = Left + Right);
}

HINLINE hmm_vec2 &
operator-=(hmm_vec2 &Left, hmm_vec2 Right)
{
    return (Left = Left - Right);
}

HINLINE hmm_vec3 &
operator-=(hmm_vec3 &Left, hmm_vec3 Right)
{
    return (Left = Left - Right);
}

HINLINE hmm_vec4 &
operator-=(hmm_vec4 &Left, hmm_vec4 Right)
{
    return (Left = Left - Right);
}

HINLINE hmm_mat4 &
operator-=(hmm_mat4 &Left, hmm_mat4 Right)
{
    return (Left = Left - Right);
}

HINLINE hmm_quaternion &
operator-=(hmm_quaternion &Left, hmm_quaternion Right)
{
    return (Left = Left - Right);
}

HINLINE hmm_vec2 &
operator/=(hmm_vec2 &Left, hmm_vec2 Right)
{
    return (Left = Left / Right);
}

HINLINE hmm_vec3 &
operator/=(hmm_vec3 &Left, hmm_vec3 Right)
{
    return (Left = Left / Right);
}

HINLINE hmm_vec4 &
operator/=(hmm_vec4 &Left, hmm_vec4 Right)
{
    return (Left = Left / Right);
}

HINLINE hmm_vec2 &
operator/=(hmm_vec2 &Left, float Right)
{
    return (Left = Left / Right);
}

HINLINE hmm_vec3 &
operator/=(hmm_vec3 &Left, float Right)
{
    return (Left = Left / Right);
}

HINLINE hmm_vec4 &
operator/=(hmm_vec4 &Left, float Right)
{
    return (Left = Left / Right);
}

HINLINE hmm_mat4 &
operator/=(hmm_mat4 &Left, float Right)
{
    return (Left = Left / Right);
}

HINLINE hmm_quaternion &
operator/=(hmm_quaternion &Left, float Right)
{
    return (Left = Left / Right);
}

HINLINE hmm_vec2 &
operator*=(hmm_vec2 &Left, hmm_vec2 Right)
{
    return (Left = Left * Right);
}

HINLINE hmm_vec3 &
operator*=(hmm_vec3 &Left, hmm_vec3 Right)
{
    return (Left = Left * Right);
}

HINLINE hmm_vec4 &
operator*=(hmm_vec4 &Left, hmm_vec4 Right)
{
    return (Left = Left * Right);
}

HINLINE hmm_vec2 &
operator*=(hmm_vec2 &Left, float Right)
{
    return (Left = Left * Right);
}

HINLINE hmm_vec3 &
operator*=(hmm_vec3 &Left, float Right)
{
    return (Left = Left * Right);
}

HINLINE hmm_vec4 &
operator*=(hmm_vec4 &Left, float Right)
{
    return (Left = Left * Right);
}

HINLINE hmm_mat4 &
operator*=(hmm_mat4 &Left, float Right)
{
    return (Left = Left * Right);
}

HINLINE hmm_quaternion &
operator*=(hmm_quaternion &Left, float Right)
{
    return (Left = Left * Right);
}

#endif /* HANDMADE_MATH_CPP_MODE */
