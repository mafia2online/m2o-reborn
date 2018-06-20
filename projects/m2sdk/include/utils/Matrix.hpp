#pragma once

namespace M2
{
    namespace Utils
    {
        struct Matrix34 {
            FLOAT _11; FLOAT _12; FLOAT _13; FLOAT _14;
            FLOAT _21; FLOAT _22; FLOAT _23; FLOAT _24;
            FLOAT _31; FLOAT _32; FLOAT _33; FLOAT _34;
            FLOAT _41; FLOAT _42; FLOAT _43; FLOAT _44;
        };


        class Matrix43
        {

        public:

            float	_11, _12, _13;
            float	_21, _22, _23;
            float	_31, _32, _33;
            float	_41, _42, _43;

            Matrix43(void)
            {
                Identity();
            }

            Matrix43(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33, float m41, float m42, float m43)
            {
                _11 = m11;	_12 = m12;	_13 = m13;
                _21 = m21;	_22 = m22;	_23 = m23;
                _31 = m31;	_32 = m32;	_33 = m33;
                _41 = m41;	_42 = m42;	_43 = m43;
            }

            void Identity(void)
            {
                _11 = 1.0f;	_12 = 0.0f;	_13 = 0.0f;
                _21 = 0.0f;	_22 = 1.0f;	_23 = 0.0f;
                _31 = 0.0f;	_32 = 0.0f;	_33 = 1.0f;
                _41 = 0.0f;	_42 = 0.0f;	_43 = 0.0f;
            }

            Matrix43 operator * (const Matrix43& right) const
            {
                Matrix43 matMatrix;
                float ** mat1 = (float **)this;
                float ** mat2 = (float **)&matMatrix;
                float ** mat3 = (float **)&right;

                for (int row = 0; row < 3; row++)
                {
                    for (int col = 0; col < 3; col++)
                    {
                        mat2[row][col] =
                            mat1[row][0] * mat3[0][col] +
                            mat1[row][1] * mat3[1][col] +
                            mat1[row][2] * mat3[2][col];
                    }
                }

                return matMatrix;
            }

        };


        class CMatrix {

        public:
            union {
                struct {
                    float        _11, _12, _13, _14;
                    float        _21, _22, _23, _24;
                    float        _31, _32, _33, _34;
                    float        _41, _42, _43, _44;

                };
                float m[4][4];
            };

        };

        static inline Matrix43* CreateEmptyMatrix()
        {
            Matrix43* new_matrix = new Matrix43;
            unsigned long matrix_alloc = 0x0041EA80;
            __asm
            {
                mov ecx, new_matrix
                call matrix_alloc
            }

            return new_matrix;
        }
    };
};
