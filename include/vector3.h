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

    Vector3 operator + (const Vector3 &other)
    {
      return Vector3(x + other.x, y + other.y, z + other.z);
    }

    void operator += (const Vector3 &other)
    {
      x += other.x;
      y += other.y;
      z += other.z;
    }

    Vector3 operator - (const Vector3 &other)
    {
      return Vector3(x - other.x, y - other.y, z - other.z);
    }

    void operator -= (const Vector3 &other)
    {
      x -= other.x;
      y -= other.y;
      z -= other.z;
    }

    Vector3 scale (const ffloat &scale)
    {
      return Vector3(x * scale, y * scale, z * scale);
    }

    Vector3 scaleUpdate(const ffloat &scale)
    {
      x *= scale;
      y *= scale;
      z *= scale;
    }

    //component-wise product with other vector
    void product(const Vector3 *other)
    {
      return Vector3(x * other.x, y * other.y, z * other.z);
    }
    
    void productUpdate(const Vector3 *other)
    {
      x *= other.x;
      y *= other.y;
      z *= other.z;
    }

    ffloat dot(const Vector3 &other)
    {
      return x*other.x + y*other.y + z*other.z;
    }


    Vector3 cross(const Vector3 &other)
    {
      return Vector3(y*other.z-z*other.y, 
                     z*other.x-x*other.z, 
                     x*other.y-y*other.x);
    }

    void crossUpdate(const Vector3 &other)
    {
      x = y*other.z-z*other.y;
      y = z*other.x-x*other.z;
      z = x*other.y-y*other.x;
    }

    //Performs cross-product A x B x C = B x (C . A) - A x (C . B)
    static Vector3 tripleCross(const Vector3 &A, const Vector3 &B, const Vector3 &C)
    {
      return (B.scale(C.dot(A))) - (A.scale(C.dot(B)))
    }

    void addScaleVector(const Vector3 &other, ffloat scale)
    {
      x += other.x * scale;
      y += other.y * scale;
      z += other.z * scale;
    }

    ffloat mag() const
    {
      return ffsqrt(x*x + y*y + z*z);
    }

    ffloat sqearMag() const
    {
      return x*x + y*y + z*z;
    }
  }
}
}

#endif