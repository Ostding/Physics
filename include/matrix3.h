#ifndef __MATRIX3_H__
#define __MATRIX3_H__
#include "types.h"
#include "quaternion.h"

namespace physics
{

	class Matrix3
	{
		public:
			ffloat data[9];

		public:
			Matrix3()
			{
				data[0] = data[1] = data[2] = 
				data[3] = data[4] = data[5] =
        data[6] = data[7] = data[8] = ffzero;
			}

			Matrix3(const Matrix3 &other)
			{
				memcpy(&data, &other.data, 9 * sizeof(ffloat));
			}

			Matrix3(ffloat c0, ffloat c1, ffloat c2, 
							ffloat c3, ffloat c4, ffloat c5, 
            	ffloat c6, ffloat c7, ffloat c8)
      {
        data[0] = c0; data[1] = c1; data[2] = c2;
        data[3] = c3; data[4] = c4; data[5] = c5;
        data[6] = c6; data[7] = c7; data[8] = c8;
      }

			Matrix3(const Vector3 &A, const Vector3 &B, const Vector3 &C)
			{
				setComponents(A, B, C);
			}

			Matrix3 & operator = (const Matrix3 &other)
			{
				memcpy(&data, &other.data, 9 * sizeof(ffloat));
				return *this;
			}

			void setComponents(const Vector3 &A, const Vector3 &B, const Vector3 &C)
      {
        data[0] = A.x; data[1] = B.x; data[2] = C.x;
        data[3] = A.y; data[4] = B.y; data[5] = C.y;
        data[6] = A.z; data[7] = B.z; data[8] = C.z;
      }

      void setDiagonal(ffloat a, ffloat b, ffloat c)
      {
      	data[0] = a; 			data[1] = ffzero;	data[2] = ffzero;
      	data[3] = ffzero; data[4] = b; 			data[5] = ffzero;
      	data[6] = ffzero;	data[7] = ffzero;	data[8] = c;
      }

      void setSkewSymmetric(const Vector3 &v)
      {
      	data[0] = 0; 			data[1] = -v.z;	data[2] = v.y;
      	data[3] = v.z; 		data[4] = 0; 		data[5] = -v.x;
      	data[6] = -v.y;		data[7] = v.x;	data[8] = 0;
      }

      Vector3 operator * (const Vector3 &v) const
      {
      	return Vector3(
      			v.x * data[0] + v.y * data[1] + v.z * data[2],
            v.x * data[3] + v.y * data[4] + v.z * data[5],
            v.x * data[6] + v.y * data[7] + v.z * data[8]
      		);
      }

      Vector3 transform(const Vector3 &v)
      {
      	return (*this) * v;
      }

      Vector3 transposeTransform(const Vector3 &v)
      {
      	return Vector3(
      			v.x * data[0] + v.y * data[3] + v.z * data[6],
            v.x * data[1] + v.y * data[4] + v.z * data[7],
            v.x * data[2] + v.y * data[5] + v.z * data[8]
      		);
      }

      Vector3 getRowVector(int i) const
      {
          return Vector3(data[i*3], data[i*3+1], data[i*3+2]);
      }

      Vector3 getColumnVector(int i) const
      {
          return Vector3(data[i], data[i+3], data[i+6]);
      }

      void setInverse(const Matrix3 &m)
      {
        ffloat t4 	= m.data[0]*m.data[4];
        ffloat t6 	= m.data[0]*m.data[5];
        ffloat t8 	= m.data[1]*m.data[3];
        ffloat t10 	= m.data[2]*m.data[3];
        ffloat t12 	= m.data[1]*m.data[6];
        ffloat t14 	= m.data[2]*m.data[6];

        ffloat t16 = (t4*m.data[8] - t6*m.data[7] - t8*m.data[8] + t10*m.data[7] + t12*m.data[5] - t14*m.data[4]);
        if (t16 == ffzero) 
        	return;

        ffloat t17 = ffone / t16;

        data[0] = (m.data[4]*m.data[8]-m.data[5]*m.data[7])*t17;
        data[1] = -(m.data[1]*m.data[8]-m.data[2]*m.data[7])*t17;
        data[2] = (m.data[1]*m.data[5]-m.data[2]*m.data[4])*t17;
        data[3] = -(m.data[3]*m.data[8]-m.data[5]*m.data[6])*t17;
        data[4] = (m.data[0]*m.data[8]-t14)*t17;
        data[5] = -(t6-t10)*t17;
        data[6] = (m.data[3]*m.data[7]-m.data[4]*m.data[6])*t17;
        data[7] = -(m.data[0]*m.data[7]-t12)*t17;
        data[8] = (t4-t8)*t17;
      }

      Matrix3 inverse() const
      {
        Matrix3 result;
        result.setInverse(*this);
        return result;
      }

      void invert()
      {
        setInverse(*this);
      }

      void setTranspose(const Matrix3 &m)
      {
      	data[0] = m.data[0];	data[1] = m.data[3]; 	data[2] = m.data[6];
        data[3] = m.data[1]; 	data[4] = m.data[4]; 	data[5] = m.data[7];
        data[6] = m.data[2]; 	data[7] = m.data[5]; 	data[8] = m.data[8];
      }

      Matrix3 transpose() const
      {
      	Matrix3 result;
        result.setTranspose(*this);
        return result;
      }

      Matrix3 operator * (const Matrix3 &m) const
      {
        return Matrix3(
          data[0]*m.data[0] + data[1]*m.data[3] + data[2]*m.data[6],
          data[0]*m.data[1] + data[1]*m.data[4] + data[2]*m.data[7],
          data[0]*m.data[2] + data[1]*m.data[5] + data[2]*m.data[8],

          data[3]*m.data[0] + data[4]*m.data[3] + data[5]*m.data[6],
          data[3]*m.data[1] + data[4]*m.data[4] + data[5]*m.data[7],
          data[3]*m.data[2] + data[4]*m.data[5] + data[5]*m.data[8],

          data[6]*m.data[0] + data[7]*m.data[3] + data[8]*m.data[6],
          data[6]*m.data[1] + data[7]*m.data[4] + data[8]*m.data[7],
          data[6]*m.data[2] + data[7]*m.data[5] + data[8]*m.data[8]
		    );
      }

      void operator *= (const Matrix3 &m)
      {
        ffloat t1;
        ffloat t2;
        ffloat t3;

        t1 = data[0]*m.data[0] + data[1]*m.data[3] + data[2]*m.data[6];
        t2 = data[0]*m.data[1] + data[1]*m.data[4] + data[2]*m.data[7];
        t3 = data[0]*m.data[2] + data[1]*m.data[5] + data[2]*m.data[8];
        data[0] = t1;
        data[1] = t2;
        data[2] = t3;

        t1 = data[3]*m.data[0] + data[4]*m.data[3] + data[5]*m.data[6];
        t2 = data[3]*m.data[1] + data[4]*m.data[4] + data[5]*m.data[7];
        t3 = data[3]*m.data[2] + data[4]*m.data[5] + data[5]*m.data[8];
        data[3] = t1;
        data[4] = t2;
        data[5] = t3;

        t1 = data[6]*m.data[0] + data[7]*m.data[3] + data[8]*m.data[6];
        t2 = data[6]*m.data[1] + data[7]*m.data[4] + data[8]*m.data[7];
        t3 = data[6]*m.data[2] + data[7]*m.data[5] + data[8]*m.data[8];
        data[6] = t1;
        data[7] = t2;
        data[8] = t3;
	    }

	    void operator *= (const ffloat scale)
      {
  			data[0] *= scale; data[1] *= scale; data[2] *= scale;
  			data[3] *= scale; data[4] *= scale; data[5] *= scale;
  			data[6] *= scale; data[7] *= scale; data[8] *= scale;
  		}

  		void operator += (const Matrix3 &m)
      {
        data[0] += m.data[0]; data[1] += m.data[1]; data[2] += m.data[2];
        data[3] += m.data[3]; data[4] += m.data[4]; data[5] += m.data[5];
        data[6] += m.data[6]; data[7] += m.data[7]; data[8] += m.data[8];
      }

      void setOrientation(const Quaternion &q)
      {
        data[0] = ffone - (fftwo * q.j*q.j + fftwo * q.k*q.k);
        data[1] = fftwo * q.i*q.j + fftwo * q.k*q.r;
        data[2] = fftwo * q.i*q.k - fftwo * q.j*q.r;

        data[3] = fftwo * q.i*q.j - fftwo * q.k*q.r;
        data[4] = ffone - (fftwo * q.i*q.i  + fftwo * q.k*q.k);
        data[5] = fftwo * q.j*q.k + fftwo * q.i*q.r;

        data[6] = fftwo * q.i*q.k + fftwo * q.j*q.r;
        data[7] = fftwo * q.j*q.k - fftwo * q.i*q.r;
        data[8] = ffone - (fftwo * q.i*q.i  + fftwo * q.j*q.j);
      }

      Matrix3 linearInterpolate(const Matrix3& a, const Matrix3& b, ffloat prop)
			{
				Matrix3 result;
				for (unsigned i = 0; i < 9; i++) 
				{
					result.data[i] = (a.data[i] * (ffone - prop)) + (b.data[i] * prop);
				}
				return result;
			}

      void inspect(const char *pszTag = "") const
      {
        printf("[%s] Matrix3: \r\n %.5f %.5f %.5f \r\n %.5f %.5f %.5f \r\n %.5f %.5f %.5f \r\n", pszTag, 
                data[0].to_d(), data[1].to_d(), data[2].to_d(), data[3].to_d(), data[4].to_d(), data[5].to_d(), data[6].to_d(), data[7].to_d(), data[8].to_d());
      }
	};

}


#endif