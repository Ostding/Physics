#include "sphere.h"
#include "body.h"
#include "renderer.h"

namespace physics
{
  Sphere::Sphere()
  :Primitive(PRIMITIVE_TYPE::PRIT_SPHERE)
  {}

  Sphere::Sphere(ffloat radius)
  :Primitive(PRIMITIVE_TYPE::PRIT_SPHERE)
  ,radius(radius)
  ,squareRadius(radius * radius)
  {
    body = new RigidBody(this);
    refreshAABB();
  }

  void Sphere::refreshAABB()
  {
    Vector3 center(ffzero, ffzero, ffzero);
    center = body->getPosInWorldSpace(offset * center);
    Vector3 extents(radius, radius, radius);
    aabb.set2(center, extents);
  }

  void Sphere::render()
  {
    Renderer::renderSphere(this);
  }
}