#include "gravity_force.h"

namespace physics
{
  GravityForce::GravityForce(const Vector3 & gravity)
  :gravity(gravity)
  {
  }

  void GravityForce::updateForce(RigidBody *body, const ffloat &deltaTime)
  {
    if (!body->hasFiniteMass()) return;
    if (!body->isAwake) return;

    body->addForce(gravity * body->getMass());
  }
}