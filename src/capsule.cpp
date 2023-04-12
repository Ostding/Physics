#include "capsule.h"
#ifndef DISABLE_RENDER
#include "renderer.h"
#endif

namespace physics
{
  Capsule::Capsule(RigidBody *body)
      : Primitive(PRIMITIVE_TYPE::PRIT_CAPSULE)
  {
    this->body = body;
  }

  Capsule::Capsule()
      : Primitive(PRIMITIVE_TYPE::PRIT_CAPSULE)
  {
    body = new RigidBody(this);
  }

  Capsule::Capsule(const ffloat &halfHeight, const ffloat &radius)
      : Primitive(PRIMITIVE_TYPE::PRIT_CAPSULE), halfHeight(halfHeight), radius(radius)
  {
    body = new RigidBody(this);
    pointLocalUp = Vector3(ffzero, halfHeight, ffzero);
    pointLocalDown = Vector3(ffzero, -halfHeight, ffzero);
  }

  void Capsule::refreshAABB()
  {
    pointWorldUp = getPointInWorldSpace(pointLocalUp);
    pointWorldDown = getPointInWorldSpace(pointLocalDown);

    ffloat maxX, maxY, maxZ;
    ffloat minX, minY, minZ;
    if (pointWorldUp.x > pointWorldDown.x)
    {
      maxX = pointWorldUp.x + radius;
      minX = pointWorldDown.x - radius;
    }
    else
    {
      maxX = pointWorldDown.x + radius;
      minX = pointWorldUp.x - radius;
    }

    if (pointWorldUp.y > pointWorldDown.y)
    {
      maxY = pointWorldUp.y + radius;
      minY = pointWorldDown.y - radius;
    }
    else
    {
      maxY = pointWorldDown.y + radius;
      minY = pointWorldUp.y - radius;
    }

    if (pointWorldUp.z > pointWorldDown.z)
    {
      maxZ = pointWorldUp.z + radius;
      minZ = pointWorldDown.z - radius;
    }
    else
    {
      maxZ = pointWorldDown.z + radius;
      minZ = pointWorldUp.z - radius;
    }

    aabb.set(Vector3(minX, minY, minZ), Vector3(maxX, maxY, maxZ));
  }

  void Capsule::initInertiaTensor(const ffloat &mass)
  {
    Matrix3 tensor;
    const ffloat one_div3 = ffone / ffloat(3);
    const ffloat one_div8 = ffone / ffloat(8);
    const ffloat one_div12 = ffone / ffloat(12);

    ffloat height = halfHeight * fftwo;

    ffloat cm; // cylinder mass
    ffloat sm; // mass of two hemispheres
    ffloat r2 = radius * radius;
    ffloat vc = ffpi * height * r2;
    ffloat vs = fftwo_pi * one_div3 * r2 * radius;
    ffloat va = vc + vs;
    cm = vc / va * mass;
    sm = vs / va * mass;

    // from cylinder
    tensor.data[4] = r2 * cm * ffhalf;
    tensor.data[0] = tensor.data[8] = tensor.data[4] * ffhalf + cm * height * height * one_div12;
    // from hemispheres
    ffloat temp0 = sm * fftwo * r2 / ffloat(5);
    tensor.data[4] += temp0 * fftwo;
    ffloat temp1 = height * ffhalf;
    ffloat temp2 = temp0 + sm * (temp1 * temp1 + ffloat(3) * one_div8 * height * radius);
    tensor.data[0] += temp2 * fftwo;
    tensor.data[8] += temp2 * fftwo;

    tensor.data[1] = tensor.data[2] = tensor.data[3] = tensor.data[5] = tensor.data[6] = tensor.data[7] = ffzero;

    body->setInertiaTensor(tensor);
  }

  void Capsule::render()
  {
#ifndef DISABLE_RENDER
    Renderer::renderCapsule(this);
#endif
  }

  void Capsule::findFarthestPointInDirection(const Vector3 &direction, Vector3 &pointWorld)
  {
    Vector3 dir = direction;
    dir.normalise();
    Vector3 offset = dir * radius;

    ffloat sma = pointWorldUp.dot(dir);
    ffloat smb = pointWorldDown.dot(dir);
    if (sma > smb)
      pointWorld = pointWorldUp + offset;
    else
      pointWorld = pointWorldDown + offset;
  }

  void Capsule::setPosition(const Vector3 &position)
  {
    body->setPosition(position);
    body->updateDerivedData();

    refreshAABB();
    updateTransform();
  }

  void Capsule::setOrientation(const Quaternion &direction)
  {
    body->setOrientation(direction);
    body->updateDerivedData();

    refreshAABB();
    updateTransform();
  }

}