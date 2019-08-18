#ifndef __MOVE_FORCE_H__
#define __MOVE_FORCE_H__

#include "force_generator.h"

namespace physics
{
  class MoveForce : public ForceGenerator
  {
    public:
      MoveForce(const Vector3 &acc, const Vector3 &point);
      virtual ~MoveForce(){}
      virtual void updateForce(RigidBody *body, const ffloat &deltaTime);

    public:
      void setForceAcceleration(const Vector3 &acc);
      void setForcePoint(const Vector3 &point);

    private:
      //force acceleration
      Vector3 moveAcc;

      //Force work point on rigidbody
      //This is body space position releative to body position
      Vector3 point;  
  };
}

#endif