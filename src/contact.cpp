#include <assert.h>
#include "contact.h"

namespace physics
{

  ffloat Contact::minRestituteVelocity = ffloat(25000LL);

  void Contact::setBodyData(RigidBody *a, RigidBody *b, ffloat friction, ffloat restitution)
  {
    bodies[0] = a;
    bodies[1] = b;

    Contact::friction = friction;
    Contact::restitution = restitution;
  }
  
  void Contact::swapBodies()
  {
    contactNormal *= -ffone;
    RigidBody *temp = bodies[0];
    bodies[0] = bodies[1];
    bodies[1] = temp;
  }

  void Contact::updateAwake()
  {
     if (!bodies[1]) return;

    bool awake0 = bodies[0]->isAwake;
    bool awake1 = bodies[1]->isAwake;

    // Just wake up the sleeping one
    if (awake0 ^ awake1) {
        if (awake0) bodies[1]->setAwake();
        else bodies[0]->setAwake();
    }
  }

  void Contact::calcContactMatrix()
  {
    Vector3 contactTangent[2];

    // Check whether the Z-axis is nearer to the X or Y axis
    if(ffabs(contactNormal.x) > ffabs(contactNormal.y))
    { 
      // Scaling factor to ensure the results are normalised
      ffloat den = ffsqrt(contactNormal.z * contactNormal.z + contactNormal.x * contactNormal.x);
      ffloat s = ffone;
      if (den == ffzero)
        contactNormal.y = ffone;
      else
        s = (ffone / den);
      // The new X-axis is at right angles to the world Y-axis
      contactTangent[0].x = contactNormal.z * s;
      contactTangent[0].y = ffzero;
      contactTangent[0].z = -contactNormal.x * s;

      // The new Y-axis is at right angles to the new X- and Z- axes
      contactTangent[1].x = (contactNormal.y * contactTangent[0].x);
      contactTangent[1].y = (contactNormal.z * contactTangent[0].x) - (contactNormal.x * contactTangent[0].z);
      contactTangent[1].z = (-contactNormal.y * contactTangent[0].x);
    }
    else
    {
      // Scaling factor to ensure the results are normalised
      ffloat den = ffsqrt((contactNormal.z * contactNormal.z) + (contactNormal.y * contactNormal.y));
      ffloat s = ffone;
      if (den == ffzero)
        contactNormal.y = ffone;
      else
        s = (ffone / den);

      // The new X-axis is at right angles to the world X-axis
      contactTangent[0].x = ffzero;
      contactTangent[0].y = (-contactNormal.z * s);
      contactTangent[0].z = (contactNormal.y * s);

      // The new Y-axis is at right angles to the new X- and Z- axes
      contactTangent[1].x = (contactNormal.y * contactTangent[0].z) - (contactNormal.z * contactTangent[0].y);
      contactTangent[1].y = (-contactNormal.x * contactTangent[0].z);
      contactTangent[1].z = (contactNormal.x * contactTangent[0].y);
    }

    contactToWorld.setComponents( contactNormal,  contactTangent[0],  contactTangent[1]);
  }

  void Contact::calcInternals(ffloat deltaTime)
  {
    if (!bodies[0]) swapBodies();
    assert(bodies[0]);

    calcContactMatrix();
    relativeContactPosition[0] = contactPoint - bodies[0]->position;
    if(bodies[1])
      relativeContactPosition[1] = contactPoint - bodies[1]->position;
    
    contactVelocity = calcLocalVelocity(0, deltaTime);
    if(bodies[1])
      contactVelocity -= calcLocalVelocity(1, deltaTime);

    calcDesiredDeltaVelocity(deltaTime);
  }

  void Contact::calcDesiredDeltaVelocity(ffloat deltaTime)
  {
    ffloat velocityFromAcc = (bodies[0]->lastFrameAcceleration * deltaTime).dot(contactNormal);
    if(bodies[1])
      velocityFromAcc -= (bodies[1]->lastFrameAcceleration * deltaTime).dot(contactNormal);

    //If the velocity is too small, restitution is ommitted
    if(ffabs(contactVelocity.x) < minRestituteVelocity)
      desiredDeltaVelocity = -contactVelocity.x;
    else
      desiredDeltaVelocity = -contactVelocity.x - restitution * (contactVelocity.x - velocityFromAcc);
  }

  Vector3 Contact::calcLocalVelocity(unsigned index, ffloat deltaTime)
  {

  }

  void Contact::applyImpulse(const Vector3 &impulse, RigidBody *body, Vector3 *velocityChange, Vector3 *rotationChange)
  {

  }

  void Contact::applyVelocityChange(Vector3 veloctiyChange[2], Vector3 rotationChange[2])
  {

  }

  void Contact::applyPositionChange(Vector3 velocityChange[2], Vector3 rotationDirection[2], ffloat rotationAmount[2], ffloat penetration)
  {

  }

                            
}