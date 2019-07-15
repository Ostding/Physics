#ifndef __QUATERNION_H__
#define __QUATERNION_H__
#include "types.h"
#include "vector3.h"

namespace physics
{

  class Quaternion
  {
    public:
      ffloat r;
      ffloat i;
      ffloat j;
      ffloat k;


    public:
      Quaternion() 
      : r(0), i(0), j(0), k(0)
      {}

      Quaternion(const ffloat _r, const ffloat _i, const ffloat _j, const ffloat _k)
      :r(_r), i(_i), j(_j), k(_k)
      {}

      Quaternion(const Quaternion &other)
      {
        r = other.r;
        i = other.i;
        j = other.j;
        k = other.k;
      }

    public:
      const static Quaternion zero;

      Quaternion &operator = (const Quaternion &other)
      {
        r = other.r;
        i = other.i;
        j = other.j;
        k = other.k;
        return *this;
      }

      bool operator == (const Quaternion & other)
      {
        return ((r == other.r) && (i == other.i) &&
                (j == other.j) && (k == other.k));
      }

      bool operator != (const Quaternion &other)
      {
        return ((r != other.r) && (i != other.i) &&
                (j != other.j) && (k != other.k));
      }

      void set(const ffloat _r, const ffloat _i, const ffloat _j, const ffloat _k)
      {
        r = _r;
        i = _i;
        j = _j;
        k = _k;
      }

      static Quaternion product(const Quaternion &A, const Quaternion &B)
      {
        ffloat r = A.r * B.r - A.i * B.i -  A.j * B.j - A.k * B.k;
        ffloat i = A.r * B.i + A.i * B.r +  A.j * B.k - A.k * B.j;
        ffloat j = A.r * B.j + A.j * B.r +  A.k * B.i - A.i * B.k;
        ffloat k = A.r * B.k + A.k * B.r +   A.i * B.j - A.j * B.i;
        return Quaternion(r, i, j, k);
      }

      void operator *= (const Quaternion &other)
      {
        *this =  product(*this, other);
      }

      Quaternion operator * (const Quaternion &other)
      {
        return product(*this, other);
      }

      void normalise()
      {
        ffloat mag = r*r + i*i + j*j + k*k;
        if (mag == ffzero)
        {
          r = ffone;
          return;
        }

        ffloat den = ffsqrt(mag);
        if (den == ffzero)
        {
          r = ffone;
          return;
        }

        ffloat f = mag / den;
        r *= f;
        i *= f;
        j *= f;
        k *= f;
      }

      void addScaledVector(const Vector3 &v, ffloat scale)
      {
        Quaternion q(ffzero, v.x * scale, v.y * scale,  v.z * scale);
        q *= *this;

        r += q.r * ffhalf;
        i += q.i * ffhalf;
        j += q.j * ffhalf;
        k += q.k * ffhalf;
      }

      void rotateByVector(const Vector3 &v)
      {
         Quaternion q(ffzero, v.x, v.y, v.z);
         (*this) *= q;
      }

      Vector3 toEulerAngles()
      {
        // ffloat halfRad = ffacos(r);
        // ffloat sv = ffsin(halfRad);
        // ffloat iv = ffone / sv;
        // ffloat xRad = ffacos(i * iv);
        // ffloat yRad = ffacos(j * iv);
        // ffloat zRad = ffacos(j * iv);
        // return Vector3(fftodeg(xRad), fftodeg(yRad), fftodeg(zRad));

        ffloat sinr_cosp = fftwo * (r * i + j * k);
        ffloat cosr_cosp = ffone - fftwo * (i * i + j * j);
        ffloat x = ffatan2(sinr_cosp, cosr_cosp);

        ffloat y;
        ffloat sinp = fftwo * (r * j - k * i);
        // use 90 degrees if out of range
        if (ffabs(sinp) >= ffone)
        {
          if(sinp > 0)
            y = ffhalf_pi;
          else
            y = -ffhalf_pi;
        }
        else
        {
            y = ffasin(sinp);
        }

        ffloat siny_cosp = fftwo * (r * k + i * j);
        ffloat cosy_cosp = ffone - fftwo * (j * j + k * k);  
        ffloat z = ffatan2(siny_cosp, cosy_cosp);
        return Vector3(fftodeg(x), fftodeg(y), fftodeg(z));
      }

      void toAxisAngle(Vector3 &axis, ffloat &angle)
      {
        axis.set(i, j, k);
        axis.normalize();
        angle = fftwo * ffacos(r);
        angle = fftodeg(angle);
      }

      ffloat getAngle()
      {
        return fftodeg(fftwo * ffacos(r));
      }

      static Quaternion fromEulerAngles(const Vector3 &v)
      {
        ffloat radX = fftorad(v.x * ffhalf);
        ffloat radY = fftorad(v.y * ffhalf);
        ffloat radZ = fftorad(v.z * ffhalf);

        ffloat cr = ffcos(radX);
        ffloat sr = ffsin(radX);
        ffloat cy = ffcos(radZ);
        ffloat sy = ffsin(radZ);
        ffloat cp = ffcos(radY);
        ffloat sp = ffsin(radY);

        ffloat r = cy * cp * cr + sy * sp * sr;
        ffloat i = cy * cp * sr - sy * sp * cr;
        ffloat j = sy * cp * sr + cy * sp * cr;
        ffloat k = sy * cp * cr - cy * sp * sr;

        return Quaternion(r, i, j, k);
      }

      static Quaternion fromAxisAngle(const Vector3 &axis, const ffloat angle)
      {
        ffloat rad = fftorad(angle);
        ffloat half = rad * ffhalf;
        ffloat s = ffsin(half);
        ffloat r = ffcos(half);
        ffloat i = axis.x * s;
        ffloat j = axis.y * s;
        ffloat k = axis.z * s;
        return Quaternion(r, i, j, k);
      }

      Vector3 operator*(const Vector3 &v)
      {
        Quaternion qv;
        qv.r = ffzero;
        qv.i = v.x;
        qv.j = v.y;
        qv.k = v.z;

        Quaternion qConj;
        qConj.r = r;
        qConj.i = -i;
        qConj.j = -j;
        qConj.k = -k;

        qv = (*this) * (qv);
        qv *= qConj;

        return Vector3(qv.i, qv.j, qv.k);
      }

      void inspect(const char *pszTag = "") const
      {
        printf("[%s] Quaternion: %.5f %.5f %.5f %.5f\r\n", pszTag, r.to_d(), i.to_d(), j.to_d(), k.to_d());
      }
  };

}

#endif