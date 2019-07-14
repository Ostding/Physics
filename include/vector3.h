#ifndef __VECTOR3_H__
#define __VECTOR3_H__
#include "types.h"

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

    bool operator == (const Vector3& other) const
    {
      return  x == other.x && y == other.y && z == other.z;
    }

    bool operator != (const Vector3& other) const
    {
      return !(*this == other);
    }

    bool operator < (const Vector3& other) const
    {
      return x < other.x && y < other.y && z < other.z;
    }

    bool operator > (const Vector3& other) const
    {
      return x > other.x && y > other.y && z > other.z;
    }

    bool operator <= (const Vector3& other) const
    {
      return x <= other.x && y <= other.y && z <= other.z;
    }

    bool operator >= (const Vector3& other) const
    {
      return x >= other.x && y >= other.y && z >= other.z;
    }

    Vector3 operator - ()
    {
      return Vector3(-x, -y, -z);
    }

    Vector3 scale (const ffloat scale) const
    {
      return Vector3(x * scale, y * scale, z * scale);
    }

    void scaleUpdate(const ffloat &scale)
    {
      x *= scale;
      y *= scale;
      z *= scale;
    }

    static ffloat AngleTo(const Vector3 &A, const Vector3 &B)
    {
      ffloat cosv;
      ffloat ab = dot(A, B);
      ffloat a = A.mag();
      ffloat b = B.mag();
      ffloat mab = a * b;
      if (mab == ffzero)
      {
        cosv = ffzero;
      }
      else
      {
        ffloat cosv = ab/ mab;
        if (cosv < -ffone)
          cosv = -ffone;
        else if (cosv > ffone)
          cosv = ffone;
      }

      return ffacos(cosv);
    }

    static Vector3 product(const Vector3 &A, const Vector3 &B)
    {
      return Vector3(A.x * B.x, A.y * B.y, A.z * B.z);
    }

    static ffloat dot(const Vector3 &A, const Vector3 &B)
    {
      return A.x*B.x + A.y*B.y + A.z*B.z;
    }

    static Vector3 cross(const Vector3 &A, const Vector3 &B)
    {
      return Vector3(A.y*B.z - A.z*B.y,  A.z*B.x - A.x*B.z, A.x*B.y - A.y*B.x);
    }

    //Performs cross-product A x B x C = B x (C . A) - A x (C . B)
    static Vector3 tripleCross(const Vector3 &A, const Vector3 &B, const Vector3 &C)
    {
      return (B.scale(C.dot(A))) - (A.scale(C.dot(B)));
    }

    //component-wise product with other vector
    Vector3 product(const Vector3 &other) const
    {
      return product(*this, other);
    }
    
    void productUpdate(const Vector3 &other)
    {
      *this = product(*this, other);
    }

    ffloat dot(const Vector3 &other) const
    {
      return dot(*this, other);
    }

    Vector3 cross(const Vector3 &other)
    {
      return cross(*this, other);
    }

    void crossUpdate(const Vector3 &other)
    {
      *this = cross(*this, other);
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

    ffloat squareMag() const
    {
      return x*x + y*y + z*z;
    }

    void clear()
    {
      x = y = z = 0;
    }

    void normalize()
    {
      ffloat l = mag();
      if (l > ffzero)
        this->scale(ffone / l);
    }

    void inspect()
    {
      printf("Vector3: %.5f %.5f %.5f\r\n", x.to_d(), y.to_d(), z.to_d());
    }
  };

}

#endif