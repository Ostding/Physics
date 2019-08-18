#include "gravity_force.h"

namespace physics
{
  GravityForce::GravityForce(const Vector3 & gravity)
  :gravity(gravity)
  {
  }

  void GravityForce::updateForce(RigidBody *body, ffloat deltaTime)
  {
    if (!body->hasFiniteMass()) return;
    
    body->addForce(gravity * body->getMass());
  }
}