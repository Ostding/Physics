#include <assert.h>
#include "contact.h"

namespace physics
{

  ffloat Contact::minRestituteVelocity = ffloat(0.25f);
  ffloat Contact::maxAngularMove = ffloat(100);

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

    //Just wake up the sleeping one
    if (awake0 ^ awake1) {
        if (awake0) bodies[1]->setAwake();
        else bodies[0]->setAwake();
    }
  }

  void Contact::calcContactMatrix()
  {
    Vector3 contactTangent[2];

    //Check whether the Z-axis is nearer to the X or Y axis
    if(ffabs(contactNormal.x) > ffabs(contactNormal.y))
    { 
      //Scaling factor to ensure the results are normalised
      ffloat den = ffsqrt(contactNormal.z * contactNormal.z + contactNormal.x * contactNormal.x);
      ffloat s = ffone;
      if (den == ffzero)
      {
        contactNormal.y = ffone;
        contactTangent[0].x = contactNormal.z;
        contactTangent[0].y = ffzero;
        contactTangent[0].z = -contactNormal.x;
      }
      else
      {
        //The new X-axis is at right angles to the world Y-axis
        contactTangent[0].x = contactNormal.z / den;
        contactTangent[0].y = ffzero;
        contactTangent[0].z = -contactNormal.x / den;
      }

      //The new Y-axis is at right angles to the new X- and Z- axes
      contactTangent[1].x = (contactNormal.y * contactTangent[0].x);
      contactTangent[1].y = (contactNormal.z * contactTangent[0].x) - (contactNormal.x * contactTangent[0].z);
      contactTangent[1].z = (-contactNormal.y * contactTangent[0].x);
    }
    else
    {
      //Scaling factor to ensure the results are normalised
      ffloat den = ffsqrt((contactNormal.z * contactNormal.z) + (contactNormal.y * contactNormal.y));
      
      contactTangent[0].x = ffzero;
      if (den == ffzero)
      {
        contactNormal.y = ffone;
        contactTangent[0].y = (-contactNormal.z);
        contactTangent[0].z = (contactNormal.y);
      }
      else
      {
        //The new X-axis is at right angles to the world X-axis
        
        contactTangent[0].y = (-contactNormal.z / den);
        contactTangent[0].z = (contactNormal.y / den);
      }

      //The new Y-axis is at right angles to the new X- and Z- axes
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
    RigidBody *b = bodies[index];
    Vector3 velocity = b->rotation.cross(relativeContactPosition[index]);
    velocity += b->velocity;

    Vector3 v = contactToWorld.transposeTransform(velocity);

    Vector3 accVelocity = b->lastFrameAcceleration * deltaTime;
    accVelocity = contactToWorld.transposeTransform(accVelocity);
    //We ignore any component of acceleration in the contact normal direction, 
    //we are only interested in planar acceleration
    accVelocity.x = ffzero; 

    v += accVelocity;
    return v;
  }

  Vector3 Contact::calcContactImpulse()
  {
    Vector3 impulseContact;
    Matrix3 *iitWorld0 = &(bodies[0]->iitWorld);
    if(friction == ffzero)
    {
      //Calculate velcity's change in world space along contact tangential direction
      Vector3 deltaVelocityWorld = relativeContactPosition[0].cross(contactNormal);
      deltaVelocityWorld = iitWorld0->transform(deltaVelocityWorld);
  
      deltaVelocityWorld = deltaVelocityWorld.cross(relativeContactPosition[0]);

      //Get component in contact normal direction
      ffloat deltaVelocity = deltaVelocityWorld.dot(contactNormal);

      //Add the linear component of velocity change
      deltaVelocity += bodies[0]->inverseMass;

      if(bodies[1])
      {
        Matrix3 *iitWorld1 = &(bodies[1]->iitWorld);
        Vector3 deltaVelocityWorld = relativeContactPosition[1].cross(contactNormal);
        deltaVelocityWorld = iitWorld1->transform(deltaVelocityWorld);
        deltaVelocityWorld = deltaVelocityWorld.cross(relativeContactPosition[1]);

        ffloat deltaVelocity = deltaVelocityWorld.dot(contactNormal);
        deltaVelocity += bodies[1]->inverseMass;
      }

      impulseContact.x = desiredDeltaVelocity / deltaVelocity;
      impulseContact.y = ffzero;
      impulseContact.z = ffzero;
    }
    else
    {
      //The equivalent of a cross product in matrices is multiplication by a skew symmetric matrix,
      //we build the matrix for converting between linear and angular quantities.
      Matrix3 impulseToTorque;
      impulseToTorque.setSkewSymmetric(relativeContactPosition[0]);

      //Build the matrix to convert contact impulse to change in velocity in world space.
      Matrix3 deltaVelocityWorld = impulseToTorque; 
      deltaVelocityWorld *= *iitWorld0;
      deltaVelocityWorld *= impulseToTorque;
      deltaVelocityWorld *= -ffone;

      ffloat inverseMass = bodies[0]->inverseMass;

      if(bodies[1])
      {
        Matrix3 *iitWorld1 = &(bodies[1]->iitWorld);
        impulseToTorque.setSkewSymmetric(relativeContactPosition[1]);

        Matrix3 deltaVelocityWorld2 = impulseToTorque; 
        deltaVelocityWorld2 *= *iitWorld1;
        deltaVelocityWorld2 *= impulseToTorque;
        deltaVelocityWorld2 *= -ffone;

        deltaVelocityWorld += deltaVelocityWorld2;
        inverseMass += bodies[1]->inverseMass;
      }
      
      Matrix3 deltaVelocity = contactToWorld.transpose();
      deltaVelocity *= deltaVelocityWorld;
      deltaVelocity *= contactToWorld;

      deltaVelocity.data[0] += inverseMass;
      deltaVelocity.data[4] += inverseMass;
      deltaVelocity.data[8] += inverseMass;

      //Invert to get the impulse needed per unit velocity
      Matrix3 impulseMatrix = deltaVelocity.inverse();

      //Find the target velocities to kill
      Vector3 velKill(desiredDeltaVelocity, -contactVelocity.y,  -contactVelocity.z);

      //Find the impulse to kill target velocities
      impulseContact = impulseMatrix.transform(velKill);

      //Check for exceeding friction
      ffloat planarImpulse = ffsqrt(impulseContact.y*impulseContact.y + impulseContact.z*impulseContact.z);
      ffloat normalImpulse = impulseContact.x * friction;
      if (planarImpulse > normalImpulse)
      {
        if (planarImpulse != ffzero)
        {
          impulseContact.y /= planarImpulse;
          impulseContact.z /= planarImpulse;
        }

        impulseContact.x = deltaVelocity.data[0] + deltaVelocity.data[1]*friction*impulseContact.y + deltaVelocity.data[2]*friction*impulseContact.z;
        impulseContact.x = desiredDeltaVelocity / impulseContact.x;
        
        impulseContact.y *= friction * impulseContact.x;
        impulseContact.z *= friction * impulseContact.x;
      }
    }
    return impulseContact;
  }

  void Contact::applyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2])
  {
    Vector3 impulseContact = calcContactImpulse();
    Vector3 impulse = contactToWorld.transform(impulseContact);

    //Split in the impulse into linear and rotational components
    Vector3 impulsiveTorque = relativeContactPosition[0].cross(impulse) ;
    rotationChange[0] = bodies[0]->iitWorld.transform(impulsiveTorque);
    velocityChange[0].clear();
    velocityChange[0].addScaledVector(impulse, bodies[0]->inverseMass);

    //Apply the changes
    bodies[0]->addVelocity(velocityChange[0]);
		bodies[0]->addRotation(rotationChange[0]);

    if (bodies[1])
    {
			Vector3 impulsiveTorque = impulse.cross(relativeContactPosition[1]);
      rotationChange[1] = bodies[1]->iitWorld.transform(impulsiveTorque);
      velocityChange[1].clear();
      velocityChange[1].addScaledVector(impulse, -(bodies[1]->inverseMass));

      bodies[1]->addVelocity(velocityChange[1]);
      bodies[1]->addRotation(rotationChange[1]);
    }
  }

  void Contact::applyPositionChange(Vector3 velocityChange[2], Vector3 rotationDirection[2], ffloat rotationAmount[2], ffloat penetration)
  {
    ffloat angularMove[2];
    ffloat linearMove[2];
    ffloat angularInertia[2];
    ffloat linearInertia[2];
    ffloat totalInertia = ffzero;

    //We need to work out the inertia of each object in the direction
    //of the contact normal, due to angular inertia only. 
    for (unsigned i = 0; i < 2; i++) 
    {
      if(!bodies[i])
        continue;
      //Use the same procedure as for calculating frictionless
      //velocity change to work out the angular inertia.
      Vector3 angularInertiaWorld = relativeContactPosition[i].cross(contactNormal);
      angularInertiaWorld = bodies[i]->iitWorld.transform(angularInertiaWorld);
      angularInertiaWorld = angularInertiaWorld.cross(relativeContactPosition[i]);
      angularInertia[i] = angularInertiaWorld.dot(contactNormal);

      //The linear component is simply the inverse mass
      linearInertia[i] = bodies[i]->inverseMass;

      //Keep track of the total inertia from all components
      totalInertia += linearInertia[i] + angularInertia[i];
    }

    angularMove[0] = penetration * angularInertia[0] / totalInertia;
    linearMove[0] = penetration * bodies[0]->inverseMass / totalInertia;

    Vector3 projection = relativeContactPosition[0];
    projection.addScaledVector(contactNormal, -relativeContactPosition[0].dot(contactNormal));

    ffloat max = maxAngularMove * relativeContactPosition[0].mag();

    if(ffabs(angularMove[0]) > max)
      angularMove[0]  = (angularMove[0] > ffzero) ? max : -max;

    if(bodies[1])
    {
      angularMove[1] = (-penetration) * angularInertia[1] / totalInertia;
      linearMove[1] = (-penetration) * bodies[1]->inverseMass / totalInertia;
      // To avoid angular projections that are too great (when mass is large
      // but inertia tensor is small) limit the angular move.
      Vector3 projection = relativeContactPosition[1];
      projection.addScaledVector(contactNormal, -relativeContactPosition[1].dot(contactNormal));

      ffloat max = maxAngularMove * relativeContactPosition[0].mag();

      if(ffabs(angularMove[1]) > max)
        angularMove[1]  = (angularMove[1] > ffzero) ? max : -max;
    }

    for(unsigned i = 0; i<2; i++) 
    {
      if(!bodies[i])
        continue;

      if (angularMove[i] != ffzero)
      {
        Vector3 t = relativeContactPosition[i].cross(contactNormal);
        rotationDirection[i] = bodies[i]->iitWorld.transform(t);
        rotationAmount[i] = angularMove[i] / angularInertia[i];
      }
      else
      {
        rotationDirection[i].clear();
        rotationAmount[i] = ffone;
      }
      
      velocityChange[i] = contactNormal;
      if (rotationAmount[i] != ffzero)
        velocityChange[i] *= linearMove[i] / rotationAmount[i];
      else
        velocityChange[i] *= linearMove[i];
      
      bodies[i]->position.addScaledVector(contactNormal, linearMove[i]);
      bodies[i]->orientation.addScaledVector(rotationDirection[i], (rotationAmount[i] * ffhalf));
      bodies[i]->orientation.normalise();
    }
  }
  
}
