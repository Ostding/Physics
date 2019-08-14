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
      Capsule(ffloat halfHeigh, ffloat radius);

    public:
      ffloat halfHeight;
      ffloat radius;
      
      Vector3 pointLocalUp;
      Vector3 pointLocalDown;
      Vector3 pointWorldUp;
      Vector3 pointWorldDown;

    public:
      virtual void refreshAABB();
      virtual void render();
      virtual void findFarthestPointInDirection(const Vector3 &direction, Vector3 &pointWorld);

      void setPosition(const Vector3 &position);
      void setOrientation(const Quaternion &direction);
  };
}


#endif