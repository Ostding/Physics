#include "contact_generator.h"
#include "utils.h"
#include "gjk_epa.h"
#include "utils.h"

namespace physics
{
  //////Polyhedron
  unsigned ContactGenerator::genPolyhedronAndPlane( Polyhedron &poly, Plane &plane, CollisionData *cData)
  {
    if (cData->contactsLeft <= 0) return 0;
    if (!poly.body->isAwake) return 0;

    unsigned n = 0;
    Box::Points::iterator it = poly.pointsWorld.begin();
    for(; it < poly.pointsWorld.end(); it++)
    {
      ffloat ptToOrign = it->dot(plane.direction);
      if(ptToOrign <= plane.offset) 
      {
        ffloat penetration = (plane.offset - ptToOrign);
        Vector3 contactPoint = *it + plane.direction * penetration;
        Contact* contact = cData->nextContact;
        contact->contactNormal = plane.direction;
        contact->contactPoint = contactPoint;
        contact->penetration = penetration;

        contact->setBodyData(poly.body, 0, cData->friction, cData->restitution);
        cData->addContacts(1);
        n += 1;
      }
    }
    return n;
  }

  unsigned ContactGenerator::genPolyhedronAndSphere( Polyhedron &poly, Sphere &sphere, CollisionData *cData)
  {
    if (cData->contactsLeft <= 0) return 0;
    if (!poly.body->isAwake && !sphere.body->isAwake) return 0;

    bool suc = GjkEpa::generateContacts (&poly, &sphere, cData);
    return suc ? 1 : 0;
  }

  unsigned ContactGenerator::genPolyhedronAndBox( Polyhedron &poly, Box &box, CollisionData *cData)
  {
    if (cData->contactsLeft <= 0) return 0;
    if (!poly.body->isAwake && !box.body->isAwake) return 0;
    
    bool suc = GjkEpa::generateContacts (&poly, &box, cData);
    return suc ? 1 : 0;
  }

  unsigned ContactGenerator::genPolyhedronAndPolyhedron( Polyhedron &polyA, Polyhedron &polyB, CollisionData *cData)
  {
    if (cData->contactsLeft <= 0) return 0;
    if (!polyA.body->isAwake && !polyB.body->isAwake) return 0;
    
    bool suc = GjkEpa::generateContacts (&polyA, &polyB, cData);
    return suc ? 1 : 0;
  }


}