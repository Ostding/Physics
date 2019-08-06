#include "contact_processor.h"

namespace physics 
{

  ContactProcessor::ContactProcessor(unsigned iterations, ffloat velocityEpsilon, ffloat positionEpsilon)
  {
    setIterations(iterations, iterations);
    setEpsilon(velocityEpsilon, positionEpsilon);
  }

  ContactProcessor::ContactProcessor(unsigned velocityIterations, unsigned positionIterations,
                                      ffloat velocityEpsilon, ffloat positionEpsilon)
  {
    setIterations(velocityIterations, positionIterations);
    setEpsilon(velocityEpsilon, positionEpsilon);
  }

  bool ContactProcessor::isValid()
  {
    return  (velocityIterations > 0) && (positionIterations > 0) &&
            (positionEpsilon >= ffzero) && (velocityEpsilon >= ffzero);
  }


  void ContactProcessor::setIterations(unsigned iterations)
  {
      setIterations(iterations, iterations);
  }

  void ContactProcessor::setIterations(unsigned velocityIterations, 
                                      unsigned positionIterations)
  {
      ContactProcessor::velocityIterations = velocityIterations;
      ContactProcessor::positionIterations = positionIterations;
  }

  void ContactProcessor::setEpsilon(ffloat velocityEpsilon, ffloat positionEpsilon)
  {
      ContactProcessor::velocityEpsilon = velocityEpsilon;
      ContactProcessor::positionEpsilon = positionEpsilon;
  }

  void ContactProcessor::processContacts(CollisionData *cData, ffloat deltaTime)
  {
    unsigned numContacts = cData->contactCount;
    if (numContacts == 0) return;

    if (!isValid()) return;

		Contact *contacts = cData->contactArray;
    prepareContacts(contacts, numContacts, deltaTime);
    adjustPositions(contacts, numContacts, deltaTime);
    adjustVelocities(contacts, numContacts, deltaTime);
  }

  void ContactProcessor::prepareContacts(Contact* contacts, unsigned numContacts, ffloat deltaTime)
  {
    Contact* lastContact = contacts + numContacts;
    for(Contact* contact=contacts; contact < lastContact; contact++)
    {
      contact->calcInternals(deltaTime);
    } 
  }


void ContactProcessor::adjustVelocities(Contact *c, unsigned numContacts, ffloat deltaTime)
{
  Vector3 velocityChange[2], rotationChange[2];
  Vector3 cp;

  // iteratively handle impacts in order of severity.
  velocityIterationsUsed = 0;
  while(velocityIterationsUsed < velocityIterations) 
  {
      // Find contact with maximum magnitude of probable velocity change.
      ffloat max = velocityEpsilon;
      unsigned index = numContacts;
      for(unsigned i = 0; i < numContacts; i++)
      {
        if (c[i].desiredDeltaVelocity > max)
        {
          max = c[i].desiredDeltaVelocity;
          index = i;
        }
      }
      if (index == numContacts) break;

      c[index].updateAwake();
      c[index].applyVelocityChange(velocityChange, rotationChange);
      
      // With the change in velocity of the two bodies, the update of 
      // contact velocities means that some of the relative closing 
      // velocities need recomputing.
      for (unsigned i = 0; i < numContacts; i++)
      {
        if (c[i].bodies[0])
        {
          if (c[i].bodies[0] == c[index].bodies[0])
          {
            cp = rotationChange[0].cross(c[i].relativeContactPosition[0]);

            cp += velocityChange[0];

            c[i].contactVelocity += c[i].contactToWorld.transposeTransform(cp);
            c[i].calcDesiredDeltaVelocity(deltaTime);

          }
          else if (c[i].bodies[0] == c[index].bodies[1])
          {
            cp = rotationChange[1].cross(c[i].relativeContactPosition[0]);

            cp += velocityChange[1];

            c[i].contactVelocity += c[i].contactToWorld.transposeTransform(cp);
            c[i].calcDesiredDeltaVelocity(deltaTime);
          }
        }

        if (c[i].bodies[1])
        {
          if (c[i].bodies[1] == c[index].bodies[0])
          {
            cp = rotationChange[0].cross(c[i].relativeContactPosition[1]);

            cp += velocityChange[0];

            c[i].contactVelocity -= c[i].contactToWorld.transposeTransform(cp);
            c[i].calcDesiredDeltaVelocity(deltaTime);
          }
          else if (c[i].bodies[1] == c[index].bodies[1])
          {
            cp = rotationChange[1].cross(c[i].relativeContactPosition[1]);

            cp += velocityChange[1];

            c[i].contactVelocity -= c[i].contactToWorld.transposeTransform(cp);
            c[i].calcDesiredDeltaVelocity(deltaTime);
          }
        }
      }
      velocityIterationsUsed++;
    }
  }

  void ContactProcessor::adjustPositions(Contact *c, unsigned numContacts, ffloat deltaTime)
  {
    unsigned i,index;
    Vector3 velocityChange[2], rotationChange[2];
    ffloat rotationAmount[2];
    ffloat max;
    Vector3 cp;

    for(i=0; i<numContacts; i++) 
      c[i].calcInternals(deltaTime);

    // iteratively resolve interpenetration in order of severity.
    positionIterationsUsed = 0;
    while(positionIterationsUsed < positionIterations)
    {
      // Find biggest penetration
      max = positionEpsilon;
      index = numContacts;
      for(i=0;i<numContacts;i++) 
      {
        if(c[i].penetration > max)
        {
          max = c[i].penetration;
          index = i;
        }
      }
      if (index == numContacts) break;

      // Match the awake state at the contact
      c[index].updateAwake();

      // Resolve the penetration.
      c[index].applyPositionChange(velocityChange, rotationChange, rotationAmount, max);

      // Again this action may have changed the penetration of other bodies, so we update contacts.
      for(i = 0; i < numContacts; i++)
      {
        if(c[i].bodies[0] == c[index].bodies[0])
        {
          cp = rotationChange[0].cross(c[i].relativeContactPosition[0]);

          cp += velocityChange[0];
          ffloat x = rotationAmount[0] * cp.dot(c[i].contactNormal);
          c[i].penetration -=  x;
        }
        else if(c[i].bodies[0]==c[index].bodies[1])
        {
          cp = rotationChange[1].cross(c[i].relativeContactPosition[0]);

          cp += velocityChange[1];

          c[i].penetration -= rotationAmount[1]*cp.dot(c[i].contactNormal);
        }

        if(c[i].bodies[1])
        {
          if(c[i].bodies[1]==c[index].bodies[0])
          {
            cp = rotationChange[0].cross(c[i].relativeContactPosition[1]);

            cp += velocityChange[0];

            c[i].penetration += rotationAmount[0]*cp.dot(c[i].contactNormal);
          }
          else if(c[i].bodies[1]==c[index].bodies[1])
          {
            cp = rotationChange[1].cross(c[i].relativeContactPosition[1]);

            cp += velocityChange[1];

            c[i].penetration += rotationAmount[1]*cp.dot(c[i].contactNormal);
          }
        }
      }
      positionIterationsUsed++;
    }
  }

  void ContactProcessor::render()
  {
    //todo
  }
}