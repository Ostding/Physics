#ifndef __POLYHEDRON_H__
#define __POLYHEDRON_H__

#include <vector>
using namespace std;
#include "types.h"
#include "primitive.h"
#include "vector3.h"
#include "body.h"


namespace physics
{
  class Polyhedron : public Primitive 
  {
    public:
      Polyhedron(RigidBody *body);
      Polyhedron();

    public:
      typedef std::vector<Vector3> Points;
      Points pointsLocal;
      Points pointsWorld;
      
    public:
      virtual void refreshAABB();
      virtual void render();
      virtual void findFarthestPointInDirection(const Vector3 &direction, Vector3 &pointLocal, Vector3 &pointWorld);
      
      void setPosition(const Vector3 &position);
      void setOrientation(const Quaternion &direction);

      void addPoints(Vector3 *points, unsigned count);

  };
}

#endif