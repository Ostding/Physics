#include "box.h"
#ifndef DISABLE_RENDER
#include "renderer.h"
#endif

namespace physics
{
  Box::Box()
      : Primitive(PRIMITIVE_TYPE::PRIT_BOX)
  {
    body = new RigidBody(this);
  }

  Box::Box(RigidBody *body)
      : Primitive(PRIMITIVE_TYPE::PRIT_BOX)
  {

    Box::body = body;
    initWorldCorners();
  }

  Box::Box(const Vector3 &extents)
      : Primitive(PRIMITIVE_TYPE::PRIT_BOX), extents(extents)
  {
    body = new RigidBody(this);
    initWorldCorners();
    updateCorners();
  }

  void Box::setExtents(const Vector3 &extents)
  {
    Box::extents = extents;
    updateCorners();
  }

  void Box::setPosition(const Vector3 &position)
  {
    body->setPosition(position);
    body->updateDerivedData();

    refreshAABB();
    updateTransform();
  }

  void Box::setOrientation(const Quaternion &direction)
  {
    body->setOrientation(direction);
    body->updateDerivedData();

    refreshAABB();
    updateTransform();
  }

  void Box::initInertiaTensor(const ffloat &mass)
  {
    Vector3 squares = extents.product(extents);
    ffloat f3 = ffone / ffloat(12);
    ffloat yz = mass * (squares.y + squares.z);
    ffloat xz = mass * (squares.x + squares.z);
    ffloat xy = mass * (squares.x + squares.y);

    Matrix3 tensor;
    tensor.setDiagonal(f3 * yz, f3 * xz, f3 * xy);

    body->setInertiaTensor(tensor);
  }

  void Box::refreshAABB()
  {
    Vector3 pt = body->getPosInWorldSpace(pointsLocal[0]);
    pointsWorld[0] = pt;

    ffloat xMax = pt.x, xMin = pt.x;
    ffloat yMax = pt.y, yMin = pt.y;
    ffloat zMax = pt.z, zMin = pt.z;
    for (int i = 1; i < pointsLocal.size(); i++)
    {
      pt = body->getPosInWorldSpace(pointsLocal[i]);
      pointsWorld[i] = pt;

      if (pt.x > xMax)
        xMax = pt.x;
      if (pt.x < xMin)
        xMin = pt.x;

      if (pt.y > yMax)
        yMax = pt.y;
      if (pt.y < yMin)
        yMin = pt.y;

      if (pt.z > zMax)
        zMax = pt.z;
      if (pt.z < zMin)
        zMin = pt.z;
    }

    aabb.set(Vector3(xMin, yMin, zMin), Vector3(xMax, yMax, zMax));
  }

  void Box::updateCorners()
  {
    pointsLocal.clear();
    pointsLocal.emplace_back(Vector3(-extents.x, -extents.y, -extents.z));
    pointsLocal.emplace_back(Vector3(-extents.x, -extents.y, extents.z));
    pointsLocal.emplace_back(Vector3(extents.x, -extents.y, -extents.z));
    pointsLocal.emplace_back(Vector3(extents.x, -extents.y, extents.z));
    pointsLocal.emplace_back(Vector3(-extents.x, extents.y, -extents.z));
    pointsLocal.emplace_back(Vector3(-extents.x, extents.y, extents.z));
    pointsLocal.emplace_back(Vector3(extents.x, extents.y, -extents.z));
    pointsLocal.emplace_back(Vector3(extents.x, extents.y, extents.z));
    refreshAABB();
  }

  void Box::initWorldCorners()
  {
    pointsWorld.emplace_back(Vector3::zero);
    pointsWorld.emplace_back(Vector3::zero);
    pointsWorld.emplace_back(Vector3::zero);
    pointsWorld.emplace_back(Vector3::zero);
    pointsWorld.emplace_back(Vector3::zero);
    pointsWorld.emplace_back(Vector3::zero);
    pointsWorld.emplace_back(Vector3::zero);
    pointsWorld.emplace_back(Vector3::zero);
  }

  void Box::findFarthestPointInDirection(const Vector3 &direction, Vector3 &pointWorld)
  {
    int i = 0;
    ffloat maxValue = pointsWorld[0].dot(direction);

    for (int j = 1; j < pointsWorld.size(); j++)
    {
      ffloat value = pointsWorld[j].dot(direction);
      if (maxValue < value)
      {
        i = j;
        maxValue = value;
      }
    }
    pointWorld = pointsWorld[i];
  }

  void Box::render()
  {
#ifndef DISABLE_RENDER
    Renderer::renderBox(this);
#endif
  }

  void Box::getAllPoints(std::vector<Vector3> &points)
  {
    for (int i = 0; i < pointsWorld.size(); i++)
    {
      points.push_back(pointsWorld[i]);
    }
  }

}