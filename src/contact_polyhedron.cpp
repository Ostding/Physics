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
        Contact* contact = cData->nextContact;
        contact->contactNormal = plane.direction;
        contact->contactPoint = (*it);
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

    Vector3 posPoly = poly.getColumnVector(3);
    Vector3 posSphere = sphere.getColumnVector(3);
    Vector3 dir = posSphere - posPoly;
    
    //Find closest planar
    ffloat distMax = ffzero;
    Vector3 normal;
    Vector3 triangleP0, triangleP1, triangleP2;

    unsigned triangleCount = poly.indices.size() / 3;
    for(unsigned i=0; i< triangleCount; i++)
    {
      unsigned i0 = poly.indices[i*3];
      unsigned i1 = poly.indices[i*3+1];
      unsigned i2 = poly.indices[i*3+2];

      Vector3 &p0 = poly.pointsWorld[i0];
      Vector3 &p1 = poly.pointsWorld[i1]; 
      Vector3 &p2 = poly.pointsWorld[i2]; 

      Vector3 da = p1 - p0;
      Vector3 db = p2 - p0;
      Vector3 n = da.cross(db);
      n.normalise();
      ffloat d = n.dot(dir);

      if(i == 0 || distMax < d)
      {
        distMax = d;
        normal = n;

        triangleP0 = p0;
        triangleP1 = p1;
        triangleP2 = p2;
      }
    }

    //debug closest triangle
    contactTriangle[0] = triangleP0;
    contactTriangle[1] = triangleP1;
    contactTriangle[2] = triangleP2;
    
    Vector3 v = posSphere - triangleP0;
    ffloat dist = v.dot(normal);
    Vector3 ptInPlanar = posSphere - normal.scale(dist);
    bool inTriangle = Utils::pointInTriangle(ptInPlanar, triangleP0, triangleP1, triangleP2);
    if(inTriangle)
    {
      if (dist > sphere.radius)
        return 0;
      
      ffloat penetration = sphere.radius - dist;
      normal.normalise();

      Contact* contact = cData->nextContact;
      contact->contactNormal = normal;
      contact->penetration = penetration;
      contact->contactPoint = ptInPlanar;
      contact->setBodyData(sphere.body, 0, cData->friction, cData->restitution);
      cData->addContacts(1);
      return 1;
    }
    
    //Find the edge whose distance to the projection point is the smallest 
    Vector3 projectPt, closestPt; 
    Utils::pointProjectionToSegment(ptInPlanar, triangleP0, triangleP1, projectPt, dist);
    distMax = dist;
    closestPt = projectPt;

    Utils::pointProjectionToSegment(ptInPlanar, triangleP1, triangleP2, projectPt, dist);
    if(dist < distMax)
    {
      distMax = dist;
      closestPt = projectPt;
    }
    Utils::pointProjectionToSegment(ptInPlanar, triangleP2, triangleP0, projectPt, dist);
    if(dist < distMax)
    {
      distMax = dist;
      closestPt = projectPt;
    }

    Vector3 contactNormal = (posSphere - closestPt);
    ffloat distance = contactNormal.mag();
    if (distance > sphere.radius)
        return 0;

    ffloat penetration = (sphere.radius - distance);
    contactNormal.normalise();

    Contact* contact = cData->nextContact;
    contact->contactNormal = contactNormal;
    contact->penetration = penetration;
    contact->contactPoint = closestPt;
    contact->setBodyData(sphere.body, 0, cData->friction, cData->restitution);
    cData->addContacts(1);

    return 1;
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