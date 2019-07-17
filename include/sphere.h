#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "types.h"
#include "primitive.h"

namespace physics
{
  class Sphere : public Primitive
  {
    public:
      ffloat radius;
      ffloat squareRadius;

    public:
      Sphere();
      Sphere(ffloat radius);

      virtual void refreshAABB();
  };
}

#endif