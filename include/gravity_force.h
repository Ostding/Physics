#ifndef __GRAVITY_FORCE_H__
#define __GRAVITY_FORCE_H__

#include "force_generator.h"

namespace physics
{
  class GravityForce : public ForceGenerator
  {
    public:
      GravityForce(const Vector3 &gravity);
      virtual ~GravityForce(){}
      virtual void updateForce(RigidBody *body, const ffloat &deltaTime); 

    private:
      Vector3 gravity;
  };
}

#endif