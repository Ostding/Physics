#include "force_manager.h"

namespace physics
{ 
  int ForceManager::__force_id = 0;

  void ForceManager::resetID()
  {
    __force_id = 0;
  }

  int ForceManager::genID()
  {
    if (__force_id > 10000000)
      __force_id = 0;
    return ++__force_id;
  }

  int ForceManager::add(RigidBody* body,  ForceGenerator *fg)
  {
    MapBodies::iterator it = registrations.find(body);
    MapGenerators gens;
    if (it != registrations.end())
      gens = it->second;
    int id = genID();
    fg->id = id;
    gens[id] = fg;

    registrations[body] = gens;
    return id;
  }

  void ForceManager::remove(RigidBody* body, int id)
  {
    MapBodies::iterator it = registrations.find(body);
    if (it != registrations.end())
    {
      MapGenerators::iterator itgen = it->second.find(id);
      if (itgen != it->second.end())
      {
        it->second.erase(itgen);
        if (it->second.empty())
        {
          registrations.erase(it);
        }
      }
    }
  }

  void ForceManager::clear()
  {
    registrations.erase(registrations.begin(), registrations.end());
  }

  void ForceManager::updateForces(ffloat deltaTime)
  {
    for (MapBodies::iterator it = registrations.begin(); it != registrations.end(); it++)
    {
      RigidBody *body = it->first;
      for (MapGenerators::iterator itor = it->second.begin(); itor != it->second.end(); itor++)
      {
        itor->second->updateForce(body, deltaTime);
      }
    }
  }
}