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
  }

  unsigned ContactGenerator::genSphereAndPlane( Sphere &sphere, Plane &plane, CollisionData *cData)
  {
    if (sphere.isStatic) return 0;
    if (cData->contactsLeft <= 0) return 0;

    Vector3 ps = sphere.getColumnVector(3);
    Vector3 dirWrold = plane.body->getDirectionInWorldSpace(plane.direction);
    
    Vector3 v = (ps - plane.ptLB);
    ffloat dist = v.dot(dirWrold);
    Vector3 pp = ps - dirWrold.scale(dist);

    bool inLT = Utils::pointInTrangle(pp, plane.ptLB, plane.ptLT, plane.ptRT);
    bool inRB = Utils::pointInTrangle(pp, plane.ptRB, plane.ptLB, plane.ptRT);
    if(inRB || inLT)
    {
      if (dist > sphere.radius)
        return 0;

      ffloat penetration = sphere.radius - ffabs(dist);
      Vector3 normal = dirWrold;
      if(dist < ffzero)
        normal = -normal;

      Contact* contact = cData->nextContact;
      contact->contactNormal = normal;
      contact->penetration = penetration;
      contact->contactPoint = pp;
      contact->setBodyData(sphere.body, 0, cData->friction, cData->restitution);
      cData->addContacts(1);
      return 1;
    }

    //Find the edge whose distance to the projection point is the smallest 
    Vector3 projPoint, minPoint; ffloat dist2, minDist;
    Utils::pointProjectionToSeg(pp, plane.ptLB, plane.ptLT, projPoint, dist2);
    minDist = dist2;
    minPoint = projPoint;
    Utils::pointProjectionToSeg(pp, plane.ptLT, plane.ptRT, projPoint, dist2);
    if(dist2 < minDist)
    {
      minDist = dist2;
      minPoint = projPoint;
    }
    Utils::pointProjectionToSeg(pp, plane.ptRT, plane.ptRB, projPoint, dist2);
    if(dist2 < minDist)
    {
      minDist = dist2;
      minPoint = projPoint;
    }
    Utils::pointProjectionToSeg(pp, plane.ptRB, plane.ptLB, projPoint, dist2);
    if(dist2 < minDist)
    {
      minDist = dist2;
      minPoint = projPoint;
    }
    
    Vector3 cp = (ps - minPoint);
    ffloat magCP = cp.mag();
    if (magCP > sphere.radius)
        return 0;
    ffloat penetration = (sphere.radius - magCP);

    cp.normalize();
    Contact* contact = cData->nextContact;
    contact->contactNormal = cp;
    contact->penetration = penetration;
    contact->contactPoint = minPoint;
    contact->setBodyData(sphere.body, 0, cData->friction, cData->restitution);
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


}