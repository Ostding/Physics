#include "plane.h"

namespace physics
{
  Plane::Plane()
  : Primitive(PRIMITIVE_TYPE::PRIT_PLANE)
  {}

  Plane::Plane(Vector3 direction, Vector3 center, Vector3 extents)
  :Primitive(PRIMITIVE_TYPE::PRIT_PLANE)
  ,direction(direction)
  ,center(center)
  ,extents(extents)
  {
    refreshAABB();
  }

  void Plane::refreshAABB()
  {
    aabb.set2(center, extents);
  }
}