#include "contact_generator.h"
#include "utils.h"
#include "gjk_epa.h"

namespace physics
{
  //////Capsule
  unsigned ContactGenerator::genCapsuleAndPlane( Capsule &capsule, Plane &plane, CollisionData *cData)
  {
    if (cData->contactsLeft <= 0) return 0;
    if (!capsule.body->isAwake) return 0;

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
    if (cData->contactsLeft <= 0) return 0;
    if (!capsule.body->isAwake && !sphere.body->isAwake) return 0;

    Vector3 ptSphere = sphere.getTransformPos();
    Vector3 ptProj;
    ffloat squareMinDist;
    Utils::pointProjectionToSegment(ptSphere, capsule.pointWorldUp, capsule.pointWorldDown, ptProj, squareMinDist);
    ffloat dist = sphere.radius + capsule.radius;
    if(squareMinDist >= dist * dist)
      return 0;

    ffloat len = ffsqrt(squareMinDist);
    Contact* contact = cData->nextContact;
    Vector3 normal = ptProj - ptSphere;
    normal.normalise();
    ffloat penetration = (dist - len) * ffhalf;
    contact->contactNormal = normal;
		contact->penetration = penetration;
    contact->contactPoint = ptSphere + normal * (sphere.radius - penetration);

    contact->setBodyData(capsule.body, sphere.body, cData->friction, cData->restitution);
    cData->addContacts(1);
    return 1;
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
    if (cData->contactsLeft <= 0) return 0;
    if (!capsuleA.body->isAwake && !capsuleB.body->isAwake) return 0;

    Vector3 pa, pb;
    ffloat squareDist = Utils::squareDistanceSegment2Segments( capsuleA.pointWorldUp, capsuleA.pointWorldDown, 
                                                               capsuleB.pointWorldUp, capsuleB.pointWorldDown,
                                                               pa, pb);
    ffloat dist = capsuleA.radius + capsuleB.radius;
    if((dist * dist) <= squareDist) 
      return 0;

    ffloat len = ffsqrt(squareDist);
    Contact* contact = cData->nextContact;
    Vector3 normal = pa - pb;
    normal.normalise();
    ffloat penetration = (dist - len) * ffhalf;
    contact->contactNormal = normal;
		contact->penetration = penetration;
    contact->contactPoint = pa + normal * (capsuleA.radius - penetration);

    contact->setBodyData(capsuleA.body, capsuleB.body, cData->friction, cData->restitution);
    cData->addContacts(1);
    return 1;
  }

}