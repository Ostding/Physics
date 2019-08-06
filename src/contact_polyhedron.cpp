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

    Vector3 posPoly = poly.getColumnVector(3);
    Vector3 posSphere = sphere.getColumnVector(3);
    Vector3 dir = posSphere - posPoly;
    
    //Find closest planar
    ffloat distMax = ffzero;
    Vector3 normal;
    
    std::vector<Vector3> triangleP0;
    std::vector<Vector3> triangleP1;
    std::vector<Vector3> triangleP2;

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

      if(i == 0)
      {
        distMax = d;
        normal = n;
        
        triangleP0.clear();
        triangleP1.clear();
        triangleP2.clear();

        triangleP0.emplace_back(p0);
        triangleP1.emplace_back(p1);
        triangleP2.emplace_back(p2);
      }
      else
      {
        if(distMax < d)
        {
          distMax = d;
          normal = n;

          triangleP0.clear();
          triangleP1.clear();
          triangleP2.clear();

          triangleP0.emplace_back(p0);
          triangleP1.emplace_back(p1);
          triangleP2.emplace_back(p2);
        }else
        if(distMax == d)
        {
          distMax = d;
          normal = n;

          triangleP0.emplace_back(p0);
          triangleP1.emplace_back(p1);
          triangleP2.emplace_back(p2);
        }
      }
      
    }

    Vector3 closestPt; 
    ffloat distMin;
    int index;
    for(int i=0; i< triangleP0.size(); i ++)
    {
      Vector3 p0 = triangleP0[i];
      Vector3 p1 = triangleP1[i];
      Vector3 p2 = triangleP2[i];

      Vector3 v = posSphere - p0;
      ffloat dist = v.dot(normal);
      Vector3 ptInPlanar = posSphere - normal*dist;
      bool inTriangle = Utils::pointInTriangle(ptInPlanar, p0, p1, p2);
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
      Vector3 projectPt;
      Utils::pointProjectionToSegment(ptInPlanar, p0, p1, projectPt, dist);
      if(i == 0)
      {
        index = i;
        distMin = dist;
        closestPt = projectPt;
      }  
      else
      {
        if(dist < distMin)
        {
          index = i;
          distMin = dist;
          closestPt = projectPt;
        }
      }
      
      Utils::pointProjectionToSegment(ptInPlanar, p1, p2, projectPt, dist);
      if(dist < distMin)
      {
        index = i;
        distMin = dist;
        closestPt = projectPt;
      }
      Utils::pointProjectionToSegment(ptInPlanar, p2, p0, projectPt, dist);
      if(dist < distMin)
      {
        distMin = dist;
        closestPt = projectPt;
      }
    }

    //debug closest triangle
    // _triangleP0 = triangleP0[index];
    // _triangleP1 = triangleP1[index];
    // _triangleP2 = triangleP2[index];
    // _contactNormal = normal;
   
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