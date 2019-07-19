#ifndef __CONTACT_GENERATOR_H__
#define __CONTACT_GENERATOR_H__

#include <map>
using namespace std;

#include "types.h"
#include "primitive.h"
#include "collision_data.h"

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
      MapProfile mapTrigger;

      //OCTree *octreeRoot;
      //OCTreeNode::PotentialContacts curPotentialContacts;
    public:
      ContactGenerator(const Vector3 &min, const Vector3 &max);
	    ~ContactGenerator();

    public:
      void generateContacts(CollisionData *cData, unsigned maxContacts);
      void addPrimitive(Primitive *pri);
      void removePrimitive(Primitive *pri);
      void update(ffloat deltaTime);

      void addContactRule(unsigned int a, unsigned int b, ffloat friction, ffloat restitution);
      ffloat getContactFriction(unsigned int a, unsigned int b);
      ffloat getContactRestitution(unsigned int a, unsigned int b);

      void addIgnoreCouple(unsigned int a, unsigned int b, bool ignore);
      void addJustTestCouple(unsigned int a, unsigned int b, bool justTest);
      bool isContactIgnore(unsigned int a, unsigned int b);
      bool isJustCollide(unsigned int a, unsigned int b);

      void render();

    private:
      void generateContacts(Primitive *cpa, Primitive *cpb, CollisionData *cData);
      void fillContactParam(Primitive *cpa, Primitive *cpb, CollisionData *cData);
  };
}

#endif