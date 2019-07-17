#ifndef __PLANE_H__
#define __PLANE_H__

#include "types.h"
#include "primitive.h"

namespace physics
{
  class Plane : public Primitive
  {
  public:
    Vector3 direction;
    Vector3 center;
    Vector3 extents;
  
  public:
    Plane();
    Plane(Vector3 direction, Vector3 center, Vector3 extents);

    virtual void refreshAABB();
  };
}


#endif