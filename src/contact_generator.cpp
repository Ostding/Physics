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


  void ContactGenerator::addContactRule(unsigned int a, unsigned int b, ffloat friction, ffloat restitution)
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
			return -1;
		
		return it->second;
  }

  ffloat ContactGenerator::getContactRestitution(unsigned int a, unsigned int b)
  {
    unsigned k = a | b;
		MapCeofficient::iterator it = mapRestitution.find(k);
		if (it == mapRestitution.end())
			return -1;

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

  void ContactGenerator::fillContactParam(Primitive *cpa, Primitive *cpb, CollisionData *cData)
	{
		ffloat friction = getContactFriction(cpa->tProfile, cpb->tProfile);
		cData->friction = (friction == -ffone) ? cData->defFriction : friction;

		ffloat restitution = getContactRestitution(cpa->tProfile, cpb->tProfile);
		cData->restitution = (restitution == -ffone) ? cData->defRestitution : restitution;
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
			fillContactParam(box, plane, cData);
      genSphereAndPlane(*box, *plane, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE &&
        cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE)
    {
       Sphere *box = dynamic_cast<Sphere *>(cpa);
			Plane *plane = dynamic_cast<Plane *>(cpb);
			fillContactParam(box, plane, cData);
      genSphereAndPlane(*box, *plane, cData);
    }
  }

  unsigned ContactGenerator::genSphereAndPlane( Sphere &sphere, Plane &plane, CollisionData *cData)
  {
    
  }

  unsigned ContactGenerator::genSphereAndSphere( Sphere &sphereA, Sphere &SphereB, CollisionData *cData)
  {

  }


}