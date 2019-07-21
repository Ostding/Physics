#include "contact_generator.h"

namespace physics
{
  ContactGenerator::ContactGenerator(const Vector3 &min, const Vector3 &max)
	{
		AABB a = AABB(min, max);
		octreeRoot = new OCTreeNode(a, 0, 0, NULL);
	}

  ContactGenerator::~ContactGenerator() 
  {
		OCTreeNode::destroy(octreeRoot);
		octreeRoot = NULL;
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
  }

  unsigned ContactGenerator::genSphereAndPlane( Sphere &sphere, Plane &plane, CollisionData *cData)
  {
    if (sphere.isStatic) return 0;
    if (cData->contactsLeft <= 0) return 0;

    Vector3 ps = sphere.getColumnVector(3);
    Vector3 v = (ps - plane.center);
    ffloat dist = v.dot(plane.direction);
    
    if (dist*dist > sphere.radius*sphere.radius)
      return 0;
    
    ffloat dmax = plane.extents.x + sphere.radius;
    //Find sphere center point's projection on this plane
    Vector3 pp = ps - plane.direction.scale(dist);
    ffloat dtx = pp.x - plane.center.x;
    if(dtx >= dmax)
      return 0;

    ffloat dty = pp.y - plane.center.y;
    if(dty >= dmax)
      return 0;

    ffloat dtz = pp.z - plane.center.z;
    if(dtz >= dmax)
      return 0;

    ffloat penetration = sphere.radius - ffabs(dist);
    Vector3 normal = plane.direction;
    if(dist < ffzero)
      normal = -normal;

    Contact* contact = cData->nextContact;
    contact->contactNormal = normal;
    contact->penetration = penetration;
    contact->contactPoint = pp;
    contact->setBodyData(sphere.body, NULL, cData->friction, cData->restitution);
    cData->addContacts(1);

    return 1;
  }

  unsigned ContactGenerator::genSphereAndSphere( Sphere &sphereA, Sphere &SphereB, CollisionData *cData)
  {
    if (cData->contactsLeft <= 0) return 0;

    Vector3 ptA = sphereA.getColumnVector(3);
    Vector3 ptB = SphereB.getColumnVector(3);

    Vector3 mid = ptA - ptB;
    ffloat len = mid.mag();
    if (len <= ffzero || len >= (sphereA.radius + SphereB.radius))
      return 0;

    Vector3 normal = mid * (ffone / len);

    Contact* contact = cData->nextContact;
		contact->contactNormal = normal;
		contact->contactPoint = ptA + mid * ffhalf;
		contact->penetration = (sphereA.radius + SphereB.radius - len);

		RigidBody * b1 = b1->primitive->isStatic ? NULL : b1;
		RigidBody * b2 = b2->primitive->isStatic ? NULL : b2;
		if(!b1 && !b2)
			return 0;

		contact->setBodyData(b1, b2, cData->friction, cData->restitution);
		cData->addContacts(1);

    return 1;
  }


}