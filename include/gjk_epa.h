#ifndef __GJK_EPA_H__
#define __GJK_EPA_H__
#include "types.h"
#include "simplex.h"
#include "primitive.h"
#include "collision_data.h"
// #define DEBUG_GJKEPA
namespace physics
{
  class GjkEpa
  {
  public:
    static bool epaContactDetection(Simplex &aSimplex, Primitive *cpa, Primitive *cpb, CollisionData *cData);
    static bool extrapolateContactInformation(PolytopeFace *aClosestFace, Primitive *cpa, Primitive *cpb, CollisionData *cData);
    static bool generateContacts(Primitive *cpa, Primitive *cpb, CollisionData *cData);
  };
}

#endif