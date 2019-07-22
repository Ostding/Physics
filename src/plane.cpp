#include "plane.h"
#include "renderer.h"

namespace physics
{
  Plane::Plane()
  : Primitive(PRIMITIVE_TYPE::PRIT_PLANE)
  {}

  Plane::Plane(Vector3 direction, Vector3 extents)
  :Primitive(PRIMITIVE_TYPE::PRIT_PLANE)
  ,direction(direction)
  {
    clacDiagonalPoints(extents);
    refreshAABB();
  }

  void Plane::clacDiagonalPoints(const Vector3 &extents)
  {
    Vector3 _lb = Vector3(-extents.x, 0, -extents.z);
    Vector3 _lt = Vector3(-extents.x, 0, extents.z);
    Vector3 _rb = Vector3(extents.x, 0, -extents.z);
    Vector3 _rt = Vector3(extents.x, 0, extents.z);
    Quaternion q = Quaternion::fromVectorToVector(Vector3::up, direction);
    ptLocalLB = q * _lb;
    ptLocalLT = q * _lt;
    ptLocalRB = q * _rb;
    ptLocalRT = q * _rt;
  }

  void Plane::updateTransform()
  {
    Primitive::updateTransform();
    ptLB = body->getPosInWorldSpace(ptLocalLB);
    ptLT = body->getPosInWorldSpace(ptLocalLT);
    ptRT = body->getPosInWorldSpace(ptLocalRT);
    ptRB = body->getPosInWorldSpace(ptLocalRB);
  }

  void Plane::refreshAABB()
  {
    aabb.set(ptLB, ptRT);
  }

  void Plane::render()
  {
    Renderer::renderPlane(this);
  }
  
}