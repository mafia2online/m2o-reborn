#pragma once

#ifdef _WIN32
#include <windows.h>
#else
typedef unsigned long DWORD;
#endif

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

class Matrix
{

public:

	float	_11, _12, _13, _14;
	float	_21, _22, _23, _24;
	float	_31, _32, _33, _34;
	float	_41, _42, _43, _44;

	Matrix(void)
	{
		memset(this, 0, sizeof(Matrix));
	}

	void ToMatrix(Matrix43 * matMatrix) const
	{
		matMatrix->_11 = _11;	matMatrix->_12 = _12;	matMatrix->_13 = _13;
		matMatrix->_21 = _21;	matMatrix->_22 = _22;	matMatrix->_23 = _23;
		matMatrix->_31 = _31;	matMatrix->_32 = _32;	matMatrix->_33 = _33;
		matMatrix->_41 = _41;	matMatrix->_42 = _42;	matMatrix->_43 = _43;
	}

	void FromMatrix(Matrix43 * matMatrix)
	{
		_11 = matMatrix->_11;	_12 = matMatrix->_12;	_13 = matMatrix->_13;	_14 = 0.0f;
		_21 = matMatrix->_21;	_22 = matMatrix->_22;	_23 = matMatrix->_23;	_24 = 0.0f;
		_31 = matMatrix->_31;	_32 = matMatrix->_32;	_33 = matMatrix->_33;	_34 = 0.0f;
		_41 = matMatrix->_41;	_42 = matMatrix->_42;	_43 = matMatrix->_43;	_44 = 1.0f;
	}

};