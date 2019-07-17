#include "body.h"

namespace physics
{

  ffloat RigidBody::sleepEpsilon = ffloat(30000LL); //0.3f

  static inline void updateTransformMatrix( Matrix4 &transform,
                                            const Vector3 &position,
                                            const Quaternion &orientation)
  {
    transform.data[0] = ffone - fftwo * orientation.j*orientation.j-fftwo * orientation.k*orientation.k;
    transform.data[1] = fftwo * orientation.i*orientation.j - fftwo * orientation.r*orientation.k;
    transform.data[2] = fftwo * orientation.i*orientation.k + fftwo * orientation.r*orientation.j;
    transform.data[3] = position.x;

    transform.data[4] = fftwo * orientation.i*orientation.j + fftwo * orientation.r*orientation.k;
    transform.data[5] = ffone - fftwo * orientation.i*orientation.i - fftwo * orientation.k*orientation.k;
    transform.data[6] = fftwo * orientation.j*orientation.k - fftwo * orientation.r*orientation.i;
    transform.data[7] = position.y;

    transform.data[8] = fftwo * orientation.i*orientation.k - fftwo * orientation.r*orientation.j;
    transform.data[9] = fftwo * orientation.j*orientation.k + fftwo * orientation.r*orientation.i;
    transform.data[10] = ffone - fftwo * orientation.i*orientation.i - fftwo * orientation.j*orientation.j;
    transform.data[11] = position.z;
  }

  static inline void updateInertiaTensor(Matrix3 &matWorld, const Quaternion &q,
                                           const Matrix3 &matBody, const Matrix4 &rotmat)
  {
    ffloat t4 = rotmat.data[0]*matBody.data[0]+
        rotmat.data[1]*matBody.data[3]+
        rotmat.data[2]*matBody.data[6];
    ffloat t9 = rotmat.data[0]*matBody.data[1]+
        rotmat.data[1]*matBody.data[4]+
        rotmat.data[2]*matBody.data[7];
    ffloat t14 = rotmat.data[0]*matBody.data[2]+
        rotmat.data[1]*matBody.data[5]+
        rotmat.data[2]*matBody.data[8];
    ffloat t28 = rotmat.data[4]*matBody.data[0]+
        rotmat.data[5]*matBody.data[3]+
        rotmat.data[6]*matBody.data[6];
    ffloat t33 = rotmat.data[4]*matBody.data[1]+
        rotmat.data[5]*matBody.data[4]+
        rotmat.data[6]*matBody.data[7];
    ffloat t38 = rotmat.data[4]*matBody.data[2]+
        rotmat.data[5]*matBody.data[5]+
        rotmat.data[6]*matBody.data[8];
    ffloat t52 = rotmat.data[8]*matBody.data[0]+
        rotmat.data[9]*matBody.data[3]+
        rotmat.data[10]*matBody.data[6];
    ffloat t57 = rotmat.data[8]*matBody.data[1]+
        rotmat.data[9]*matBody.data[4]+
        rotmat.data[10]*matBody.data[7];
    ffloat t62 = rotmat.data[8]*matBody.data[2]+
        rotmat.data[9]*matBody.data[5]+
        rotmat.data[10]*matBody.data[8];

    matWorld.data[0] = t4*rotmat.data[0]+
        t9*rotmat.data[1]+
        t14*rotmat.data[2];
    matWorld.data[1] = t4*rotmat.data[4]+
        t9*rotmat.data[5]+
        t14*rotmat.data[6];
    matWorld.data[2] = t4*rotmat.data[8]+
        t9*rotmat.data[9]+
        t14*rotmat.data[10];
    matWorld.data[3] = t28*rotmat.data[0]+
        t33*rotmat.data[1]+
        t38*rotmat.data[2];
    matWorld.data[4] = t28*rotmat.data[4]+
        t33*rotmat.data[5]+
        t38*rotmat.data[6];
    matWorld.data[5] = t28*rotmat.data[8]+
        t33*rotmat.data[9]+
        t38*rotmat.data[10];
    matWorld.data[6] = t52*rotmat.data[0]+
        t57*rotmat.data[1]+
        t62*rotmat.data[2];
    matWorld.data[7] = t52*rotmat.data[4]+
        t57*rotmat.data[5]+
        t62*rotmat.data[6];
    matWorld.data[8] = t52*rotmat.data[8]+
        t57*rotmat.data[9]+
        t62*rotmat.data[10];
  }

  RigidBody::RigidBody(Primitive *primitive)
    : linearDamp( ffzero )
    , powLinerDamp( ffzero )
    , angularDamp( ffzero )
    , powAngularDamp( ffzero )
    , friction( ffzero )
    , inverseMass( ffzero )
    , motion( ffzero )
    , canSleep( true )
    , primitive( primitive )
    {}

   void RigidBody::clearAccumulators()
   {
    forceAccumulator.clear();
    torqueAccumulator.clear();
   }
   
}