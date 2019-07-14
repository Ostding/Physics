#ifndef __VECTOR3_H__
#define __VECTOR3_H__

namespace physics
{
  class Vector3
  {
  public:
    ffloat x;
    ffloat y;
    ffloat z;

  public:
    Vector3()
    : x(0), y(0), z(0)
    {}

    Vector3(ffloat _x, ffloat _y, ffloat _z)
    : x(_x), y(_y), z(_z)
    {}

    Vector3(const Vector3 &other)
    {
      x = other.x;
      y = other.y;
      z = other.z;
    }

    void set(ffloat _x, ffloat _y, ffloat _z)
    {
      x = _x;
      y = _y;
      z = _z;
    }

    const static Vector3 UP;
    const static Vector3 RIGHT;
    const static Vector3 X;
    const static Vector3 Y;
    const static Vector3 Z;
    const static Vector3 ZERO;
    const static Vector3 ONE;

    Vector3 & operator = (const Vector3 &other)
    {
      x = other.x;
      y = other.y;
      z = other.z;
      return *this;
    }

    Vector3 & operator + (const Vector3 &other)
    {
      return Vector3(x + other.x, y + other.y, z + other.z);
    }

    void operator += (const Vector3 &other)
    {
      x += other.x;
      y += other.y;
      z += other.z;
    }

    Vector3 & operator - (const Vector3 &other)
    {
      return Vector3(x - other.x, y - other.y, z - other.z);
    }

    void operator -= (const Vector3 &other)
    {
      x -= other.x;
      y -= other.y;
      z -= other.z;
    }

    Vector3 & operator * (const ffloat &scale)
    {
      return Vector3(x * scale, y * scale, z * scale);
    }

    void operator *= (const ffloat &scale)
    {
      x *= scale;
      y *= scale;
      z *= scale;
    }

    
  }
}

#endif