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
    Vector3 pos = getColumnVector(3);
    pointWorldUp = getPointInWorldSpace(pointLocalUp);
    pointWorldDown = getPointInWorldSpace(pointLocalDown);

    ffloat maxX, maxY, maxZ;
    ffloat minX, minY, minZ;
    if(pointWorldUp.x > pointWorldDown.x)
    {
      maxX = pointWorldUp.x + radius;
      minX = pointWorldDown.x - radius;
    }
    else
    {
      maxX = pointWorldDown.x + radius;
      minX = pointWorldUp.x - radius;
    }
    
    if(pointWorldUp.y > pointWorldDown.y)
    {
      maxY = pointWorldUp.y + radius;
      minY = pointWorldDown.y - radius;
    }
    else
    {
      maxY = pointWorldDown.y + radius;
      minY = pointWorldUp.y - radius;
    }

    if(pointWorldUp.z > pointWorldDown.z)
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

  void Capsule::render()
  {

  }

  void Capsule::findFarthestPointInDirection(const Vector3 &direction, Vector3 &pointWorld)
  {
    Vector3 dir = direction;
    dir.normalise();
    Vector3 offset = dir * radius;
    Vector3 ptA = pointWorldUp + offset;
    Vector3 ptB = pointWorldDown + offset;
    if(ptA.squareMag() > ptB.squareMag())
      pointWorld = ptA;
    else
      pointWorld = ptB;
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