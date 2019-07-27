#include "contact_generator.h"
#include "utils.h"

namespace physics
{
  ContactGenerator::ContactGenerator(const Vector3 &min, const Vector3 &max)
	{
		AABB a = AABB(min, max);
		octreeRoot = new OCTreeNode(a, 0, 0, 0);
	}

  ContactGenerator::~ContactGenerator() 
  {
		OCTreeNode::destroy(octreeRoot);
		octreeRoot = 0;
	}

  void ContactGenerator::addPrimitive(Primitive *pri)
  {
    octreeRoot->insert(pri);
  }

  void ContactGenerator::removePrimitive(Primitive *pri)
  {
    pri->node->remove(pri);
  }

  void ContactGenerator::update(ffloat deltaTime)
  {
    octreeRoot->refresh();
  }


  void ContactGenerator::addContactCeofficient(unsigned int a, unsigned int b, ffloat friction, ffloat restitution)
  {
    unsigned int k = a | b;
		mapFriction[k] = friction;
		mapRestitution[k] = restitution;
  }

  ffloat ContactGenerator::getContactFriction(unsigned int a, unsigned int b)
  {
    unsigned int k = a | b;
		MapCeofficient::iterator it = mapFriction.find(k);
		if (it == mapFriction.end())
			return CollisionData::defFriction;
		
		return it->second;
  }

  ffloat ContactGenerator::getContactRestitution(unsigned int a, unsigned int b)
  {
    unsigned k = a | b;
		MapCeofficient::iterator it = mapRestitution.find(k);
		if (it == mapRestitution.end())
			return CollisionData::defRestitution;

		return it->second;
  }
  
  void ContactGenerator::addIgnoreCouple(unsigned int a, unsigned int b, bool ignore)
  {
    unsigned int k = a | b;
		mapIgnore[k] = ignore;
  }

  bool ContactGenerator::isContactIgnore(unsigned int a, unsigned int b)
  {
    unsigned int k = a | b;
		MapProfile::iterator it = mapIgnore.find(k);
		if (it == mapIgnore.end())
			return false;

		return it->second;
  }
  
  void ContactGenerator::ContactGenerator::render()
  {
    octreeRoot->render();
  }

  void ContactGenerator::generateContacts(CollisionData *cData)
  {
    OCTreeNode::PotentialContacts poContacts;
		octreeRoot->getPotentialContacts(poContacts, mapIgnore);
    if (poContacts.empty())
			return;
		
    curPotentialContacts.clear();

    cData->reset();
		cData->friction = cData->defFriction;
		cData->restitution = cData->defRestitution;

    for (OCTreeNode::PotentialContacts::iterator it = poContacts.begin(); it != poContacts.end(); it++)
		{
			OCTreeNode::ContactPair pa = (*it);
			Primitive *cpa = pa.first;
			Primitive *cpb = pa.second;
			generateContacts(cpa, cpb, cData);

			curPotentialContacts.push_back(std::make_pair(pa.first, pa.second));
		}
  }

  void ContactGenerator::fillContactCeofficient(Primitive *cpa, Primitive *cpb, CollisionData *cData)
	{
		cData->friction = getContactFriction(cpa->tProfile, cpb->tProfile);
		cData->restitution = getContactRestitution(cpa->tProfile, cpb->tProfile);
	}

  void ContactGenerator::generateContacts(Primitive *cpa, Primitive *cpb, CollisionData *cData)
  {
    bool ignore = isContactIgnore(cpa->tContact, cpb->tContact);
		if (ignore) return;

		if (!cData->hasMoreContacts()) return;

    if( cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE &&
        cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE)
    {
      Sphere *box = dynamic_cast<Sphere *>(cpa);
			Plane *plane = dynamic_cast<Plane *>(cpb);
			fillContactCeofficient(box, plane, cData);
      genSphereAndPlane(*box, *plane, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE)
    {
      Sphere *box = dynamic_cast<Sphere *>(cpb);
			Plane *plane = dynamic_cast<Plane *>(cpa);
			fillContactCeofficient(box, plane, cData);
      genSphereAndPlane(*box, *plane, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE)
    {
      Sphere *sA = dynamic_cast<Sphere *>(cpa);
			Sphere *sB = dynamic_cast<Sphere *>(cpb);
			fillContactCeofficient(sA, sB, cData);
      genSphereAndSphere(*sA, *sB, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE)
    {
      Box *box = dynamic_cast<Box *>(cpa);
			Plane *plane = dynamic_cast<Plane *>(cpb);
			fillContactCeofficient(box, plane, cData);
      genBoxAndPlane(*box, *plane, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX)
    {
      Box *box = dynamic_cast<Box *>(cpb);
			Plane *plane = dynamic_cast<Plane *>(cpa);
			fillContactCeofficient(box, plane, cData);
      genBoxAndPlane(*box, *plane, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE)
    {
      Box *box = dynamic_cast<Box *>(cpa);
			Sphere *sphere = dynamic_cast<Sphere *>(cpb);
			fillContactCeofficient(box, sphere, cData);
      genBoxAndSphere(*box, *sphere, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX)
    {
      Box *box = dynamic_cast<Box *>(cpb);
			Sphere *sphere = dynamic_cast<Sphere *>(cpa);
			fillContactCeofficient(box, sphere, cData);
      genBoxAndSphere(*box, *sphere, cData);
    }
  }


  /////Sphere
  unsigned ContactGenerator::genSphereAndPlane( Sphere &sphere, Plane &plane, CollisionData *cData)
  {
    if (sphere.isStatic) return 0;
    if (cData->contactsLeft <= 0) return 0;

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
		contact->contactPoint = ptB + mid * ffhalf;
		contact->penetration = (sphereA.radius + sphereB.radius - len);

		contact->setBodyData(b1, b2, cData->friction, cData->restitution);
		cData->addContacts(1);

    return 1;
  }




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
    if (ffabs(relCentre.x) - sphere.radius > box.extents.x ||
        ffabs(relCentre.y) - sphere.radius > box.extents.y ||
        ffabs(relCentre.z) - sphere.radius > box.extents.z)
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
      closestPtWorld = box.transform.transform(closestPt);
      normal = (centre - closestPtWorld);
      penetration += ffsqrt(dist);
    }
		else
		{
      dist = (closestPt - relCentre).squareMag();
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

  bool ContactGenerator::overlapOnAxis(Box &boxA, Box &boxB, const Vector3 &axis)
  {
    return false;
  }

  unsigned ContactGenerator::genBoxAndBox( Box &boxA, Box &boxB, CollisionData *cData)
  {
    return 0;
  }

}