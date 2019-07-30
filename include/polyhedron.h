#ifndef __POLYHEDRON_H__
#define __POLYHEDRON_H__

#include <vector>
using namespace std;
#include "types.h"
#include "primitive.h"
#include "vector3.h"
#include "body.h"


namespace physics
{
  class Polyhedron : public Primitive 
  {
    public:
      Polyhedron(RigidBody *body);
      Polyhedron();

    public:
      typedef std::vector<Vector3> Points;
      Points pointsLocal;
      Points pointsWorld;

      typedef std::vector<unsigned> Indecies;
      Indecies indecies;

    public:
      virtual void refreshAABB();
      virtual void render();
      virtual void findFarthestPointInDirection(const Vector3 &direction, Vector3 &pointLocal, Vector3 &pointWorld);
      
      void setPosition(const Vector3 &position);
      void setOrientation(const Quaternion &direction);

      void setPoints(Points &points, Indecies &indecies);
      void getInertiaTensor(ffloat mass, Matrix3 &inertiaTensor);

    private:
      //Refer to Polyhedral Mass Properties (Revisited) 
      //  The pseudocode for computing the integrals is quite simple. The polyhedron vertices are passes as the array
      //  points[]. The number of triangles is triangleCount. The array indices[] has triangleCount triples of unsigned integers that are indices into
      //  the vertex array. 
      //  The return values are the mass, the center of mass, and the inertia tensor relative to the center of mass.
      //Note: The code assumes that the rigid body has constant density 1. If your rigid body has constant
      //  density D, then you need to multiply the output mass by D and the output inertia tensor by D.
      void calculateInertiaTensor(Points &points, Indecies &indices, ffloat &mass,
                                  Vector3 &massCenter, Matrix3 &inertiaTensor);
  };
}

#endif