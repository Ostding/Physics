#include "sphere.h"
#include "body.h"
#include "renderer.h"

namespace physics
{
  Sphere::Sphere()
  :Primitive(PRIMITIVE_TYPE::PRIT_SPHERE)
  {}

  Sphere::Sphere(const ffloat &radius)
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
    center = body->getPosInWorldSpace(center);
    Vector3 extents(radius, radius, radius);
    aabb.set2(center, extents);
  }

  void Sphere::render()
  {
    Renderer::renderSphere(this);
  }

  void Sphere::initInertiaTensor(const ffloat &mass)
  {
    ffloat coeff = ffloat(0.4f) * mass * radius * radius;
    Matrix3 tensor;
    tensor.setDiagonal(coeff, coeff, coeff);
    body->setInertiaTensor(tensor);
  }

  void Sphere::findFarthestPointInDirection(const Vector3 &direction, Vector3 &pointWorld)
  {
    Vector3 dir = direction;
    dir.normalise();
    Vector3 offset = dir * radius;
    Vector3 pos = getColumnVector(3);
    pointWorld =  pos + offset;
  }
}