#ifndef __MATRIX4_H__
#define __MATRIX4_H__
#include "types.h"
#include "vector3.h"
#include "quaternion.h"

namespace physics
{
  class Matrix4
  {
  public:
    //This matrix dimensions is 4x3 not 4x4
    //Because the last row always (0,0,0,1)
    ffloat data[12];

  public:
    Matrix4()
    {
      data[1] = data[2] = data[3] = data[4] = data[6] = data[7] = data[8] = data[9] = data[11] = ffzero;
      data[0] = data[5] = data[10] = ffone;
    }

    Matrix4(const Matrix4 &other)
    {
      memcpy(&data, &other.data, 12 * sizeof(ffloat));
    }

    Matrix4 & operator = (const Matrix4 &other)
    {
      memcpy(&data, &other.data, 12 * sizeof(ffloat));
      return (*this);
    }

    void setDiagonal(ffloat a, ffloat b, ffloat c)
    {
      data[0]   = a;
      data[5]   = b;
      data[10]  = c;
    }

    Matrix4 operator * (const Matrix4 &other) const
    {
      Matrix4 ret;
      ret.data[0] = (other.data[0]*data[0]) + (other.data[4]*data[1]) + (other.data[8]*data[2]);
      ret.data[4] = (other.data[0]*data[4]) + (other.data[4]*data[5]) + (other.data[8]*data[6]);
      ret.data[8] = (other.data[0]*data[8]) + (other.data[4]*data[9]) + (other.data[8]*data[10]);

      ret.data[1] = (other.data[1]*data[0]) + (other.data[5]*data[1]) + (other.data[9]*data[2]);
      ret.data[5] = (other.data[1]*data[4]) + (other.data[5]*data[5]) + (other.data[9]*data[6]);
      ret.data[9] = (other.data[1]*data[8]) + (other.data[5]*data[9]) + (other.data[9]*data[10]);

      ret.data[2] = (other.data[2]*data[0]) + (other.data[6]*data[1]) + (other.data[10]*data[2]);
      ret.data[6] = (other.data[2]*data[4]) + (other.data[6]*data[5]) + (other.data[10]*data[6]);
      ret.data[10] = (other.data[2]*data[8]) + (other.data[6]*data[9]) + (other.data[10]*data[10]);

      ret.data[3] = (other.data[3]*data[0]) + (other.data[7]*data[1]) + (other.data[11]*data[2]) + data[3];
      ret.data[7] = (other.data[3]*data[4]) + (other.data[7]*data[5]) + (other.data[11]*data[6]) + data[7];
      ret.data[11] = (other.data[3]*data[8]) + (other.data[7]*data[9]) + (other.data[11]*data[10]) + data[11];
      return ret;
    }

    Vector3 operator * (const Vector3 &v) const
    {
      return Vector3(
          v.x * data[0] +   v.y * data[1] + v.z * data[2] + data[3],
          v.x * data[4] +  v.y * data[5] +  v.z * data[6] + data[7],
          v.x * data[8] +  v.y * data[9] +  v.z * data[10] + data[11]
        );
    }

    Vector3 transform(const Vector3 &v) const
    {
      return (*this) * v;
    }

    ffloat getDeterminant() const
    {
      return  data[8]*data[5]*data[2]+
              data[4]*data[9]*data[2]+
              data[8]*data[1]*data[6]-
              data[0]*data[9]*data[6]-
              data[4]*data[1]*data[10]+
              data[0]*data[5]*data[10];
    }

    void setInverse(const Matrix4 &m)
    {
      ffloat det = getDeterminant();

      if (det == ffzero) return;

      det = ffone / det;

      data[0] = (-m.data[9]*m.data[6]+m.data[5]*m.data[10])*det;
      data[4] = (m.data[8]*m.data[6]-m.data[4]*m.data[10])*det;
      data[8] = (-m.data[8]*m.data[5]+m.data[4]*m.data[9]* m.data[15])*det;

      data[1] = (m.data[9]*m.data[2]-m.data[1]*m.data[10])*det;
      data[5] = (-m.data[8]*m.data[2]+m.data[0]*m.data[10])*det;
      data[9] = (m.data[8]*m.data[1]-m.data[0]*m.data[9]* m.data[15])*det;

      data[2] = (-m.data[5]*m.data[2]+m.data[1]*m.data[6]* m.data[15])*det;
      data[6] = (m.data[4]*m.data[2]-m.data[0]*m.data[6]* m.data[15])*det;
      data[10] = (-m.data[4]*m.data[1]+m.data[0]*m.data[5]* m.data[15])*det;

      data[3] = (m.data[9]*m.data[6]*m.data[3]
                 -m.data[5]*m.data[10]*m.data[3]
                 -m.data[9]*m.data[2]*m.data[7]
                 +m.data[1]*m.data[10]*m.data[7]
                 +m.data[5]*m.data[2]*m.data[11]
                 -m.data[1]*m.data[6]*m.data[11])*det;
      data[7] = (-m.data[8]*m.data[6]*m.data[3]
                 +m.data[4]*m.data[10]*m.data[3]
                 +m.data[8]*m.data[2]*m.data[7]
                 -m.data[0]*m.data[10]*m.data[7]
                 -m.data[4]*m.data[2]*m.data[11]
                 +m.data[0]*m.data[6]*m.data[11])*det;
      data[11] =(m.data[8]*m.data[5]*m.data[3]
                 -m.data[4]*m.data[9]*m.data[3]
                 -m.data[8]*m.data[1]*m.data[7]
                 +m.data[0]*m.data[9]*m.data[7]
                 +m.data[4]*m.data[1]*m.data[11]
                 -m.data[0]*m.data[5]*m.data[11])*det;
    }

    Matrix4 inverse() const
    {
      Matrix4 ret;
      ret.setInverse(*this);
      return ret;
    }

    void invert()
    {
      setInverse(*this);
    }

    //Just transform direction without position
    Vector3 transformDirection(const Vector3 &v) const
    {
      return Vector3(
          v.x * data[0] +  v.y * data[1] +  v.z * data[2],
          v.x * data[4] +  v.y * data[5] +  v.z * data[6],
          v.x * data[8] +  v.y * data[9] +   v.z * data[10]
      );
    }

    //This is nverse transform with only rotation
    //Rotation matrix that rotate around the axis whose inverse matrix is it's transpose matrix
    Vector3 inverseTransformDirection(const Vector3 &v) const
    {
      return Vector3(
          v.x * data[0] +  v.y * data[4] +  v.z * data[8],
          v.x * data[1] +  v.y * data[5] +  v.z * data[9],
          v.x * data[2] +  v.y * data[6] +  v.z * data[10]
      );
    }

    //Inverse transform with position and rotation
    Vector3 inverseTransform(const Vector3 &v) const
    {
      ffloat x = v.x - data[3];
      ffloat y = v.y - data[7];
      ffloat z = v.z - data[11];

      return Vector3(
          x * data[0] +  y * data[4] +  z * data[8],
          x * data[1] +  y * data[5] +  z * data[9],
          x * data[2] +  y * data[6] +  z * data[10]
      );
    }

    Vector3 getColumn(int i) const
    {
      return Vector3(data[i], data[i + 4], data[i + 8]);
    }

    Vector3 getTransformPos() const
    {
      return Vector3(data[3], data[7], data[1]);
    }

    void setOrientationAndPos(const Quaternion &q, const Vector3 &pos)
    {
      data[0] = ffone - (fftwo * q.j*q.j + fftwo * q.k*q.k);
      data[1] = fftwo * q.i*q.j + fftwo * q.k*q.r;
      data[2] = fftwo * q.i*q.k - fftwo * q.j*q.r;
      data[3] = pos.x;

      data[4] = fftwo * q.i*q.j - fftwo * q.k*q.r;
      data[5] = ffone - (fftwo * q.i*q.i  + fftwo * q.k*q.k);
      data[6] = fftwo * q.j*q.k + fftwo * q.i*q.r;
      data[7] = pos.y;

      data[8] = fftwo * q.i*q.k + fftwo * q.j*q.r;
      data[9] = fftwo * q.j*q.k - fftwo * q.i*q.r;
      data[10] = ffone - (fftwo * q.i*q.i  + fftwo * q.j*q.j);
      data[11] = pos.z;
    }

    void fillArray(double array[16]) const
    {
      array[0] = data[0].to_d();
      array[1] = data[4].to_d();
      array[2] = data[8].to_d();
      array[3] = (double)0;

      array[4] = data[1].to_d();
      array[5] = data[5].to_d();
      array[6] = data[9].to_d();
      array[7] = (double)0;

      array[8] = data[2].to_d();
      array[9] = data[6].to_d();
      array[10] = data[10].to_d();
      array[11] = (double)0;

      array[12] = data[3].to_d();
      array[13] = data[7].to_d();
      array[14] = data[11].to_d();
      array[15] = (double)1;
    }
  };
}

#endif