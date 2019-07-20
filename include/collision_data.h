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
      ffloat restitution;

      static ffloat defFriction;
      static ffloat defRestitution;

    public:
      CollisionData(unsigned maxContacts);
      ~CollisionData();

      bool hasMoreContacts();
      void reset();
      void addContacts(unsigned count);
      void render();
  };
}


#endif