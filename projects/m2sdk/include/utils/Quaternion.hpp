#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#include "Vector3.hpp"
#include <math.h>

class Quaternion
{

public:

    float x;
    float y;
    float z;
    float w;

    Quaternion ( void )
    {
        w = x = y = z = 0.0f;
    }

    Quaternion ( float _w, float _x, float _y, float _z )
    {
        w = _w;
        x = _x;
        y = _y;
        z = _z;
    }

    Quaternion ( float _x, float _y, float _z )
    {
        // Calculate the cos angles
        float fCosZ = cosf( 0.5f * _z );
        float fCosY = cosf( 0.5f * _y );
        float fCosX = cosf( 0.5f * _x );

        // Calculate the sin angles
        float fSinZ = sinf( 0.5f * _z );
        float fSinY = sinf( 0.5f * _y );
        float fSinX = sinf( 0.5f * _x );

        // Compute the quaternion
        w = fCosZ * fCosY * fCosX + fSinZ * fSinY * fSinX;
        z = fCosZ * fCosY * fSinX - fSinZ * fSinY * fCosX;
        y = fCosZ * fSinY * fCosX + fSinZ * fCosY * fSinX;
        x = fSinZ * fCosY * fCosX - fCosZ * fSinY * fSinX;
    }

    Quaternion ( Vector3 vecEular )
    {
        // Calculate the cos angles
        float fCosZ = cosf( 0.5f * vecEular.z );
        float fCosY = cosf( 0.5f * vecEular.y );
        float fCosX = cosf( 0.5f * vecEular.x );

        // Calculate the sin angles
        float fSinZ = sinf( 0.5f * vecEular.z );
        float fSinY = sinf( 0.5f * vecEular.y );
        float fSinX = sinf( 0.5f * vecEular.x );

        // Compute the quaternion
        w = fCosZ * fCosY * fCosX + fSinZ * fSinY * fSinX;
        z = fCosZ * fCosY * fSinX - fSinZ * fSinY * fCosX;
        y = fCosZ * fSinY * fCosX + fSinZ * fCosY * fSinX;
        x = fSinZ * fCosY * fCosX - fCosZ * fSinY * fSinX;
    }

    // ===================== Operators =====================

    Quaternion operator + (const Quaternion &q) const
    {
        return Quaternion( (w + q.w), (x + q.x), (y + q.y), (z + q.z) );
    }

    Quaternion operator - (const Quaternion &q) const
    {
        return Quaternion( (w - q.w), (x - q.x), (y - q.y), (z - q.z) );
    }

    Quaternion operator * (const Quaternion &q) const
    {
        return Quaternion( (w * q.w) - ((x * q.x) + (y * q.y) + (z * q.z)),
            (y * q.z - z * q.y + w * q.x + x * q.w),
            (z * q.x - x * q.z + w * q.y + y * q.w),
            (x * q.y - y * q.x + w * q.z + z * q.w) );
    }

    Quaternion operator * (float fScale) const
    {
        return Quaternion( (w * fScale), (x * fScale), (y * fScale), (z * fScale) );
    }

    Quaternion operator / (const Quaternion &q) const
    {
        Quaternion p( q );
        p.invert();

        return (*this * p);
    }

    Quaternion operator / (float fScale) const
    {
        return Quaternion( (w / fScale), (x / fScale), (y / fScale), (z / fScale) );
    }

    Quaternion operator /= (float fScale)
    {
        w /= fScale;
        x /= fScale;
        y /= fScale;
        z /= fScale;
        return *this;
    }

    Quaternion operator -() const
    {
        return Quaternion( -w, -x, -y, -z );
    }

    void operator = (const Quaternion &q)
    {
        w = q.w;
        x = q.x;
        y = q.y;
        z = q.z;
    }

    void operator += (const Quaternion &q)
    {
        w += q.w;
        x += q.x;
        y += q.y;
        z += q.z;
    }

    void operator -= (const Quaternion &q)
    {
        w -= q.w;
        x -= q.x;
        y -= q.y;
        z -= q.z;
    }

    void operator *= (const Quaternion &q)
    {
        w *= q.w;
        x *= q.x;
        y *= q.y;
        z *= q.z;
    }

    void operator /= (const Quaternion &q)
    {
        w /= q.w;
        x /= q.x;
        y /= q.y;
        z /= q.z;
    }

    // ===================== Functions =====================

    float length ( void )
    {
        return (float)sqrt( w * w + x * x + y * y + z * z );
    }

    float length_squared ( void )
    {
        return (float)( w * w + x * x + y * y + z * z );
    }

    void normalize ( void )
    {
        w /= length();
        x /= length();
        y /= length();
        z /= length();
    }
    
    void conjugate ( void )
    {
        x = -x;
        y = -y;
        z = -z;
    }

    void invert ( void )
    {
        conjugate();
        *this /= length_squared();
    }

    float dot ( Quaternion q1, Quaternion q2 )
    {
        return (q1.x * q2.x + q1.y * q2.y + q1.z + q2.z + q1.w * q2.w);
    }

    void lerp ( Quaternion quatTarget, float delta )
    {
        *this = (*this * (1.0f - delta) + quatTarget * delta);
    }

    void slerp ( Quaternion quatTarget, float delta )
    {
        Quaternion quatTemp;
        float fDot = dot(*this, quatTarget);

        if( fDot < 0 )
        {
            fDot = -fDot;
            quatTemp = -quatTarget;
        }
        else
            quatTemp = quatTarget;

        if( fDot < 0.95f )
        {
            float fAngle = acosf( fDot );
            *this = (*this * sinf( fAngle * (1.0f - delta) ) + quatTemp * sinf( fAngle * delta )) / sinf( fAngle );
        }
        else
            lerp( quatTemp, delta );
    }

    void slerpNoInvert ( Quaternion quatTarget, float delta )
    {
        float fDot = dot( *this, quatTarget );

        if( fDot > -0.95f && fDot < 0.95f )
        {
            float fAngle = acosf( fDot );
            *this = (*this * sinf( fAngle * (1.0f - delta)) + quatTarget * sinf( fAngle * delta )) / sinf( fAngle );
        }
        else
            lerp( quatTarget, delta );
    }

    void toAxisAngle ( Vector3 * vecAxis, float * fAngle )
    {
        *fAngle = acosf( w );
        float sinf_inv = (1.0f / sinf( *fAngle ));

        vecAxis->x = (x * sinf_inv);
        vecAxis->y = (y * sinf_inv);
        vecAxis->z = (z * sinf_inv);

        *fAngle *= 2;
    }

    Vector3 toEularAngles ( void ) const
    {
        float sqw = (w * w);
        float sqx = (x * x);
        float sqy = (y * y);
        float sqz = (z * z);

        Vector3 vecEular;
        vecEular.x = atan2f( 2.f * (x * y + z * w), sqx - sqy - sqz + sqw );
        vecEular.y = asinf( -2.f * (x * z - y * w) );
        vecEular.z = atan2f( 2.f * (y * z + x * w), -sqx - sqy + sqz + sqw );

        return vecEular;
    }

};
