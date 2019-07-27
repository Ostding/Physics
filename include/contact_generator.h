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
#include "box.h"

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

      //sphere
      unsigned genSphereAndPlane( Sphere &sphere, Plane &plane, CollisionData *cData);
      unsigned genSphereAndSphere( Sphere &sphereA, Sphere &sphereB, CollisionData *cData);

      //box  
      unsigned genBoxAndPlane( Box &box, Plane &plane, CollisionData *cData);
      unsigned genBoxAndSphere( Box &box, Sphere &sphere, CollisionData *cData);
      bool overlapOnAxis(Box &boxA, Box &boxB, const Vector3 &axis);
      unsigned genBoxAndBox( Box &boxA, Box &boxB, CollisionData *cData);

      //capsule
      // unsigned genCapsuleAndPlane( Capsule &capsule, Plane &plane, CollisionData *cData);
      // unsigned genCapsuleAndSphere( Capsule &capsule, Sphere &sphere, CollisionData *cData);
      // unsigned genCapsuleAndBox( Capsule &capsule, Box &box, CollisionData *cData);
      // unsigned genCapsuleAndCapsule( Capsule &capsuleA, Capsule &capsuleB, CollisionData *cData);

      //polyhedron
      // unsigned genPolyhedronAndPlane( Polyhedron &poly, Plane &plane, CollisionData *cData);
      // unsigned genPolyhedronAndSphere( Polyhedron &poly, Sphere &sphere, CollisionData *cData);
      // unsigned genPolyhedronAndBox( Polyhedron &poly, Box &box, CollisionData *cData);
      // unsigned genPolyhedronAndCapsule( Polyhedron &poly, Capsule &capsule, CollisionData *cData);
      // unsigned genPolyhedronAndCapsule( Polyhedron &polyA, Polyhedron &polyB, CollisionData *cData);
  };
}

#endif