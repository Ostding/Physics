#include "collision_data.h"
#include "renderer.h"

namespace physics
{
  ffloat CollisionData::defFriction = ffloat(0.4f);
  ffloat CollisionData::defRestitution = ffloat(0.02f);

  CollisionData::CollisionData(unsigned maxContacts)
  {
    CollisionData::maxContacts = maxContacts;
    contactArray = new Contact[maxContacts];
    reset();
  }

  CollisionData::~CollisionData()
  {
    if (contactArray != 0)
    {
      delete contactArray;
      contactArray = 0;
    }
  }

  bool CollisionData::hasMoreContacts()
  {
    return contactsLeft > 0;
  }

  void CollisionData::reset()
  {
    contactsLeft = maxContacts;
    contactCount = 0;
    nextContact = contactArray;
  }

  void CollisionData::addContacts(unsigned count)
  {
    contactsLeft -= count;
    contactCount += count;
    nextContact += count;
  }

  void CollisionData::render()
  {
#ifndef DISABLE_RENDER
    // to do show contact point normal and penetration
    for (Contact *p = contactArray; p < nextContact; p++)
    {
      Renderer::renderContact(p);
    }
#endif
  }
}