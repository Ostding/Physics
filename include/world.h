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
      static unsigned int __pri_id;  
      void resetPriIDGen();
			unsigned int genPrimitiveID();

      unsigned int addPrimitive( Primitive *pri );
			void removePrimitive(unsigned int id);

      unsigned int  addForceGenerator(Primitive *p, ForceGenerator *fg);
			ForceGenerator * removeForceGenerator(Primitive *p, unsigned int id);

      void addContactCeofficient(unsigned int a, unsigned int b, ffloat  friction, ffloat restitution);
			void setContactDefaultCeofficient(ffloat friction, ffloat restitution);

      void prepare();
      void update(ffloat deltaTime);

      void render();
      void renderPrimitives();
  }; 
}

#endif