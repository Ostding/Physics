#include "move_force.h"

namespace physics
{
  MoveForce::MoveForce(const Vector3 &acc, const Vector3 &point)
  :moveAcc (acc)
  ,point(point)
  {
  }

  void MoveForce::updateForce(RigidBody *body, const ffloat & deltaTime)
  {
    if (!body->hasFiniteMass()) return;
    body->addForceAtBodyPos(moveAcc * body->getMass(), point);
  }

  void MoveForce::setForceAcceleration(const Vector3 &acc)
  {
    moveAcc = acc;
  }
  void MoveForce::setForcePoint(const Vector3 &point)
  { 
    MoveForce::point = point;
  }
}