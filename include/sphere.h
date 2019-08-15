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
      Sphere(const ffloat &radius);

      virtual void refreshAABB();
      virtual void render();

      void initInertiaTensor(const ffloat &mass);
  };
}

#endif