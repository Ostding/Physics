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
    Vector3 extents;
    ffloat offset;

  public:
    Plane();
    Plane(const Vector3 &direction, const Vector3 &extents, const ffloat &offset);

    virtual void refreshAABB();
    virtual void render();
  };
}


#endif