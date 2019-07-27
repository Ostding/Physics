#include "box.h"
#include "renderer.h"

namespace physics
{
  Box::Box()
  :Primitive(PRIMITIVE_TYPE::PRIT_BOX)
  {
    body = new RigidBody(this);
  }

  Box::Box(RigidBody *body)
  :Primitive(PRIMITIVE_TYPE::PRIT_BOX)
  {

    Primitive::body = body;
    initWorldCorners();
  }

  Box::Box(const Vector3 &extents)
  :Primitive(PRIMITIVE_TYPE::PRIT_BOX)
  ,extents(extents)
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
    pointsLocal.push_back(Vector3(-extents.x, -extents.y,	-extents.z));
    pointsLocal.push_back(Vector3(-extents.x, -extents.y,	extents.z));
    pointsLocal.push_back(Vector3(extents.x,	-extents.y,	-extents.z));
    pointsLocal.push_back(Vector3(extents.x,	-extents.y,	extents.z));
    pointsLocal.push_back(Vector3(-extents.x, extents.y,	-extents.z));
    pointsLocal.push_back(Vector3(-extents.x, extents.y,	extents.z));
    pointsLocal.push_back(Vector3(extents.x,	extents.y,	-extents.z));
    pointsLocal.push_back(Vector3(extents.x,	extents.y,	extents.z));
    refreshAABB();
  }

  void Box::initWorldCorners()
  {
    pointsWorld.push_back(Vector3::zero);
    pointsWorld.push_back(Vector3::zero);
    pointsWorld.push_back(Vector3::zero);
    pointsWorld.push_back(Vector3::zero);
    pointsWorld.push_back(Vector3::zero);
    pointsWorld.push_back(Vector3::zero);
    pointsWorld.push_back(Vector3::zero);
    pointsWorld.push_back(Vector3::zero);
  }

  Vector3 Box::findFarthestPointInDirection(const Vector3 &dir)
  {
    Vector3 dirLocal = body->getDirectionInBodySpace(dir);
    ffloat x, y, z;
    if(dirLocal.x < ffzero)
      x = -extents.x;
    else if (dirLocal.x == ffzero)
      x = ffzero;
    else
      x = extents.x;

    if (dirLocal.y < ffzero)
      y = -extents.y;
    else if (dirLocal.y == ffzero)
      y = ffzero;
    else
      y = extents.y;

    if (dirLocal.z < ffzero)
      z = -extents.z;
    else if (dirLocal.z == ffzero)
      z = ffzero;
    else
      z = extents.z;
    
    return Vector3(x, y, z);
  }

  void Box::render()
  {
    Renderer::renderBox(this);
  }
}