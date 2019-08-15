#include "plane.h"
#include "renderer.h"

namespace physics
{
  Plane::Plane()
  : Primitive(PRIMITIVE_TYPE::PRIT_PLANE)
  {}

  Plane::Plane(const Vector3 &direction, const Vector3 &extents, const ffloat &offset)
  :Primitive(PRIMITIVE_TYPE::PRIT_PLANE)
  ,direction(direction)
  ,extents(extents)
  ,offset(offset)
  {
    Plane::direction.normalise();
    refreshAABB();
  }

  void Plane::refreshAABB()
  {
    Vector3 center = direction * offset;
	  aabb.set2(center, extents);
  }

  void Plane::render()
  {
    Renderer::renderPlane(this);
  }
  
}