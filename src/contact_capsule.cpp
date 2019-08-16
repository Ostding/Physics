#include "contact_generator.h"
#include "utils.h"
#include "gjk_epa.h"

namespace physics
{
  //////Capsule
  unsigned ContactGenerator::genCapsuleAndPlane( Capsule &capsule, Plane &plane, CollisionData *cData)
  {
    ffloat distUp = capsule.pointWorldUp.dot(plane.direction);
    ffloat distDown = capsule.pointWorldDown.dot(plane.direction);
    Vector3 ptSeg;
    ffloat distMin;
    if(distUp < distDown)
    {
      if(distUp >= capsule.radius) 
        return 0;
      ptSeg = capsule.pointWorldUp;
      distMin = distUp;
    }
    else if(distUp > distDown)
    {
      if(distDown >= capsule.radius)
        return 0;
      ptSeg = capsule.pointWorldDown;
      distMin = distDown;
    }
    else
    {
      if(distUp >= capsule.radius)
        return 0;
      ptSeg = capsule.getColumnVector(3);
      distMin = distUp;
    }

    Vector3 contactPoint = ptSeg - plane.direction * distMin;
    Contact* contact = cData->nextContact;
    contact->contactNormal = plane.direction;
    contact->contactPoint = contactPoint;
		contact->penetration = capsule.radius - distMin;

    contact->setBodyData(capsule.body, 0, cData->friction, cData->restitution);
    cData->addContacts(1);
    return 1;
  }

  unsigned ContactGenerator::genCapsuleAndSphere( Capsule &capsule, Sphere &sphere, CollisionData *cData)
  {
    
    return 0;
  }

  unsigned ContactGenerator::genCapsuleAndBox( Capsule &capsule, Box &box, CollisionData *cData)
  {
    return 0;
  }

  unsigned ContactGenerator::genCapsuleAndPolyhedron( Capsule &capsule, Polyhedron &poly, CollisionData *cData)
  {
    return 0;
  }

  unsigned ContactGenerator::genCapsuleAndCapsule( Capsule &capsuleA, Capsule &capsuleB, CollisionData *cData)
  {
    Vector3 pa, pb;
    ffloat squareDist = Utils::smallestSquareDistanceOfTwoSegments( capsuleA.pointWorldUp, capsuleA.pointWorldDown, 
                                                                    capsuleB.pointWorldUp, capsuleB.pointWorldDown,
                                                                    pa, pb);
    ffloat dist = capsuleA.radius + capsuleB.radius;
    if((dist * dist) <= squareDist) 
      return 0;

    ffloat len = ffsqrt(squareDist);
    Contact* contact = cData->nextContact;
    Vector3 normal = pb - pa;
    normal.normalise();
    contact->contactNormal = normal;
		contact->penetration = (dist - len) * ffhalf;
    contact->contactPoint = pa + normal * (capsuleA.radius - contact->penetration);

    contact->setBodyData(capsuleA.body, capsuleB.body, cData->friction, cData->restitution);
    cData->addContacts(1);
    return 1;
  }

}