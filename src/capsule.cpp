#include "capsule.h"

namespace physics
{
  Capsule::Capsule(RigidBody *body)
  :Primitive(PRIMITIVE_TYPE::PRIT_CAPSULE)
  {
    Capsule::body = body;
  }
  
  Capsule::Capsule()
  :Primitive(PRIMITIVE_TYPE::PRIT_CAPSULE)
  {
    body = new RigidBody(this);
  }

  Capsule::Capsule(ffloat halfHeight, ffloat radius)
  :Primitive(PRIMITIVE_TYPE::PRIT_CAPSULE)
  ,halfHeight(halfHeight)
  ,radius(radius)
  {
    body = new RigidBody(this);
    pointLocalUp = Vector3(ffzero, halfHeight, ffzero);
    pointLocalDown = Vector3(ffzero, -halfHeight, ffzero);
  }

  void Capsule::refreshAABB()
  {
    
  }

  void Capsule::render()
  {

  }

  void Capsule::findFarthestPointInDirection(const Vector3 &direction, Vector3 &pointLocal, Vector3 &pointWorld)
  {

  }
  
  void Capsule::setPosition(const Vector3 &position)
  {

  }

  void Capsule::setOrientation(const Quaternion &direction)
  {

  }

}