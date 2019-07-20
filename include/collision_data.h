#ifndef __COLLISION_DATA_H__
#define __COLLISION_DATA_H__

#include "types.h"
#include "contact.h"

namespace physics
{
  class CollisionData
  {
    public:
      unsigned maxContacts;
      Contact *contactArray;
      Contact *nextContact;

      int contactsLeft;
      unsigned contactCount;

      ffloat friction;
      ffloat defFriction;

      ffloat restitution;
      ffloat defRestitution;

    public:
      CollisionData(unsigned maxContacts)
      { 
        CollisionData::maxContacts = maxContacts;
        reset();
      }

      ~CollisionData()
      {
        if(contactArray != 0)
        {
          delete contactArray;
          contactArray = 0;
        }
      }

      bool hasMoreContacts()
      {
        return contactsLeft > 0;
      }

      void reset()
      {	
        contactsLeft = maxContacts;
        contactCount = 0;
        nextContact = contactArray;
      }

      void addContacts(unsigned count)
      {
        contactsLeft -= count;
        contactCount += count;
        nextContact += count;
      }
  };
}


#endif