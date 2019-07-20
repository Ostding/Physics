#ifndef __CONTACT_GENERATOR_H__
#define __CONTACT_GENERATOR_H__

#include <map>
using namespace std;

#include "types.h"
#include "collision_data.h"
#include "octree.h"
#include "primitive.h"
#include "sphere.h"
#include "plane.h"

namespace physics
{
  class ContactGenerator
  {
    public:
      typedef map<unsigned int, ffloat> MapCeofficient;
      MapCeofficient mapFriction;
      MapCeofficient mapRestitution;

      typedef map<unsigned int, bool> MapProfile;
      MapProfile mapIgnore;

      OCTreeNode *octreeRoot;
      OCTreeNode::PotentialContacts curPotentialContacts;
      
    public:
      ContactGenerator(const Vector3 &min, const Vector3 &max);
	    ~ContactGenerator();

    public:
      void addPrimitive(Primitive *pri);
      void removePrimitive(Primitive *pri);
      void update(ffloat deltaTime);

      void addContactCeofficient(unsigned int a, unsigned int b, ffloat friction, ffloat restitution);
      ffloat getContactFriction(unsigned int a, unsigned int b);
      ffloat getContactRestitution(unsigned int a, unsigned int b);

      void addIgnoreCouple(unsigned int a, unsigned int b, bool ignore);
      bool isContactIgnore(unsigned int a, unsigned int b);

      void render();

      void generateContacts(CollisionData *cData);

    private:
      void generateContacts(Primitive *cpa, Primitive *cpb, CollisionData *cData);
      void fillContactCeofficient(Primitive *cpa, Primitive *cpb, CollisionData *cData);

      unsigned genSphereAndPlane( Sphere &sphere, Plane &plane, CollisionData *cData);
      unsigned genSphereAndSphere( Sphere &sphereA, Sphere &SphereB, CollisionData *cData);

  };
}

#endif