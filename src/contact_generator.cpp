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
#ifndef DISABLE_RENDER
    octreeRoot->render();
    Renderer::renderContactGenerator(this);
#endif
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
    if (ignore)
      return;

    if (!cData->hasMoreContacts())
      return;

    int primPair = cpa->tPrimitive | cpb->tPrimitive;
    switch (primPair)
    {
    case PRIMITIVE_TYPE::PRIT_SPHERE:
    {
      Sphere *sA = dynamic_cast<Sphere *>(cpa);
      Sphere *sB = dynamic_cast<Sphere *>(cpb);
      fillContactCeofficient(sA, sB, cData);
      genSphereAndSphere(*sA, *sB, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_BOX:
    {
      Box *boxA = dynamic_cast<Box *>(cpa);
      Box *boxB = dynamic_cast<Box *>(cpb);
      fillContactCeofficient(boxA, boxB, cData);
      genBoxAndBox(*boxA, *boxB, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_CAPSULE:
    {
      Capsule *polyA = dynamic_cast<Capsule *>(cpb);
      Capsule *polyB = dynamic_cast<Capsule *>(cpa);
      fillContactCeofficient(polyA, polyB, cData);
      genCapsuleAndCapsule(*polyA, *polyB, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_POLYHEDRON:
    {
      Polyhedron *polyA = dynamic_cast<Polyhedron *>(cpa);
      Polyhedron *polyB = dynamic_cast<Polyhedron *>(cpb);
      fillContactCeofficient(polyA, polyB, cData);
      genPolyhedronAndPolyhedron(*polyA, *polyB, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_PLANE | PRIMITIVE_TYPE::PRIT_SPHERE:
    {
      Sphere *box = cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE ? dynamic_cast<Sphere *>(cpa) : dynamic_cast<Sphere *>(cpb);
      Plane *plane = cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE ? dynamic_cast<Plane *>(cpb) : dynamic_cast<Plane *>(cpa);
      fillContactCeofficient(box, plane, cData);
      genSphereAndPlane(*box, *plane, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_PLANE | PRIMITIVE_TYPE::PRIT_BOX:
    {
      Box *box = cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX ? dynamic_cast<Box *>(cpa) : dynamic_cast<Box *>(cpb);
      Plane *plane = cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE ? dynamic_cast<Plane *>(cpb) : dynamic_cast<Plane *>(cpa);
      fillContactCeofficient(box, plane, cData);
      genBoxAndPlane(*box, *plane, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_PLANE | PRIMITIVE_TYPE::PRIT_CAPSULE:
    {
      Capsule *capuse = cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE ? dynamic_cast<Capsule *>(cpa) : dynamic_cast<Capsule *>(cpb);
      Plane *plane = cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE ? dynamic_cast<Plane *>(cpb) : dynamic_cast<Plane *>(cpa);
      fillContactCeofficient(capuse, plane, cData);
      genCapsuleAndPlane(*capuse, *plane, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_PLANE | PRIMITIVE_TYPE::PRIT_POLYHEDRON:
    {
      Polyhedron *poly = cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON ? dynamic_cast<Polyhedron *>(cpa) : dynamic_cast<Polyhedron *>(cpb);
      Plane *plane = cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_PLANE ? dynamic_cast<Plane *>(cpb) : dynamic_cast<Plane *>(cpa);
      fillContactCeofficient(poly, plane, cData);
      genPolyhedronAndPlane(*poly, *plane, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_SPHERE | PRIMITIVE_TYPE::PRIT_BOX:
    {
      Box *box = cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX ? dynamic_cast<Box *>(cpa) : dynamic_cast<Box *>(cpb);
      Sphere *sphere = cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE ? dynamic_cast<Sphere *>(cpb) : dynamic_cast<Sphere *>(cpa);
      fillContactCeofficient(box, sphere, cData);
      genBoxAndSphere(*box, *sphere, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_SPHERE | PRIMITIVE_TYPE::PRIT_CAPSULE:
    {
      Capsule *capsule = cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE ? dynamic_cast<Capsule *>(cpa) : dynamic_cast<Capsule *>(cpb);
      Sphere *sphere = cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE ? dynamic_cast<Sphere *>(cpb) : dynamic_cast<Sphere *>(cpa);
      fillContactCeofficient(capsule, sphere, cData);
      genCapsuleAndSphere(*capsule, *sphere, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_SPHERE | PRIMITIVE_TYPE::PRIT_POLYHEDRON:
    {
      Polyhedron *poly = cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON ? dynamic_cast<Polyhedron *>(cpa) : dynamic_cast<Polyhedron *>(cpb);
      Sphere *sphere = cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_SPHERE ? dynamic_cast<Sphere *>(cpb) : dynamic_cast<Sphere *>(cpa);
      fillContactCeofficient(poly, sphere, cData);
      genPolyhedronAndSphere(*poly, *sphere, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_BOX | PRIMITIVE_TYPE::PRIT_CAPSULE:
    {
      Capsule *polyA = cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE ? dynamic_cast<Capsule *>(cpa) : dynamic_cast<Capsule *>(cpb);
      Box *polyB = cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX ? dynamic_cast<Box *>(cpb) : dynamic_cast<Box *>(cpa);
      fillContactCeofficient(polyA, polyB, cData);
      genCapsuleAndBox(*polyA, *polyB, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_BOX | PRIMITIVE_TYPE::PRIT_POLYHEDRON:
    {
      Polyhedron *poly = cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON ? dynamic_cast<Polyhedron *>(cpa) : dynamic_cast<Polyhedron *>(cpb);
      Box *box = cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_BOX ? dynamic_cast<Box *>(cpb) : dynamic_cast<Box *>(cpa);
      fillContactCeofficient(poly, box, cData);
      genPolyhedronAndBox(*poly, *box, cData);
      break;
    }
    case PRIMITIVE_TYPE::PRIT_CAPSULE | PRIMITIVE_TYPE::PRIT_POLYHEDRON:
    {
      Capsule *polyA = cpa->tPrimitive == PRIMITIVE_TYPE::PRIT_CAPSULE ? dynamic_cast<Capsule *>(cpa) : dynamic_cast<Capsule *>(cpb);
      Polyhedron *polyB = cpb->tPrimitive == PRIMITIVE_TYPE::PRIT_POLYHEDRON ? dynamic_cast<Polyhedron *>(cpb) : dynamic_cast<Polyhedron *>(cpa);
      fillContactCeofficient(polyA, polyB, cData);
      genCapsuleAndPolyhedron(*polyA, *polyB, cData);
      break;
    }
    default:
      break;
    }
  }

}