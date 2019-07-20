#ifndef __CONTACT_PROCESSOR_H__
#define __CONTACT_PROCESSOR_H__

#include "types.h"
#include "collision_data.h"

namespace physics
{
  class ContactProcessor
  {
    public:
      unsigned velocityIterations;
      unsigned positionIterations;

      unsigned velocityIterationsUsed;
      unsigned positionIterationsUsed;

      ffloat velocityEpsilon;
      ffloat positionEpsilon;
    
    public:
      ContactProcessor( unsigned iterations, 
                        ffloat velocityEpsilon = ffloat(10000LL),
                        ffloat positionEpsilon = ffloat(10000LL));

      ContactProcessor( unsigned velocityIterations, 
                        unsigned positionIterations, 
                        ffloat velocityEpsilon = ffloat(10000LL),
                        ffloat positionEpsilon = ffloat(10000LL));

      bool isValid();

      void setIterations(unsigned iterations = 4);
      void setIterations(unsigned velocityIterations = 4, unsigned positionIterations = 4);      
      void setEpsilon(ffloat velocityEpsilon, ffloat positionEpsilon);

      void processContacts(CollisionData *cData, ffloat deltaTime);

      void render();
    protected:
      void prepareContacts(Contact *contactArray, unsigned numContacts, ffloat deltaTime);
      void adjustVelocities(Contact *contactArray, unsigned numContacts, ffloat deltaTime);
      void adjustPositions(Contact *contacts, unsigned numContacts, ffloat deltaTime);
  };
}

#endif