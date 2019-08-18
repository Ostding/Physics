#ifndef __SPRING_FORCE_H__
#define __SPRING_FORCE_H__

#include "force_generator.h"

namespace physics
{
  class SpringForce : public ForceGenerator
  {
    public:
      SpringForce(const Vector3 &connectPoint, RigidBody *ohter, const Vector3 &otherConnectPoint,
                  const ffloat &ceofficient, const ffloat &length, const ffloat &maxForce);
      virtual void updateForce(RigidBody *body, const ffloat &duration);
    private:
      //Connect point on self in local space
      Vector3 connectPoint;

      RigidBody *other;
      //Connect point on other in self local space
      Vector3 otherConnectPoint;

      //Spring ceofficient
      ffloat ceofficient;
      //Hold the max foce can added
      ffloat maxForce;
      //Hold spring's length, force direction will reverse while connect points's distance less than length
      ffloat length;
  };
}

#endif