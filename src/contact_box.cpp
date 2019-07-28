#include "contact_generator.h"
#include "utils.h"
#include "gjk_epa.h"

namespace physics
{
  ///////Box
  unsigned ContactGenerator::genBoxAndPlane( Box &box, Plane &plane, CollisionData *cData)
  {
    if (box.isStatic) return 0;
    if (cData->contactsLeft <= 0) return 0;

    ffloat projLen = Utils::calcProjectionLengthOnAxis(box.extents, box.transform, plane.direction);
    ffloat projDist = box.getColumnVector(3).dot(plane.direction);
    ffloat boxToOrign = projDist - projLen;
    if(boxToOrign >= plane.offset) 
      return 0;

    unsigned n = 0;
    Box::Points::iterator it = box.pointsWorld.begin();
    for(; it < box.pointsWorld.end(); it++)
    {
      ffloat ptToOrign = it->dot(plane.direction);
      if(ptToOrign <= plane.offset) 
      {
        ffloat penetration = (plane.offset - ptToOrign);
        Contact* contact = cData->nextContact;
        contact->contactNormal = plane.direction;
        contact->contactPoint = (*it);
        contact->penetration = penetration;

        contact->setBodyData(box.body, 0, cData->friction, cData->restitution);
        cData->addContacts(1);
        n += 1;
      }
    }
    return n;
  }

  unsigned ContactGenerator::genBoxAndSphere( Box &box, Sphere &sphere, CollisionData *cData)
  {
    Vector3 centre = sphere.getColumnVector(3);
    Vector3 relCentre = box.transform.inverseTransform(centre);
    if (ffabs(relCentre.x) - sphere.radius >= box.extents.x ||
        ffabs(relCentre.y) - sphere.radius >= box.extents.y ||
        ffabs(relCentre.z) - sphere.radius >= box.extents.z)
    {
      return 0;
    }

    Vector3 closestPt(0,0,0);
    ffloat dist;
  
    dist = relCentre.x;
		bool halfIn = true;
		if (dist > box.extents.x) { dist = box.extents.x; halfIn = false; }
    if (dist < -box.extents.x){ dist = -box.extents.x; halfIn = false; }
    closestPt.x = dist;

    dist = relCentre.y;
    if (dist > box.extents.y) { dist = box.extents.y; halfIn = false; }
    if (dist < -box.extents.y){ dist = -box.extents.y; halfIn = false; }
    closestPt.y = dist;

    dist = relCentre.z;
		if (dist > box.extents.z) { dist = box.extents.z; halfIn = false; }
		if (dist < -box.extents.z) { dist = -box.extents.z; halfIn = false; }
    closestPt.z = dist;

    Vector3 normal, closestPtWorld;
		ffloat penetration = sphere.radius;
		if (halfIn)
		{
      ffloat dx = ffabs(relCentre.x);
      ffloat dy = ffabs(relCentre.y);
      ffloat dz = ffabs(relCentre.z);
      int d = 0;
      if(dx > dy)
      {
        d = 1;
        if(dy > dz)
          d = 2;
      }

      if(d == 0) 
      {
        if(closestPt.x < ffzero)
          closestPt.x = -box.extents.x;
        else
          closestPt.x = box.extents.x;
      }
      if(d == 1) 
      {
        if(closestPt.y < ffzero)
          closestPt.y = -box.extents.y;
        else
          closestPt.y = box.extents.y;
      }
      if(d == 2) 
      {
        if(closestPt.z < ffzero)
          closestPt.z = -box.extents.z;
        else
          closestPt.z = box.extents.z;
      }

      dist = (closestPt - relCentre).squareMag();
      if(dist >= sphere.squareRadius) return 0;
      closestPtWorld = box.transform.transform(closestPt);
      normal = (centre - closestPtWorld);
      penetration += ffsqrt(dist);
    }
		else
		{
      dist = (closestPt - relCentre).squareMag();
      if(dist >= sphere.squareRadius) return 0;
      closestPtWorld = box.transform.transform(closestPt);
			normal = (closestPtWorld - centre);
			penetration -= ffsqrt(dist);
		}

    normal.normalise();

    Contact* contact = cData->nextContact;
    contact->contactNormal = normal;
    contact->contactPoint = closestPtWorld;
		contact->penetration = penetration;

    RigidBody *rb = box.isStatic ? 0 : box.body;
    RigidBody *rs = sphere.isStatic ? 0 : sphere.body;
		if (!rb && !rs)
			return 0;

    contact->setBodyData(rb, rs, cData->friction, cData->restitution);
    cData->addContacts(1);

    return 1;
  }

  unsigned ContactGenerator::genBoxAndBox( Box &boxA, Box &boxB, CollisionData *cData)
  {
    bool suc = GjkEpa::generateContacts (&boxA, &boxB, cData);
    return suc ? 1 : 0;
  }
}