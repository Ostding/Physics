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
        : id(0) {}
    virtual ~ForceGenerator(){};

  public:
    int id;
    /**
     * Overwrite this in implementations of the interface to calculate
     * and update the force applied to the given rigid body.
     */
    virtual void updateForce(RigidBody *body, const ffloat &deltaTime) = 0;
  };

}

#endif