#include "body.h"

namespace physics
{

  ffloat RigidBody::sleepEpsilon = ffloat(30000LL); //0.3f

  RigidBody::RigidBody(Primitive *primitive)
    : linearDamp( ffzero )
    , powLinerDamp( ffzero )
    , angularDamp( ffzero )
    , powAngularDamp( ffzero )
    , friction( ffzero )
    , inverseMass( ffzero )
    , motion( ffzero )
    , canSleep( true )
    , primitive( primitive )
    {}

   void RigidBody::clearAccumulators()
   {

   }
}