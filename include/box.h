#ifndef __BOX_H__
#define __BOX_H__

#include <vector>
using namespace std;
#include "types.h"
#include "primitive.h"
#include "vector3.h"
#include "body.h"

namespace physics
{
  class Box : public Primitive
  {
    public:
      Vector3 extents;
      typedef std::vector<Vector3> Points;
      Points pointsLocal;
      Points pointsWorld;

    public:
      Box();
      Box(RigidBody *body);
      Box(const Vector3 &extents);

    public:
      virtual void refreshAABB();
      void updateCorners();
      Vector3 findFarthestPointInDirection(const Vector3 &dir);
  };
}


#endif



