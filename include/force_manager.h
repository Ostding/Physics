#ifndef __FORCEMANAGER_H__
#define __FORCEMANAGER_H__
#include <map>
#include "types.h"
#include "primitive.h"
#include "body.h"
#include "force_generator.h"

namespace physics
{
  class ForceManager
  {
  public:
    ~ForceManager();

    typedef map<int, ForceGenerator *> MapGenerators;

    // This compare method is used to make sure that
    // we can get same element in same map's iterations
    struct CompareByPriID
    {
      bool operator()(const RigidBody *b1, const RigidBody *b2) const
      {
        return b1->primitive->id < b2->primitive->id;
      }
    };
    typedef std::map<RigidBody *, MapGenerators, CompareByPriID> MapBodies;
    MapBodies registrations;

  public:
    static unsigned int __force_id;
    void resetID();
    unsigned int genID();

    unsigned int add(RigidBody *body, ForceGenerator *fg);
    ForceGenerator *remove(RigidBody *body, unsigned int id);
    void clear();
    void updateForces(ffloat duration);

    void render();
  };
}

#endif