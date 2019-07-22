#include "world.h"

namespace physics
{
  int World::__pri_id = 0;

  World::World(const Vector3 &min, const Vector3 &max, unsigned maxContacts, unsigned iterations/* =0 */)
  :cProcessor(iterations)
  ,cGenerator(min, max)
  ,cData(maxContacts)
  {}

  World::~World()
  {}

  void World::resetPriIDGen()
  {
    __pri_id = 0;
  }

  int World::genPrimitiveID()
  {
    if (__pri_id >= 10000000)
      __pri_id = 0;
    return ++__pri_id;
  }

  int World::addPrimitive( Primitive *pri)
  {
    int id = genPrimitiveID();
    pri->id = id;
    primitives[id] = pri;
    pri->refreshAABB();

    cGenerator.addPrimitive(pri);
    return id;
  }

  void World::removePrimitive(int id)
  {
    MapPrimitives::iterator it = primitives.find(id);
    if (it != primitives.end())
    {
      Primitive *pri = it->second;
      cGenerator.removePrimitive(pri);

      primitives.erase(it);
    }
  }

  int World::addForceGenerator(RigidBody *rb, ForceGenerator *fg)
  {
    return fManager.add(rb, fg);
  }

  void World::removeForceGenerator(RigidBody *rb, int id)
  {
    fManager.remove(rb, id);
  }

  void World::addContactCeofficient(unsigned int a, unsigned int b, ffloat  friction, ffloat restitution)
  {
    cGenerator.addContactCeofficient(a, b, friction, restitution);
    cGenerator.addContactCeofficient(b, a, friction, restitution);
  }

  void World::setContactDefaultCeofficient(ffloat friction, ffloat restitution)
  {
    cData.defFriction = friction;
    cData.defRestitution = restitution;
  }

  void World::prepare()
  {
    for (MapPrimitives::iterator it = primitives.begin(); it != primitives.end(); it++)
    {
      if (it->second->body == 0) continue;
      it->second->body->clearAccumulators();
      it->second->body->updateDerivedData();
      it->second->updateTransform();
    }
  }

  void World::update(ffloat deltaTime)
  {
    cData.reset();
    fManager.updateForces(deltaTime);
    for (MapPrimitives::iterator it = primitives.begin(); it != primitives.end(); it++)
		{ 
			if (it->second->body == 0)
				continue;
			it->second->body->update(deltaTime);
		}

    cGenerator.update(deltaTime);
    cGenerator.generateContacts(&cData);

    cProcessor.processContacts(&cData, deltaTime);

    for (MapPrimitives::iterator it = primitives.begin(); it != primitives.end(); it++)
		{
			if (it->second->body == 0 || 
          it->second->isStatic ||
          !it->second->body->isAwake )
				continue;

			it->second->body->updateDerivedData();
			it->second->updateTransform();
		}
  }

  void World::renderPrimitives()
  {
    MapPrimitives::iterator it = primitives.begin();
    for(; it != primitives.end(); it ++)
    {
      it->second->render();
    }
  }

  void World::render()
  {
    renderPrimitives();
    fManager.render();
    cGenerator.render();
    cProcessor.render();
    cData.render();
  }

}