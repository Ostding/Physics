#ifndef __CONTACT_H__
#define __CONTACT_H__

#include "types.h"
#include "body.h"

namespace physics
{
  class Contact
  {
    public:
      RigidBody *bodies[2];

      ffloat friction;
      ffloat restitution;

      ffloat peneration;
      Vector3 contactPoint;
      Vector3 contactNormal;
      Vector3 contactVelocity;

      Matrix3 contactToWorld;
      ffloat desiredDeltaVelocity;
      //Holds positions relative to each body's center point
      Vector3 relativeContactPosition[2];

      static ffloat minRestituteVelocity;
    public:
      void setBodyData(RigidBody *a, RigidBody *b, ffloat friction, ffloat restitution);
      void swapBodies();
      void updateAwake();
      void calcContactMatrix();
      void calcInternals(ffloat deltaTime);
      void calcDesiredDeltaVelocity(ffloat deltaTime);
      Vector3 calcLocalVelocity(unsigned index, ffloat deltaTime);
      void applyImpulse(const Vector3 &impulse, RigidBody *body, Vector3 *velocityChange, Vector3 *rotationChange);
      void applyVelocityChange(Vector3 veloctiyChange[2], Vector3 rotationChange[2]);
      void applyPositionChange(Vector3 velocityChange[2], Vector3 rotationDirection[2],
                                ffloat rotationAmount[2], ffloat penetration);
  };

}

#endif