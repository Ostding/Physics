#ifndef __BODY_H__
#define __BODY_H__

#include "types.h"

namespace physics
{
  class RigidBody
  {
  public:
    //Damp for liner velocity
    ffloat  linerDamp;
    ffloat  powAngularDamp;
    ffloat  powLinerDamp;   

    //Damp for angular velocity
    ffloat  angularDamp;

    //Anisotropic friction
    ffloat  friction;

    //Hold inverse of mass, for divide optimize 
    ffloat  inverseMass;

    //Inertia tensor's inverse
    Matrix3 inverseInertiaTensor;

    //Body's direction
    Quaternion orientation;

    //Position in world space
    Vector3 position;

    //Liner velocity
    Vector3 velocity;

    //Angular velocity
    Vector3 rotation;

    //This matrix used for converting between body space and world space
    Matrix4 transformMatrix;

    Vector3 forceAcc;

    Vector3 torqueAcc;


    //This is recency-weighted average, used to put a body to sleep 
    float   motion;
    bool    canSleep;

  };
}

#endif