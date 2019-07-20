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

      ffloat penetration;
      Vector3 contactPoint;
      Vector3 contactNormal;
      Vector3 contactVelocity;

      Matrix3 contactToWorld;
      ffloat desiredDeltaVelocity;
      //Holds positions relative to each body's center point
      Vector3 relativeContactPosition[2];

      static ffloat minRestituteVelocity;
      static ffloat maxAngularMove;

    public:
      void setBodyData(RigidBody *a, RigidBody *b, ffloat friction, ffloat restitution);
      void swapBodies();
      void updateAwake();
      void calcContactMatrix();
      void calcInternals(ffloat deltaTime);
      void calcDesiredDeltaVelocity(ffloat deltaTime);
      Vector3 calcLocalVelocity(unsigned index, ffloat deltaTime);
      Vector3 calcContactImpulse();
      void applyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2]);
      void applyPositionChange(Vector3 velocityChange[2], Vector3 rotationDirection[2],
                                ffloat rotationAmount[2], ffloat penetration);
  };

}

#endif