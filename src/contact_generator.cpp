#include "contact_generator.h"
#include "utils.h"
#include "gjk_epa.h"

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
    Renderer::renderContactGenerator(this);
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

			curPotentialContacts.emplace_back(std::make_pair(pa.first, pa.second));
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
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX)
    {
      Box *boxA = dynamic_cast<Box *>(cpa);
			Box *boxB = dynamic_cast<Box *>(cpb);
			fillContactCeofficient(boxA, boxB, cData);
      genBoxAndBox(*boxA, *boxB, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE)
    {
      Polyhedron *poly = dynamic_cast<Polyhedron *>(cpa);
			Plane *plane = dynamic_cast<Plane *>(cpb);
			fillContactCeofficient(poly, plane, cData);
      genPolyhedronAndPlane(*poly, *plane, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON)
    {
      Polyhedron *poly = dynamic_cast<Polyhedron *>(cpb);
			Plane *plane = dynamic_cast<Plane *>(cpa);
			fillContactCeofficient(poly, plane, cData);
      genPolyhedronAndPlane(*poly, *plane, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE)
    {
      Polyhedron *poly = dynamic_cast<Polyhedron *>(cpa);
			Sphere *sphere = dynamic_cast<Sphere *>(cpb);
			fillContactCeofficient(poly, sphere, cData);
      genPolyhedronAndSphere(*poly, *sphere, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON)
    {
      Polyhedron *poly = dynamic_cast<Polyhedron *>(cpb);
			Sphere *sphere = dynamic_cast<Sphere *>(cpa);
			fillContactCeofficient(poly, sphere, cData);
      genPolyhedronAndSphere(*poly, *sphere, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX)
    {
      Polyhedron *poly = dynamic_cast<Polyhedron *>(cpa);
			Box *box = dynamic_cast<Box *>(cpb);
			fillContactCeofficient(poly, box, cData);
      genPolyhedronAndBox(*poly, *box, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON)
    {
      Polyhedron *poly = dynamic_cast<Polyhedron *>(cpb);
			Box *box = dynamic_cast<Box *>(cpa);
		  fillContactCeofficient(poly, box, cData);
      genPolyhedronAndBox(*poly, *box, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON)
    {
      Polyhedron *polyA = dynamic_cast<Polyhedron *>(cpa);
			Polyhedron *polyB = dynamic_cast<Polyhedron *>(cpb);
		  fillContactCeofficient(polyA, polyB, cData);
      genPolyhedronAndPolyhedron(*polyA, *polyB, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE)
    {
      Capsule *polyA = dynamic_cast<Capsule *>(cpa);
			Plane   *polyB = dynamic_cast<Plane *>(cpb);
      fillContactCeofficient(polyA, polyB, cData);
      genCapsuleAndPlane(*polyA, *polyB, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE)
    {
      Capsule *polyA = dynamic_cast<Capsule *>(cpb);
			Plane   *polyB = dynamic_cast<Plane *>(cpa);
      fillContactCeofficient(polyA, polyB, cData);
      genCapsuleAndPlane(*polyA, *polyB, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE)
    {
      Capsule *polyA = dynamic_cast<Capsule *>(cpa);
			Sphere   *polyB = dynamic_cast<Sphere *>(cpb);
      fillContactCeofficient(polyA, polyB, cData);
      genCapsuleAndSphere(*polyA, *polyB, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE)
    {
      Capsule *polyA = dynamic_cast<Capsule *>(cpb);
			Sphere   *polyB = dynamic_cast<Sphere *>(cpa);
      fillContactCeofficient(polyA, polyB, cData);
      genCapsuleAndSphere(*polyA, *polyB, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX)
    {
      Capsule *polyA = dynamic_cast<Capsule *>(cpa);
			Box   *polyB = dynamic_cast<Box *>(cpb);
      fillContactCeofficient(polyA, polyB, cData);
      genCapsuleAndBox(*polyA, *polyB, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE)
    {
      Capsule *polyA = dynamic_cast<Capsule *>(cpb);
			Box   *polyB = dynamic_cast<Box *>(cpa);
      fillContactCeofficient(polyA, polyB, cData);
      genCapsuleAndBox(*polyA, *polyB, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON)
    {
      Capsule      *polyA = dynamic_cast<Capsule *>(cpa);
			Polyhedron   *polyB = dynamic_cast<Polyhedron *>(cpb);
      fillContactCeofficient(polyA, polyB, cData);
      genCapsuleAndPolyhedron(*polyA, *polyB, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE)
    {
      Capsule     *polyA = dynamic_cast<Capsule *>(cpb);
			Polyhedron  *polyB = dynamic_cast<Polyhedron *>(cpa);
      fillContactCeofficient(polyA, polyB, cData);
      genCapsuleAndPolyhedron(*polyA, *polyB, cData);
    }
    else if(cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE &&
            cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE)
    {
      Capsule *polyA = dynamic_cast<Capsule *>(cpb);
			Capsule *polyB = dynamic_cast<Capsule *>(cpa);
      fillContactCeofficient(polyA, polyB, cData);
      genCapsuleAndCapsule(*polyA, *polyB, cData);
    }
  }


}