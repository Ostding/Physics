#ifndef __FROCEGENERATOR_H__
#define __FROCEGENERATOR_H__

#include "types.h"
#include "body.h"

namespace physics
{
  class ForceGenerator
  {
  public:
    ForceGenerator()
    :id(0) {}

  public:
    int id;
    /**
     * Overload this in implementations of the interface to calculate
     * and update the force applied to the given rigid body.
     */
    virtual void updateForce(RigidBody *body, ffloat deltaTime) = 0;
  };

}

#endif