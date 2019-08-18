#include "force_manager.h"

namespace physics
{ 
  unsigned int ForceManager::__force_id = 1;

  void ForceManager::resetID()
  {
    __force_id = 1;
  }

  unsigned int ForceManager::genID()
  {
    if (__force_id > 10000000)
      __force_id = 1;
    return ++__force_id;
  }

  ForceManager::~ForceManager()
  {
    for (MapBodies::iterator it = registrations.begin(); it != registrations.end(); it++)
    {
      for (MapGenerators::iterator itor = it->second.begin(); itor != it->second.end(); itor++)
      {
        ForceGenerator *f = itor->second;
        if(f != 0)
          delete f;
      }
      it->second.clear();
    }
    registrations.clear();
  }

  unsigned int ForceManager::add(RigidBody* body,  ForceGenerator *fg)
  {
    MapBodies::iterator it = registrations.find(body);
    MapGenerators gens;
    if (it != registrations.end())
      gens = it->second;
    unsigned int id = genID();
    fg->id = id;
    gens[id] = fg;

    registrations[body] = gens;
    return id;
  }

  ForceGenerator * ForceManager::remove(RigidBody* body, unsigned int id)
  {
    ForceGenerator *f = 0;
    MapBodies::iterator it = registrations.find(body);
    if (it != registrations.end())
    {
      MapGenerators::iterator itgen = it->second.find(id);
      if (itgen != it->second.end())
      {
        it->second.erase(itgen);
        if (it->second.empty())
        {
          f = itgen->second;
          registrations.erase(it);
        }
      }
    }
    return f;
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

  void ForceManager::render()
  {
    //todo show force direction etc
  }
}