#ifndef __CAPSULE_H__
#define __CAPSULE_H__

#include "types.h"
#include "primitive.h"
#include "body.h"
#include "vector3.h"

namespace physics
{
  class Capsule : public Primitive
  {
    public:
      Capsule();
      Capsule(RigidBody *body);
      Capsule(const Vector3 &pointA, const Vector3 &pointB, ffloat radius);

    public:
      ffloat radius;
      Vector3 pointA;
      Vector3 pointB;

    public:
      virtual void refreshAABB();
      virtual void render();
      virtual void findFarthestPointInDirection(const Vector3 &direction, Vector3 &pointLocal, Vector3 &pointWorld);

      void setPosition(const Vector3 &position);
      void setOrientation(const Quaternion &direction);
  };
}


#endif