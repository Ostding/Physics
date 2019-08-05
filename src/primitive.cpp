#include <assert.h>
#include "primitive.h"
#include "body.h"
#include "octree.h"

namespace physics
{
    Primitive::Primitive(PRIMITIVE_TYPE t)
    : id( 0 )
    , tContact( -1 )
    , tProfile( -1 )
    , tPrimitive (t)
    , body( 0 )
    , parent( 0 )
    , node( 0 )
    , isStatic( false )
    , isTrigger( false )
    , interCount( 0 )
    {
      body = new RigidBody(this);
    } 

    Primitive::~Primitive()
    {
      if(body != 0)
      {
        delete body;
        body = 0;
      }
    }

    void Primitive::refreshAABB()
    {      
      assert("You shoule implement refreshAABB method!!!");
    }

    void Primitive::render()
    {
      assert("You shoule implement render method!!!");
    }

    void Primitive::updateTransform()
    {
      transform = body->transformMatrix;
    }

    Vector3 Primitive::getColumnVector(unsigned index) const
    {
      return transform.getColumnVector(index);
    }

    Vector3 Primitive::getTransformPos() const
    {
      return transform.getTransformPos();
    }

    void Primitive::setPosition(const Vector3 &position)
    {
      body->setPosition(position);
		  body->updateDerivedData();
      updateTransform();
    }

    void Primitive::setOrientation(const Quaternion &direction)
    {
      body->setOrientation(direction);
      body->updateDerivedData();
      updateTransform();
    }

    Vector3 Primitive::getPosition()
    {
      return body->position;
    }

    Quaternion Primitive::getOrientation()
    {
      return body->orientation;
    }

    void Primitive::findFarthestPointInDirection(const Vector3 &dir, Vector3 &pointLocal, Vector3 &pointWorld)
    {
      pointLocal = Vector3::zero;
      pointWorld = Vector3::zero;
    }

    void Primitive::getAllPoints(std::vector<Vector3> &points)
    {

    }

    Vector3 Primitive::getPointInWorldSpace(const Vector3 &point)
    {
      return body->getPosInWorldSpace(point);
    }
}