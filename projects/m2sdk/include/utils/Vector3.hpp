#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#include <limits>

#ifdef max
#undef max
#endif

template<class T>
static T Clamp_( const T& min, const T& a, const T& max )
{
    if( a < min )
        return min;

    if( a > max )
        return max;

    return a;
}

class Vector2
{

public:

    float x, y;

    Vector2()
    {
        x = y = 0.0f;
    }

    Vector2(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

};

class Vector3
{

public:

    float x;
    float y;
    float z;

    Vector3()
    {
        x = y = z = 0.0f;
    }

    Vector3(float _x, float _y, float _z)
    {
        x = _x; y = _y; z = _z;
    }

    bool IsEmpty() const
    {
        return (x == 0 && y == 0 && z == 0);
    }

    float Length() const
    {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    void Normalize( void )
    {
        float fLength = (1.0f / Length());
        x *= fLength;
        y *= fLength;
        z *= fLength;
    }

    void Clamp( float fMin, float fMax )
    {
        x = Clamp_< const float >( fMin, x, fMax );
        y = Clamp_< const float >( fMin, y, fMax );
        z = Clamp_< const float >( fMin, z, fMax );
    }

    void ToRadians( void )
    {
        x = (x * (M_PI / 180.0f));
        y = (y * (M_PI / 180.0f));
        z = (z * (M_PI / 180.0f));
    }

    void FromRadians( void )
    {
        x = (x * (180.0f / M_PI));
        y = (y * (180.0f / M_PI));
        z = (z * (180.0f / M_PI));
    }

    Vector3 operator+ (const Vector3& vecRight) const
    {
        return Vector3(x + vecRight.x, y + vecRight.y, z + vecRight.z);
    }

    Vector3 operator+ (float fRight) const
    {
        return Vector3(x + fRight, y + fRight, z + fRight);
    }

    Vector3 operator- (const Vector3& vecRight) const
    {
        return Vector3(x - vecRight.x, y - vecRight.y, z - vecRight.z);
    }

    Vector3 operator- (float fRight) const
    {
        return Vector3(x - fRight, y - fRight, z - fRight);
    }

    Vector3 operator* (const Vector3& vecRight) const
    {
        return Vector3(x * vecRight.x, y * vecRight.y, z * vecRight.z);
    }

    Vector3 operator* (float fRight) const
    {
        return Vector3(x * fRight, y * fRight, z * fRight);
    }

    Vector3 operator/ (const Vector3& vecRight) const
    {
        return Vector3(x / vecRight.x, y / vecRight.y, z / vecRight.z);
    }

    Vector3 operator/ (float fRight) const
    {
        return Vector3(x / fRight, y / fRight, z / fRight);
    }

    Vector3 operator - () const
    {
        return Vector3(-x, -y, -z);
    }

    void operator += (float fRight)
    {
        x += fRight;
        y += fRight;
        z += fRight;
    }

    void operator += (const Vector3 vector)
    {
        x += vector.x;
        y += vector.y;
        z += vector.z;
    }

    void operator -= (float fRight)
    {
        x -= fRight;
        y -= fRight;
        z -= fRight;
    }

    void operator -= (const Vector3 vector)
    {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
    }

    void operator *= (float fRight)
    {
        x *= fRight;
        y *= fRight;
        z *= fRight;
    }

    void operator *= (const Vector3 vector)
    {
        x *= vector.x;
        y *= vector.y;
        z *= vector.z;
    }

    void operator /= (float fRight)
    {
        x /= fRight;
        y /= fRight;
        z /= fRight;
    }

    void operator /= (const Vector3 vector)
    {
        x /= vector.x;
        y /= vector.y;
        z /= vector.z;
    }

    bool operator != (const Vector3& vecVector) const
    {
        return (x != vecVector.x && y != vecVector.y && z != vecVector.z);
    }
    bool operator == (const Vector3& vecVector) const
    {
        return (x == vecVector.x && y == vecVector.y && z == vecVector.z);
    }
};
