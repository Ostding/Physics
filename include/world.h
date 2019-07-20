#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>
#include "types.h"
#include "primitive.h"
#include "contact_generator.h"
#include "contact_processor.h"
#include "force_manager.h"

namespace physics
{
  class World
  {
    public:
      typedef std::map<int, Primitive *> MapPrimitives;
      MapPrimitives primitives;

      ForceManager fManager;
      ContactProcessor cProcessor;
      ContactGenerator cGenerator;
      CollisionData cData;

    public:
      World(const Vector3 &min, const Vector3 &max,  unsigned maxContacts, unsigned iterations=0);
      ~World();

    public:
      static int __pri_id;  
      void resetPriIDGen();
			int genPrimitiveID();

      int addPrimitive( Primitive *pri );
			void removePrimitive(int id);

      int  addForceGenerator(RigidBody *rb, ForceGenerator *fg);
			void removeForceGenerator(RigidBody *rb, int id);

      void addContactCeofficient(unsigned int a, unsigned int b, ffloat  friction, ffloat restitution);
			void setContactDefaultCeofficient(ffloat friction, ffloat restitution);

      void prepare();
      void update(ffloat deltaTime);

      void render();
      void renderPrimitives();
  }; 
}

#endif