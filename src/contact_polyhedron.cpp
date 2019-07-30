#include "contact_generator.h"
#include "utils.h"
#include "gjk_epa.h"

namespace physics
{
  //////Polyhedron
  unsigned ContactGenerator::genPolyhedronAndPlane( Polyhedron &poly, Plane &plane, CollisionData *cData)
  {
    
    return 0;
  }

  unsigned ContactGenerator::genPolyhedronAndSphere( Polyhedron &poly, Sphere &sphere, CollisionData *cData)
  {
    return 0;
  }

  unsigned ContactGenerator::genPolyhedronAndBox( Polyhedron &poly, Box &box, CollisionData *cData)
  {
    return 0;
  }

  unsigned ContactGenerator::genPolyhedronAndCapsule( Polyhedron &polyA, Polyhedron &polyB, CollisionData *cData)
  {
    return 0;
  }


}