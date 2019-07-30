#include "contact_generator.h"

namespace physics
{
  /////Sphere
  unsigned ContactGenerator::genSphereAndPlane( Sphere &sphere, Plane &plane, CollisionData *cData)
  {
    if (cData->contactsLeft <= 0) return 0;
    if (!sphere.body->isAwake) return 0;

    Vector3 position = sphere.getColumnVector(3);
    ffloat centreDistance = plane.direction.dot(position) - plane.offset;

    if (centreDistance * centreDistance > sphere.radius * sphere.radius)
      return 0;

    Vector3 normal = plane.direction;
    ffloat penetration = -centreDistance;

    if (centreDistance < ffzero)
    {
			normal = -normal;
      penetration = -penetration;
    }
    penetration += sphere.radius;
		
		RigidBody *b = sphere.isStatic ? 0 : sphere.body;
		if (!b) 
			return 0;

    Contact* contact = cData->nextContact;
    contact->contactNormal = normal;
    contact->penetration = penetration;
    contact->contactPoint = position - plane.direction * centreDistance;
    contact->setBodyData(b, 0, cData->friction, cData->restitution);
    cData->addContacts(1);
    return 1;
  }

  unsigned ContactGenerator::genSphereAndSphere( Sphere &sphereA, Sphere &sphereB, CollisionData *cData)
  {
    if (cData->contactsLeft <= 0) return 0;
    if (!sphereA.body->isAwake && !sphereB.body->isAwake) return 0;

    RigidBody * b1 = sphereA.isStatic ? 0 : sphereA.body;
		RigidBody * b2 = sphereB.isStatic ? 0 : sphereB.body;
		if(!b1 && !b2)
			return 0;

    Vector3 ptA = sphereA.getColumnVector(3);
    Vector3 ptB = sphereB.getColumnVector(3);

    Vector3 mid = ptA - ptB;
    ffloat squareLen = mid.squareMag();
    ffloat dist = (sphereA.radius + sphereB.radius);
    if (squareLen >= (dist * dist))
      return 0;

    ffloat len = ffsqrt(squareLen);
    Vector3 normal = mid * (ffone / len);

    Contact* contact = cData->nextContact;
		contact->contactNormal = normal;
    contact->penetration = (sphereA.radius + sphereB.radius - len) / fftwo;
		contact->contactPoint = ptB + normal * (sphereB.radius - contact->penetration);//mid * ffhalf;

		contact->setBodyData(b1, b2, cData->friction, cData->restitution);
		cData->addContacts(1);

    return 1;
  }

}